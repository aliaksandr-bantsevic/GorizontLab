//---------------------------------------------------------------------------
#include <stdio.h>
#include <mem.h>
#include <math.h>

#pragma hdrstop
#include "Protocol_ASIN.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TProtocol_ASIN::TProtocol_ASIN(void)
{
        Clear();
}

TProtocol_ASIN::~TProtocol_ASIN(void)
{

}

void TProtocol_ASIN::Clear(void)
{
        memset(bufTX,0,1024);
        memset(bufRX,0,1024);
}

BYTE TProtocol_ASIN::CheckSumCalculate(BYTE* buf, int len)
{
        BYTE csum=0;
        for (int i=0; i<len; i++)
        {
            csum+=buf[i]; csum&=0xff;
        }

        csum=0x100-csum;
        return csum;
}

int TProtocol_ASIN::EscapeBytesDecode(unsigned char* buf, int* len)
{
        BYTE buft[1024];
        int idx=0;
        for (int i=0; i<*len; i++)
        {
                if (buf[i]==PACKET_END) return 1;

                if (buf[i]!=ESCAPE_BYTE)
                {
                        buft[idx]=buf[i];idx++;
                }
                else
                {
                        buft[idx]=buf[i+1]^0x20; idx++; i++;
                }
        }

        *len=idx;
        memcpy(buf,buft,idx);
        return 0;
}

int TProtocol_ASIN::EscapeBytesEncode(unsigned char* buf, int* len)
{
        BYTE buft[1024];
        int idx=0;
        for (int i=0; i<*len; i++)
        {
                if ((buf[i]!=PACKET_END)&&(buf[i]!=ESCAPE_BYTE))
                {
                        buft[idx]=buf[i];idx++;
                }
                else
                {
                         if (buf[i]==PACKET_END)
                         {
                            buft[idx]=ESCAPE_BYTE;idx++;
                            buft[idx]=CODE7E_BYTE;idx++;
                         }
                         if (buf[i]==ESCAPE_BYTE)
                         {
                            buft[idx]=ESCAPE_BYTE;idx++;
                            buft[idx]=CODE7D_BYTE;idx++;
                         }

                }
        }

        *len=idx;
        memcpy(buf,buft,idx);
        return 0;
}


