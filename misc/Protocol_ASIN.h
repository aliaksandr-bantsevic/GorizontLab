//---------------------------------------------------------------------------
#include <stdint.h>
#include <vcl.h>

#ifndef Protocol_ASINH
#define Protocol_ASINH
//---------------------------------------------------------------------------
#endif

#define PACKETBUF_LEN 1024

#define PACKET_START_211        0x9B   //разделитель пакетов 211
#define PACKET_START_211_1      0x9C   //разделитель пакетов 211

#define PACKET_START 0x9A       //разделитель пакетов АСИН
#define PACKET_END   0x7E       //разделитель пакетов
#define ESCAPE_BYTE  0x7D       //escape байт
#define CODE7D_BYTE  0x5D       //0x7D код байт
#define CODE7E_BYTE  0x5E       //0x7E код байт


typedef struct {

  double angle_x;
  double angle_y;
  BYTE  unit_x;
  BYTE  unit_y;

} measresASIN;

typedef struct {

  BYTE pstart;
  BYTE cmd;
  BYTE* data;
  int lel;
  BYTE csum;
  BYTE pend;

} packetASIN;

typedef enum {

  ASIN_CMD_VERSION=124,
  ASIN_CMD_SLIST=123,
  ASIN_CMD_SETADDR=122,
  ASIN_CMD_MEAS=121,
  ASIN_CDM_MEASALL=120,
  ASIN_CMD_REVISION_ASIN=0x6D,

  ASIN_CDM_MEASALL_IND3=0x01,
  ASIN_CMD_VERSION_IND3=0x0E,
  ASIN_CMD_SERIAL_IND3=0x0B,
  ASIN_CMD_EDITION_IND3=0x0A,
  ASIN_CMD_NAME_IND3=0x03,
  ASIN_CMD_BAUD_IND3=0x01,
  ASIN_CMD_OFFSET_IND3=0x05,
  ASIN_CMD_TACTS_IND3=0x0C,

  ASIN_CMD_SET_NAME_IND3=0x04,
  ASIN_CMD_SET_OFFSET_IND3=0x06,
  ASIN_CMD_SET_TACTS_IND3=0x0D,
  ASIN_CMD_SET_ADDR_IND3=0x09

} cmdASIN;

typedef enum {

   ASIN_ERR_CSUM_REQUEST=1,
   ASIN_ERR_CMD=2,
   ASIN_ERR_NIT_RESPOND=3,
   ASIN_ERR_CSUM_RESPOND=4
} errASIN;

class TProtocol_ASIN
{
private:

        BYTE bufTX[1024];
        BYTE bufRX[1024];
        void Clear(void);
        int  lenCMD;
        
public:
        int EscapeBytesEncode(unsigned char* buf, int* len);
        int EscapeBytesDecode(unsigned char* buf, int* len);
        BYTE CheckSumCalculate(BYTE* buf, int len);

protected:

public:
         TProtocol_ASIN();
        ~TProtocol_ASIN();
        int MakeRequest(cmdASIN cmd,BYTE** buf,int* len);
        int PacketProc(BYTE* buf, int* len);
        int PacketProcVersion(BYTE* buf, char* ver);

        int MakeRequestNewAddr(cmdASIN cmd, BYTE** buf, int* len, BYTE oldaddr, BYTE newaddr);

        int MakeRequestVersionASIN(BYTE** buf, int* len);
        int PacketProcVersionASIN(BYTE* buf, int* len, char* ver);
        int MakeRequestRevisionASIN(BYTE** buf, int* len);
        int PacketProcRevisionASIN(BYTE* buf, int* len, unsigned int* rev);

        int PacketProcList(BYTE* buf, BYTE* list);

        //Запрос списка датчиков
        int MakeRequestSensorsList(BYTE** buf, int* len);
        int PacketProcSensorsList(BYTE* buf, int* len, BYTE* list);

        //Запросить показания датчика ИН-Д3
        int MakeRequestMeas211(BYTE** buf, int* len, BYTE ddr);
        //Распаковать показания датчика ИН-Д3
        int PacketProcMeas211(BYTE* buf, int* len, BYTE addr, measresASIN* res);

        //Запросить верию ПО датчика ИН-Д3
        int MakeRequestIND3VersionASIN(BYTE** buf, int* len, BYTE addr);
        //Распаковать пакет версия ПО датчика ИН-Д3
        int PacketProcIND3VersionASIN(BYTE* buf, int* len, BYTE addr, char* rev);


        //Запросить заводскй номер датчика ИН-Д3
        int MakeRequestIND3SerialASIN(BYTE** buf, int* len, BYTE addr);
        //Распаковать заводскй номер датчика ИН-Д3
        int PacketProcIND3SerialASIN(BYTE* buf, int* len, BYTE addr, unsigned int* sn);

        //Запросить редакция прошивки датчика ИН-Д3
        int MakeRequestIND3EditionASIN(BYTE** buf, int* len, BYTE addr);
        //Распаковать пакет редакция прошивки датчика ИН-Д3
        int PacketProcIND3EditionASIN(BYTE* buf, int* len, BYTE addr, unsigned short* ed);

        //Запросить имя датчика ИН-Д3
        int MakeRequestIND3NameASIN(BYTE** buf, int* len, BYTE addr);
        //Распаковать пакет имя датчика ИН-Д3
        int PacketProcIND3NameASIN(BYTE* buf, int* len, BYTE addr, char* name);

        //Запросить скрость датчика ИН-Д3
        int TProtocol_ASIN::MakeRequestIND3BaudASIN(BYTE** buf, int* len, BYTE addr);
        //Распаковать пакет скорость датчика ИН-Д3
        int TProtocol_ASIN::PacketProcIND3BaudASIN(BYTE* buf, int* len, BYTE addr, DWORD* baud);

        //Запросить смещения датчика ИН-Д3
        int TProtocol_ASIN::MakeRequestIND3OffsetASIN(BYTE** buf, int* len, BYTE addr);
        //Распаковать смещения датчика ИН-Д3
        int TProtocol_ASIN::PacketProcIND3OffsetASIN(BYTE* buf, int* len, BYTE addr, measresASIN* res);

        //Запросить такты усреднения датчика ИН-Д3
        int TProtocol_ASIN::MakeRequestIND3TactsASIN(BYTE** buf, int* len, BYTE addr);
        //Распаковать такты усреднения датчика ИН-Д3
        int TProtocol_ASIN::PacketProcIND3TactsASIN(BYTE* buf, int* len, BYTE addr, WORD* tacts);

        //Установить имя датчика ИН-Д3
        int TProtocol_ASIN::MakeRequestIND3SetNameASIN(BYTE** buf, int* len, BYTE addr, char* name);

        //Установить смещения датчика ИН-Д3
        int TProtocol_ASIN::MakeRequestIND3SetOffsetASIN(BYTE** buf, int* len, BYTE addr, measresASIN res);

        //Установить такты усреднения датчика ИН-Д3
        int TProtocol_ASIN::MakeRequestIND3SetTactsASIN(BYTE** buf, int* len, BYTE addr, WORD tacts);

        //Установить новый адрес датчика ИН-Д3
        int TProtocol_ASIN::MakeRequestIND3SetAddrASIN(BYTE** buf, int* len, BYTE oldaddr, BYTE newaddr);

};