int TProtocol_ASIN::MakeRequest(cmdASIN cmd, BYTE** buf, int* len)
{
        //TODO: Add your source code here
        lenCMD=0; int idx=0;
        //bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START;
        bufTX[idx++]=cmd;
        bufTX[idx++]=this->CheckSumCalculate(&bufTX[1],1);
        lenCMD=idx;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

int TProtocol_ASIN::PacketProc(BYTE* buf, int* len)
{
        //TODO: Add your source code here
        int ln=*len;

        if (buf[0]==PACKET_END)
        {
            ln--;
            memcpy(&buf[0],&buf[1],ln);
        }

        if (buf[0]!=PACKET_START) return 1;
        if (buf[ln-1]!=PACKET_END) return 1;
        ln-=2;
        this->EscapeBytesDecode(buf,&ln);
        if (this->CheckSumCalculate(&buf[1],ln-1)!=buf[ln]) return 1;
        *len=ln+2;
        return 0;
}

int TProtocol_ASIN::PacketProcVersion(BYTE* buf, char* ver)
{
        //TODO: Add your source code here
        memcpy(ver,&buf[2],5); ver[5]='\0';
        return 0;
}

int TProtocol_ASIN::PacketProcList(BYTE* buf, BYTE* list)
{
        //TODO: Add your source code here

        int inum=buf[2];
        list[0]=buf[2];
        memcpy(&list[1],&buf[3],inum);
        list[inum+1]=0;

        return 0;
}

int TProtocol_ASIN::MakeRequestNewAddr(cmdASIN cmd, BYTE** buf, int* len, BYTE oldaddr, BYTE newaddr)
{
        //TODO: Add your source code here
        lenCMD=0; int idx=0;
        bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START;
        bufTX[idx++]=cmd;
        bufTX[idx++]=oldaddr;
        bufTX[idx++]=newaddr;
        bufTX[idx++]=this->CheckSumCalculate(&bufTX[1],1);
        lenCMD=idx;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

int TProtocol_ASIN::MakeRequestMeas211(BYTE** buf, int* len, BYTE addr)
{
        //TODO: Add your source code here
        lenCMD=0; int idx=0; BYTE csum=0;
        bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START_211;          csum^=PACKET_START_211;
        bufTX[idx++]=ASIN_CDM_MEASALL_IND3;      csum^=ASIN_CDM_MEASALL_IND3;
        bufTX[idx++]=addr;                      csum^=addr;
        bufTX[idx++]=csum;
        lenCMD=idx-1;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD+1;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

int TProtocol_ASIN::PacketProcMeas211(BYTE* buf, int* len, BYTE addr, measresASIN* res)
{
        //TODO: Add your source code here
        int ln=*len;

        if (buf[0]==PACKET_END)
        {
            ln--;
            memcpy(&buf[0],&buf[1],ln);
        }

        if (buf[0]!=PACKET_START_211)           return -1;  //Неправильный формат пакета
        if (buf[ln-1]!=PACKET_END)              return -1;
        if (buf[2]!=addr)                       return -2; //Неправильный адрес

        ln-=1;
        this->EscapeBytesDecode(buf,&ln);
        ln+=1;

        BYTE csum=0;
        for (int i=0; i<ln; i++)
        {
             csum^=buf[i];
        }

        if (csum!=buf[ln-1])    return -3; //Неправильная КС
        *len=ln+2;

        int idx=8;

        BYTE signx= (buf[idx]&(1<<7))>>7;     //бит знак результата по X
        res->unit_x=(buf[idx]&(1<<6))>>6;     //Размерность по X
        unsigned int iresx=(unsigned int)buf[idx--];
        iresx&=0x3F;
        iresx<<=8;
        iresx+=(unsigned int)buf[idx--];
        unsigned int dresx=(unsigned int)buf[idx--];
        float fresx=(double)iresx+(double)dresx/256.;
        if (signx) fresx*=-1.;
        res->angle_x=fresx;

        BYTE signy=(buf[idx]&(1<<7))>>7;      //бит знак результата по Y
        res->unit_y=(buf[idx]&(1<<6))>>6;     //Размерность по Y
        unsigned int iresy=(unsigned int)buf[idx--];
        iresy&=0x3F;
        iresy<<=8;
        iresy+=(unsigned int)buf[idx--];
        unsigned int dresy=(unsigned int)buf[idx--];
        float fresy=(double)iresy+(double)dresy/256.;
        if (signy) fresy*=-1.;
        res->angle_y=fresy;

        return 0;
}

int TProtocol_ASIN::MakeRequestVersionASIN(BYTE** buf, int* len)
{
        //TODO: Add your source code here
        lenCMD=0; int idx=0;
        //bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START;
        bufTX[idx++]=ASIN_CMD_VERSION;
        bufTX[idx++]=this->CheckSumCalculate(&bufTX[1],1);
        lenCMD=idx;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

int TProtocol_ASIN::PacketProcVersionASIN(BYTE* buf, int* len, char* ver)
{
        //TODO: Add your source code here
        int ln=*len;

        if (buf[0]==PACKET_END)
        {
            ln--;
            memcpy(&buf[0],&buf[1],ln);
        }

        if (buf[0]!=PACKET_START)               return -1;  //Неправильный формат пакета
        if (buf[ln-1]!=PACKET_END)              return -1;


        ln-=2;
        this->EscapeBytesDecode(buf,&ln);

        BYTE csum=this->CheckSumCalculate(&buf[1],ln-1);

        if (csum!=buf[ln])    return -3; //Неправильная КС
        *len=ln+1;

        memcpy(ver,(char*)&buf[2],5); ver[5]=0;

        return 0;
}


int TProtocol_ASIN::MakeRequestSensorsList(BYTE** buf, int* len)
{
        //TODO: Add your source code here
        lenCMD=0; int idx=0;
        //bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START;
        bufTX[idx++]=ASIN_CMD_SLIST;
        bufTX[idx++]=this->CheckSumCalculate(&bufTX[1],1);
        lenCMD=idx;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

int TProtocol_ASIN::PacketProcSensorsList(BYTE* buf, int* len, BYTE* list)
{
        //TODO: Add your source code here
        int ln=*len;

        if (buf[0]==PACKET_END)
        {
            ln--;
            memcpy(&buf[0],&buf[1],ln);
        }

        if (buf[0]!=PACKET_START)               return -1;  //Неправильный формат пакета
        if (buf[ln-1]!=PACKET_END)              return -1;

        ln-=2;
        this->EscapeBytesDecode(buf,&ln);

        BYTE csum=this->CheckSumCalculate(&buf[1],ln-1);

        if (csum!=buf[ln])    return -3; //Неправильная КС
        *len=ln+1;

        memcpy(list,&buf[2],buf[2]+1);

        return 0;
}

int TProtocol_ASIN::MakeRequestRevisionASIN(BYTE** buf, int* len)
{
        //TODO: Add your source code here
        lenCMD=0; int idx=0;
        bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START;
        bufTX[idx++]=ASIN_CMD_REVISION_ASIN;
        bufTX[idx++]=this->CheckSumCalculate(&bufTX[2],1);
        lenCMD=idx;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

int TProtocol_ASIN::PacketProcRevisionASIN(BYTE* buf, int* len, unsigned int* rev)
{
        //TODO: Add your source code here
        int ln=*len;

        if (buf[0]==PACKET_END)
        {
            ln--;
            memcpy(&buf[0],&buf[1],ln);
        }

        if (buf[0]!=PACKET_START)               return -1;  //Неправильный формат пакета
        if (buf[ln-1]!=PACKET_END)              return -1;

        ln-=2;
        this->EscapeBytesDecode(buf,&ln);

        BYTE csum=this->CheckSumCalculate(&buf[1],ln-1);

        if (csum!=buf[ln])    return -3; //Неправильная КС
        *len=ln+1;

        *rev=(((unsigned int)buf[3])<<8)+(unsigned int)buf[2];

        return 0;
}

//------------------------------------------------------------------------------
//Запросить версию ПО датчика ИН-Д3
int TProtocol_ASIN::MakeRequestIND3VersionASIN(BYTE** buf, int* len, BYTE addr)
{
        //TODO: Add your source code here
        BYTE csum=0;
        lenCMD=0; int idx=0;
        bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START_211;          csum^=PACKET_START_211;
        bufTX[idx++]=ASIN_CMD_VERSION_IND3;     csum^=ASIN_CMD_VERSION_IND3;
        bufTX[idx++]=addr;                      csum^=addr;
        bufTX[idx++]=csum;
        lenCMD=idx-1;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD+1;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

//Распаковать пакет версия ПО датчика ИН-Д3
int TProtocol_ASIN::PacketProcIND3VersionASIN(BYTE* buf, int* len, BYTE addr, char* ver)
{
        //TODO: Add your source code here
        int ln=*len;

        if (buf[0]==PACKET_END)
        {
            ln--;
            memcpy(&buf[0],&buf[1],ln);
        }

        if (buf[0]!=PACKET_START_211)               return -1;  //Неправильный формат пакета
        if (buf[1]!=ASIN_CMD_VERSION_IND3)          return -1;
        if (buf[2]!=addr)                           return -2;  //Неправильный адрес
        if (buf[ln-1]!=PACKET_END)                  return -1;

        ln-=1;
        this->EscapeBytesDecode(buf,&ln);

        //BYTE csum=this->CheckSumCalculate(&buf[1],ln-1);
        BYTE csum=0;
        for (int i=0; i<ln-1; i++) csum^=buf[i];

        if (csum!=buf[ln-1])                        return -3; //Неправильная КС
        *len=ln+1;

        memcpy(ver,(char*)&buf[3],5); ver[5]=0;

        return 0;
}

//------------------------------------------------------------------------------
//Запросить заводскй номер датчика ИН-Д3
int TProtocol_ASIN::MakeRequestIND3SerialASIN(BYTE** buf, int* len, BYTE addr)
{
        //TODO: Add your source code here
        BYTE csum=0;
        lenCMD=0; int idx=0;
        bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START_211_1;          csum^=PACKET_START_211_1;
        bufTX[idx++]=ASIN_CMD_SERIAL_IND3;        csum^=ASIN_CMD_SERIAL_IND3;
        bufTX[idx++]=addr;                        csum^=addr;
        bufTX[idx++]=csum;
        lenCMD=idx-1;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD+1;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

//Распаковать пакет заводской номер датчика ИН-Д3
int TProtocol_ASIN::PacketProcIND3SerialASIN(BYTE* buf, int* len, BYTE addr, unsigned int* sn)
{
        //TODO: Add your source code here
        int ln=*len;

        if (buf[0]==PACKET_END)
        {
            ln--;
            memcpy(&buf[0],&buf[1],ln);
        }

        if (buf[0]!=PACKET_START_211_1)             return -1;  //Неправильный формат пакета
        if (buf[1]!=ASIN_CMD_SERIAL_IND3)           return -1;
        if (buf[2]!=addr)                           return -2;  //Неправильный адрес
        if (buf[ln-1]!=PACKET_END)                  return -1;

        ln-=1;
        this->EscapeBytesDecode(buf,&ln);

        //BYTE csum=this->CheckSumCalculate(&buf[1],ln-1);
        BYTE csum=0;
        for (int i=0; i<ln-1; i++) csum^=buf[i];

        if (csum!=buf[ln-1])                        return -3; //Неправильная КС
        *len=ln+1;

        *sn=*(unsigned int*)&buf[3];

        return 0;
}

//------------------------------------------------------------------------------
//Запросить редакция прошивки датчика ИН-Д3
int TProtocol_ASIN::MakeRequestIND3EditionASIN(BYTE** buf, int* len, BYTE addr)
{
        //TODO: Add your source code here
        BYTE csum=0;
        lenCMD=0; int idx=0;
        bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START_211_1;          csum^=PACKET_START_211_1;
        bufTX[idx++]=ASIN_CMD_EDITION_IND3;       csum^=ASIN_CMD_EDITION_IND3;
        bufTX[idx++]=addr;                        csum^=addr;
        bufTX[idx++]=csum;
        lenCMD=idx-1;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD+1;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

//Распаковать пакет редакция прошивки датчика ИН-Д3
int TProtocol_ASIN::PacketProcIND3EditionASIN(BYTE* buf, int* len, BYTE addr, unsigned short* ed)
{
        //TODO: Add your source code here
        int ln=*len;

        if (buf[0]==PACKET_END)
        {
            ln--;
            memcpy(&buf[0],&buf[1],ln);
        }

        if (buf[0]!=PACKET_START_211_1)             return -1;  //Неправильный формат пакета
        if (buf[1]!=ASIN_CMD_EDITION_IND3)           return -1;
        if (buf[2]!=addr)                           return -2;  //Неправильный адрес
        if (buf[ln-1]!=PACKET_END)                  return -1;

        ln-=1;
        this->EscapeBytesDecode(buf,&ln);

        //BYTE csum=this->CheckSumCalculate(&buf[1],ln-1);
        BYTE csum=0;
        for (int i=0; i<ln-1; i++) csum^=buf[i];

        if (csum!=buf[ln-1])                        return -3; //Неправильная КС
        *len=ln+1;

        *ed=*(unsigned short*)&buf[3];

        return 0;
}


//------------------------------------------------------------------------------
//Запросить имя датчика ИН-Д3
int TProtocol_ASIN::MakeRequestIND3NameASIN(BYTE** buf, int* len, BYTE addr)
{
        //TODO: Add your source code here
        BYTE csum=0;
        lenCMD=0; int idx=0;
        bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START_211_1;          csum^=PACKET_START_211_1;
        bufTX[idx++]=ASIN_CMD_NAME_IND3;          csum^=ASIN_CMD_NAME_IND3;
        bufTX[idx++]=addr;                        csum^=addr;
        bufTX[idx++]=csum;
        lenCMD=idx-1;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD+1;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

//Распаковать пакет имя датчика ИН-Д3
int TProtocol_ASIN::PacketProcIND3NameASIN(BYTE* buf, int* len, BYTE addr, char* name)
{
        //TODO: Add your source code here
        int ln=*len;

        if (buf[0]==PACKET_END)
        {
            ln--;
            memcpy(&buf[0],&buf[1],ln);
        }

        if (buf[0]!=PACKET_START_211_1)             return -1;  //Неправильный формат пакета
        if (buf[1]!=ASIN_CMD_NAME_IND3)            return -1;
        if (buf[2]!=addr)                           return -2;  //Неправильный адрес
        if (buf[ln-1]!=PACKET_END)                  return -1;

        ln-=1;
        this->EscapeBytesDecode(buf,&ln);

        //BYTE csum=this->CheckSumCalculate(&buf[1],ln-1);
        BYTE csum=0;
        for (int i=0; i<ln-1; i++) csum^=buf[i];

        if (csum!=buf[ln-1])                        return -3; //Неправильная КС
        *len=ln+1;

        memcpy(name,(char*)&buf[3],ln-4); name[ln-4]=0;

        return 0;
}

//------------------------------------------------------------------------------
//Запросить скрость датчика ИН-Д3
int TProtocol_ASIN::MakeRequestIND3BaudASIN(BYTE** buf, int* len, BYTE addr)
{
        //TODO: Add your source code here
        BYTE csum=0;
        lenCMD=0; int idx=0;
        bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START_211_1;          csum^=PACKET_START_211_1;
        bufTX[idx++]=ASIN_CMD_BAUD_IND3;          csum^=ASIN_CMD_BAUD_IND3;
        bufTX[idx++]=addr;                        csum^=addr;
        bufTX[idx++]=csum;
        lenCMD=idx-1;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD+1;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

//Распаковать пакет скорость датчика ИН-Д3
int TProtocol_ASIN::PacketProcIND3BaudASIN(BYTE* buf, int* len, BYTE addr, DWORD* baud)
{
        //TODO: Add your source code here
        int ln=*len;

        if (buf[0]==PACKET_END)
        {
            ln--;
            memcpy(&buf[0],&buf[1],ln);
        }

        if (buf[0]!=PACKET_START_211_1)             return -1;  //Неправильный формат пакета
        if (buf[1]!=ASIN_CMD_BAUD_IND3)            return -1;
        if (buf[2]!=addr)                           return -2;  //Неправильный адрес
        if (buf[ln-1]!=PACKET_END)                  return -1;

        ln-=1;
        this->EscapeBytesDecode(buf,&ln);

        //BYTE csum=this->CheckSumCalculate(&buf[1],ln-1);
        BYTE csum=0;
        for (int i=0; i<ln-1; i++) csum^=buf[i];

        if (csum!=buf[ln-1])                        return -3; //Неправильная КС
        *len=ln+1;

/*
typedef enum {

        baud_1200  =  0x01,
	baud_2400  =  0x02,
	baud_4800  =  0x03,
	baud_9600  =  0x04,
	baud_19200 =  0x05,
	baud_38400 =  0x06,
	baud_57600 =  0x07,
	baud_115200=  0x08

} ind3_baudrate;
*/

        switch (buf[3])
        {
                case 0x01:
                        *baud=1200;
                break;

                case 0x02:
                        *baud=2400;
                break;

                case 0x03:
                        *baud=4800;
                break;

                case 0x04:
                        *baud=9600;
                break;

                case 0x05:
                        *baud=19200;
                break;

                case 0x06:
                        *baud=38400;
                break;

                case 0x07:
                        *baud=57600;
                break;

                case 0x08:
                        *baud=115200;
                break;

                default:
                        *baud=115200;

        }

        return 0;
}

//------------------------------------------------------------------------------
//Запросить смещения датчика ИН-Д3
int TProtocol_ASIN::MakeRequestIND3OffsetASIN(BYTE** buf, int* len, BYTE addr)
{
        //TODO: Add your source code here
        BYTE csum=0;
        lenCMD=0; int idx=0;
        bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START_211_1;          csum^=PACKET_START_211_1;
        bufTX[idx++]=ASIN_CMD_OFFSET_IND3;        csum^=ASIN_CMD_OFFSET_IND3;
        bufTX[idx++]=addr;                        csum^=addr;
        bufTX[idx++]=csum;
        lenCMD=idx-1;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD+1;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

//Распаковать смещения датчика ИН-Д3
int TProtocol_ASIN::PacketProcIND3OffsetASIN(BYTE* buf, int* len, BYTE addr, measresASIN* res)
{
        //TODO: Add your source code here
        int ln=*len;

        if (buf[0]==PACKET_END)
        {
            ln--;
            memcpy(&buf[0],&buf[1],ln);
        }

        if (buf[0]!=PACKET_START_211_1)             return -1;  //Неправильный формат пакета
        if (buf[1]!=ASIN_CMD_OFFSET_IND3)             return -1;
        if (buf[2]!=addr)                           return -2;  //Неправильный адрес
        if (buf[ln-1]!=PACKET_END)                  return -1;

        ln-=1;
        this->EscapeBytesDecode(buf,&ln);

        //BYTE csum=this->CheckSumCalculate(&buf[1],ln-1);
        BYTE csum=0;
        for (int i=0; i<ln-1; i++) csum^=buf[i];

        if (csum!=buf[ln-1])                        return -3; //Неправильная КС
        *len=ln+1;


        int idx=8;

        BYTE signx= (buf[idx]&(1<<7))>>7;     //бит знак результата по X
        res->unit_x=(buf[idx]&(1<<6))>>6;     //Размерность по X
        unsigned int iresx=(unsigned int)buf[idx--];
        iresx&=0x3F;
        iresx<<=8;
        iresx+=(unsigned int)buf[idx--];
        unsigned int dresx=(unsigned int)buf[idx--];
        float fresx=(double)iresx+(double)dresx/256.;
        if (signx) fresx*=-1.;
        res->angle_x=fresx;

        BYTE signy=(buf[idx]&(1<<7))>>7;      //бит знак результата по Y
        res->unit_y=(buf[idx]&(1<<6))>>6;     //Размерность по Y
        unsigned int iresy=(unsigned int)buf[idx--];
        iresy&=0x3F;
        iresy<<=8;
        iresy+=(unsigned int)buf[idx--];
        unsigned int dresy=(unsigned int)buf[idx--];
        float fresy=(double)iresy+(double)dresy/256.;
        if (signy) fresy*=-1.;
        res->angle_y=fresy;



        return 0;
}

//------------------------------------------------------------------------------
//Запросить такты усреднения датчика ИН-Д3
int TProtocol_ASIN::MakeRequestIND3TactsASIN(BYTE** buf, int* len, BYTE addr)
{
        //TODO: Add your source code here
        BYTE csum=0;
        lenCMD=0; int idx=0;
        bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START_211_1;          csum^=PACKET_START_211_1;
        bufTX[idx++]=ASIN_CMD_TACTS_IND3;         csum^=ASIN_CMD_TACTS_IND3;
        bufTX[idx++]=addr;                        csum^=addr;
        bufTX[idx++]=csum;
        lenCMD=idx-1;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD+1;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

//Распаковать такты усреднения датчика ИН-Д3
int TProtocol_ASIN::PacketProcIND3TactsASIN(BYTE* buf, int* len, BYTE addr, WORD* tacts)
{
        //TODO: Add your source code here
        int ln=*len;

        if (buf[0]==PACKET_END)
        {
            ln--;
            memcpy(&buf[0],&buf[1],ln);
        }

        if (buf[0]!=PACKET_START_211_1)             return -1;  //Неправильный формат пакета
        if (buf[1]!=ASIN_CMD_TACTS_IND3)            return -1;
        if (buf[2]!=addr)                           return -2;  //Неправильный адрес
        if (buf[ln-1]!=PACKET_END)                  return -1;

        ln-=1;
        this->EscapeBytesDecode(buf,&ln);

        //BYTE csum=this->CheckSumCalculate(&buf[1],ln-1);
        BYTE csum=0;
        for (int i=0; i<ln-1; i++) csum^=buf[i];

        if (csum!=buf[ln-1])                        return -3; //Неправильная КС
        *len=ln+1;

/*
typedef enum {
        averaging_tacts_1 = 0x00,
	averaging_tacts_2 = 0x01,
	averaging_tacts_4 = 0x02,
	averaging_tacts_8 = 0x03,
	averaging_tacts_16= 0x04,
	averaging_tacts_32= 0x05
} ind3_averaging_tacts;
*/

        switch (buf[3])
        {
                case 0x0:
                        *tacts=1;
                break;

                case 0x01:
                        *tacts=2;
                break;

                case 0x02:
                        *tacts=4;
                break;

                case 0x03:
                        *tacts=8;
                break;

                case 0x04:
                        *tacts=16;
                break;

                case 0x05:
                        *tacts=32;
                break;


                default:
                        *tacts=1;
          }


        return 0;
}


//------------------------------------------------------------------------------
//Установить имя датчика ИН-Д3
int TProtocol_ASIN::MakeRequestIND3SetNameASIN(BYTE** buf, int* len, BYTE addr, char* name)
{
        //TODO: Add your source code here
        BYTE csum=0;
        lenCMD=0; int idx=0;
        bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START_211_1;          csum^=PACKET_START_211_1;
        bufTX[idx++]=ASIN_CMD_SET_NAME_IND3;         csum^=ASIN_CMD_SET_NAME_IND3;
        bufTX[idx++]=addr;                        csum^=addr;

        for (int i=0; i<12; i++)
        {
                if (name[i]=='\0') break;
                bufTX[idx++]=name[i]; csum^=name[i];
        }

        bufTX[idx++]=csum;
        lenCMD=idx-1;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD+1;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

//------------------------------------------------------------------------------
//Установить смещения датчика ИН-Д3
int TProtocol_ASIN::MakeRequestIND3SetOffsetASIN(BYTE** buf, int* len, BYTE addr, measresASIN res)
{
        //TODO: Add your source code here
        BYTE csum=0;
        lenCMD=0; int idx=0;
        bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START_211_1;          csum^=PACKET_START_211_1;
        bufTX[idx++]=ASIN_CMD_SET_OFFSET_IND3;         csum^=ASIN_CMD_SET_OFFSET_IND3;
        bufTX[idx++]=addr;                        csum^=addr;


        //фиксируем знак по X Y
        unsigned char sign_X=0;
        unsigned char sign_Y=0;

        double angle_X=res.angle_x;
        double angle_Y=res.angle_y;

        if (res.angle_x<0)
        {
                sign_X=1;
                angle_X*=-1;
        }

        if (res.angle_y<0)
        {
                sign_Y=1;
                angle_Y*=-1;
        }

        //целая часть
        double intpart=0;
        //дробная часть
        double fltpart=0;

        unsigned short tmp=0;

        //выделяем целую и дробную по Y
        fltpart=modf(angle_Y,&intpart);

        //дробную часть приводим к трем знакам после запятой и множим на 1000
        fltpart*=1000;
        //дробную часть приводим к 256 долям
        fltpart*=256;
        fltpart/=1000;

        //пакуем Y
        //D0.7-D0.0 - дробная часть ?_Y
        bufTX[idx]=fltpart;                                      csum^=bufTX[idx++];

        //D2.5-D1.0 - целая часть ?_Y
        //младший байт Y
        tmp=((unsigned short)intpart)&0xFF;
        bufTX[idx]=(unsigned char)tmp;                          csum^=bufTX[idx++];

        //старший байт Y
        tmp=(((unsigned short)intpart)>>8)&0xFF;
        bufTX[idx]=(unsigned char)tmp;                 //пока стоим на том же байте

        //пакуем размерность по Y
        if (res.unit_y)               bufTX[idx]|= (1<<6);
        else                          bufTX[idx]&=~(1<<6);

        //пакуем знак по Y
        if (sign_Y>0)                bufTX[idx]|= (1<<7);
        else                         bufTX[idx]&=~(1<<7);

        //теперь двигаем индекс
        csum^=bufTX[idx++];

        //выделяем целую и дробную по X
        fltpart=modf(angle_X,&intpart);

        //дробную часть приводим к трем знакампосле запятой и множим на 1000
        fltpart*=1000;
        //дробнуючасть приводим к 256 долям
        fltpart*=256;
        fltpart/=1000;

        //пакуем X
        //D3.7-D3.0 - дробная часть ?_X
        bufTX[idx]=fltpart;                            csum^=bufTX[idx++];

        //D5.5-D4.0 - целая часть ?_X

        //младший байт X
        tmp=((unsigned short)intpart)&0xFF;
        bufTX[idx]=(unsigned char)tmp;                 csum^=bufTX[idx++];

        //старшийбайт X
        tmp=(((unsigned short)intpart)>>8)&0xFF;
        bufTX[idx]=(unsigned char)tmp;                 //пока стоим на том же байте

        //пакуем размерность по X

        if (res.unit_x)               bufTX[idx]|= (1<<6);
        else                          bufTX[idx]&=~(1<<6);

        //пакуем знак по X

        if (sign_X>0)                bufTX[idx]|= (1<<7);
        else                         bufTX[idx]&=~(1<<7);

        csum^=bufTX[idx++];

        bufTX[idx++]=csum;

        lenCMD=idx-1;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD+1;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}


//------------------------------------------------------------------------------
//Установить такты усреднения датчика ИН-Д3
int TProtocol_ASIN::MakeRequestIND3SetTactsASIN(BYTE** buf, int* len, BYTE addr, WORD tacts)
{
        //TODO: Add your source code here
        BYTE csum=0;
        lenCMD=0; int idx=0;
        bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START_211_1;          csum^=PACKET_START_211_1;
        bufTX[idx++]=ASIN_CMD_SET_TACTS_IND3;     csum^=ASIN_CMD_SET_TACTS_IND3;
        bufTX[idx++]=addr;                        csum^=addr;

        BYTE t=0;

        switch (tacts)
        {
                case 1:
                          t=0;
                break;

                case 2:
                          t=1;
                break;

                case 4:
                          t=2;
                break;

                case 8:
                          t=3;
                break;

                case 16:
                          t=4;
                break;

                case 32:
                          t=5;
                break;

                default:
                         t=0;

        }

        csum^=t;
        bufTX[idx++]=t;
        bufTX[idx++]=csum;

        lenCMD=idx;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

//------------------------------------------------------------------------------
//Установить новый адрес датчика ИН-Д3
int TProtocol_ASIN::MakeRequestIND3SetAddrASIN(BYTE** buf, int* len, BYTE oldaddr, BYTE newaddr)
{
        //TODO: Add your source code here
        BYTE csum=0;
        lenCMD=0; int idx=0;
        bufTX[idx++]=PACKET_END;
        bufTX[idx++]=PACKET_START_211_1;              csum^=PACKET_START_211_1;
        bufTX[idx++]=ASIN_CMD_SET_ADDR_IND3;          csum^=ASIN_CMD_SET_ADDR_IND3;
        bufTX[idx++]=oldaddr;                         csum^=oldaddr;
        bufTX[idx++]=newaddr;                         csum^=newaddr;
        bufTX[idx++]=csum;

        lenCMD=idx;
        this->EscapeBytesEncode(&bufTX[1],&lenCMD);
        idx=lenCMD;
        bufTX[idx++]=PACKET_END;
        lenCMD=idx;
        *buf=bufTX;
        *len=lenCMD;
        return 0;
}

