#define QUASI_TIME_SINCHRONIZE_MODE

// Sensor.cpp
//-----------------------------------------------------------------------------
#include "Sensor.h"
#include "math.h"


//-----------------------------------------------------------------------------

extern void InportSensorPermissions(unsigned short sn, unsigned short* prm);

extern TSNTPTime SNTPCoreTime;
extern int rint(double d);
extern int PPauseTick,PReOpen,PClose,TORCom,TOWCom,TORDat,TOWDat;
extern int AccFileTimeLen;      //Длина блок-файла по ускорениям                                                                                                                   
extern bool SaveAccTxt;
extern bool SaveAccBin;
extern char MainCurrentDirectory[1024];
extern bool SyncTimeOFF;
extern bool bGlobalRunFlag;
extern bool bGlobalSetZeroFlag;

extern int iGlobalMostProletLen;
extern int iGlobalMostSensorRang;


//extern void chart_data_view(void* buf, int size);
//extern void chart_data_view_Y(void* buf, int size);

double   ACC_CALC_KOEF =  -1.0/8.0/3600.0/180.0*M_PI*9.81347*1000.0; // Minsk

//-----------------------------------------------------------------------------
// TBTSensor
//-----------------------------------------------------------------------------
int TBTSensor::ReOpen(TCOMPort* P)
{
  if(P->st.res)    return 1;
  if(P->st.cmd==3) {er++;return 3;}      // Ошибка чтения
  if(P->st.cmd==7) {ec++;er=0;return 7;} // Ошибка CRC
  return 0;
}
//-----------------------------------------------------------------------------
int TBTSensor::GetTCur(TCOMPort* P)
{
//запрос температуры термодатчика
  int *i001=(int*)P->BS4I1_Cmd(adr,35,0,0,4,TOWCom,TORCom);
  if(i001==NULL) {}
  else TCur=*i001/1000.0;
  return ReOpen(P);
}
//-----------------------------------------------------------------------------

int TBTSensor::GetXYCur(TCOMPort* P)
{
//запрос значений углов
  double X,Y;
  BYTE *b=P->BS4I1_Cmd(adr,108,0,0,8,TOWCom,TORCom);
  if(b==NULL) {}
  else
  {
    X=(*(short*)b)/8.0;b+=2;
    Y=(*(short*)b)/8.0;b+=2;

    XCur_raw = X;
    YCur_raw = Y;

    d3r_tm=*(DWORD*)b;
    double anl=RotX*M_PI/180.0;
    if(InvY) Y=-Y;// else anl=-anl;
    // InvY==0 - вращение новой левой системы координат по часовой стрелке
    // InvY==1 - вращение новой правой системы координат против часовой стрелки
    XCur=+X*cos(anl)+Y*sin(anl)-X0;
    YCur=-X*sin(anl)+Y*cos(anl)-Y0;

    /*
                 if (((XCur != -31777)&&(abs(XCur_raw) > i_sensor_range)) || ((YCur_raw != -31777)&&(abs(YCur_raw) > i_sensor_range)))
                 {
                         b_out_of_range = true;
                 }
                 else
                 {
                         b_out_of_range = false;
                 }
     */

     isOutOfRange();

  }
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::GetThCur(TCOMPort* P)
{
//запрос показаний тахометра
  BYTE *b=P->BS4I1_Cmd(adr,134,0,0,2,TOWCom,TORCom);
  if(b==NULL) {}
  else ThCur=(*(WORD*)b)/8;
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::GetPwCur(TCOMPort* P)
{
//запрос мощности термостата
  BYTE *b=P->BS4I1_Cmd(adr,36,2,0,4,TOWCom,TORCom);
  if(b==NULL) {}
  else PwCur=(*(WORD*)b)/10.0;
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::ResErr(TCOMPort* P)
{
//запрос //???

  BYTE *b=P->BS4I1_Cmd(adr,202,0,0,10,TOWCom,TORCom);
  if(b==NULL) {}
  else res_err=*((BS4I1_NResErr*)b);
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::Getd3Stat(TCOMPort* P)
{
//запрос //???
  BYTE *b=P->BS4I1_Cmd(adr,87,0,0,10,TOWCom,TORCom);
  if(b==NULL) {}
  else d3stat=*((BS4I1_d3stat*)b);
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::Gett5Stat(TCOMPort* P)
{

//запрос //???
  BYTE *b=P->BS4I1_Cmd(adr,135,0,0,10,TOWCom,TORCom);
  if(b==NULL) {}
  else t5stat=*((BS4I1_t5stat*)b);
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::SetAdr(TCOMPort* P,BYTE a,BYTE c)
{

  if(!UCmd.bsadr) P->BS4I1_Cmd(adr,90,a,c,0,TOWCom,TORCom);
  else            P->BS4I1_Cmd(adr,222,a,0,0,TOWCom,TORCom); // 1.0.2
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::Setn50(TCOMPort* P,WORD n)
{

  P->BS4I1_Cmd(adr,204,n&0xFF,n/256,0,TOWCom,TORCom);
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::SetTmax(TCOMPort* P,char T)
{

  P->BS4I1_Cmd(adr,40,T,0,0,TOWCom,TORCom);
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::GetStp(TCOMPort* P,BYTE tstp)
{
  BYTE *b=P->BS4I1_Cmd(adr,221,tstp,0,sizeof(BS4I1_STP),TOWCom,TORCom);
  if(b==NULL) {}
  else stp=*((BS4I1_STP*)b);
  return ReOpen(P);                                                                                                                                                                 
}
//-----------------------------------------------------------------------------
int TBTSensor::WriteStp(TCOMPort* P)
{

  BYTE *b=P->BS4I1_Cmd(adr,220,66,99,2,TOWCom,TORCom);
  if(b==NULL) {}
  else {NVMCur=b[0];NVMSize=b[1];}
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::CopyStp(TCOMPort* P)
{

  P->BS4I1_Cmd(adr,225,0,0,0,TOWCom,TORCom);
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::NVMState(TCOMPort* P)
{

  BYTE *b=P->BS4I1_Cmd(adr,223,0,0,2,TOWCom,TORCom);
  if(b==NULL) {}
  else {NVMCur=b[0];NVMSize=b[1];}
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::SetTmp(TCOMPort* P)
{

  BYTE *b=P->BS4I1_Cmd(adr,224,B1,B2,0,TOWCom,TORCom);
  if(b==NULL) {}
  else {}
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::GetInfo(TCOMPort* P,int ix)
{
//запрос //???
  BYTE *b=P->BS4I1_Cmd(adr,36,ix,0,4,TOWCom,TORCom);
  if(b==NULL) {}
  else UCmd.info=*((DWORD*)b);
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::MODE(TCOMPort* P)
{

  P->BS4I1_Cmd(adr,50,UCmd.mode_com,UCmd.mode_on,0,TOWCom,TORCom);
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::ClrReset(TCOMPort* P)
{

  P->BS4I1_Cmd(adr,50,101,1,0,TOWCom,TORCom);
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::SetPulse(TCOMPort* P)
{

  BYTE *b=(byte*)&UCmd.tick_rate;
  if(P->BS4I1_Cmd(adr,91,b[0],b[1],0,TOWCom,TORCom)==0)       return ReOpen(P);
  if(P->BS4I1_Cmd(adr,92,b[2],b[3],0,TOWCom,TORCom)==0)       return ReOpen(P);
  b=(byte*)&UCmd.puls_time;
  if(P->BS4I1_Cmd(adr,93,b[0],b[1],0,TOWCom,TORCom)==0)       return ReOpen(P);
  if(P->BS4I1_Cmd(adr,94,b[2],b[3],0,TOWCom,TORCom)==0)       return ReOpen(P);
  if(P->BS4I1_Cmd(adr,95,UCmd.del_type,0,0,TOWCom,TORCom)==0) return ReOpen(P);
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int TBTSensor::Prog(TCOMPort* P)
{

  BYTE *b=P->BS4I1_Cmd(adr,99,66,99,0,TOWCom,TORCom);
  Sleep(50);
  b=P->BS4I1_Cmd(0x55,66,99,66,2,TOWCom,TORCom);
  if(b==NULL) {}
  else        {}
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::D3Set(TCOMPort* P)
{

  P->BS4I1_Cmd(adr,67,99,66,0,TOWCom,TORCom);
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::Reset(TCOMPort* P)
{

  P->BS4I1_Cmd(adr,99,66,99,0,TOWCom,TORCom);
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
//int TBTSensor::Reset(TCOMPort* P)  // !!! write flash
//{
//  P->BS4I1_Cmd(adr,236,66,99,0,TOWCom,TORCom);
//  return ReOpen(P);
//}
//-----------------------------------------------------------------------------
int TBTSensor::StackOn(TCOMPort* P,bool on,bool clr,WORD nGroup)
{

  byte b1=nGroup&0xFF,b2=on*128+clr*64+((nGroup/256)&0x3F);
  P->BS4I1_Cmd(adr,205,b1,b2,0,TOWCom,TORCom);
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::StackSOn(TCOMPort* P)
{

  P->BS4I1_Cmd(adr,207,0,0,0,TOWCom,TORCom);
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::StackOff(TCOMPort* P)
{

  P->BS4I1_Cmd(adr,206,0,0,0,TOWCom,TORCom);
  return ReOpen(P);
}
//****************************************************************************
// GetAll
//****************************************************************************
int TBTSensor::GetMWRes(TCOMPort* P)
{

  BYTE *b=P->BS4I1_Cmd(adr,137,0,0,5,TOWCom,TORCom);
  if(b==NULL) {}
  else mwres=*((BS4I1_wres*)b);
  return ReOpen(P);
}
//-----------------------------------------------------------------------------
int TBTSensor::GetMWStat(TCOMPort* P)
{

  BYTE *b=P->BS4I1_Cmd(adr,136,0,0,10,TOWCom,TORCom);
  double tm=Now();
  if(b==NULL) return ReOpen(P);
  mwstat=*((BS4I1_wstat*)b);
  SWBuf.res_pn=mwstat.sn;
  SWBuf.nmeas=SWBuf.res_pn/PACK_SIZE;
  Tm_Wpn=tm;Wpn_Tm=SWBuf.res_pn;
  return ReOpen(P);
}
//-----------------------------------------------------------------------------

//АН-Д3 формат float
int TBTSensor::GetAll(TCOMPort* P)
{

       if (this->sIND3sn=="Нет данных")
       {
                ReadSerialNumber(P);
       }

    double tm=Now();
    double X,Y=0;

//Для нового формата представления

  BYTE *b=P->BS4I1_Cmd(adr,201,0,0,18,TOWCom,TORCom);

  if (b==NULL)
  {
          b=P->BS4I1_Cmd(adr,201,0,0,18,TOWCom,TORCom);

          if(b==NULL)  {Sleep(20); b=P->BS4I1_Cmd(adr,201,0,0,18,TOWCom,TORCom);}

          if(b==NULL)  {Sleep(20); b=P->BS4I1_Cmd(adr,201,0,0,18,TOWCom,TORCom);}

          if(b==NULL) return ReOpen(P);
  }

  BS4I1_Res_float* res;
  res=(BS4I1_Res_float*)b;

extern bool b_temperature_sensors_active;

  if ((res->sw.ft5dr)&&( b_temperature_sensors_active == true))
  {
           TCur=(float)res->T/250.0-T0;
           if (isAccel) b_is_temperature_sensor = true;
  }
  else
  {
           TCur=-31777;
  }

  if (res->sw.fdr)
  {
          X=res->X;
          Y=res->Y;
  }
  else
  {
          X=-31777;
          Y=-31777;
  }

    XCur_raw = X;
    YCur_raw = Y;

  double anl=RotX*M_PI/180.0;

  if (this->isSVWG == false)
  {
          if(InvY) Y=-Y; // else anl=-anl;
          // InvY==0 - вращение новой левой системы координат по часовой стрелке
          // InvY==1 - вращение новой правой системы координат против часовой стрелки

          /*
                 if (((XCur != -31777)&&(abs(XCur_raw) > i_sensor_range)) || ((YCur != -31777)&&(abs(YCur_raw) > i_sensor_range)))
                 {
                         b_out_of_range = true;
                 }
                 else
                 {
                         b_out_of_range = false;
                 }
           */

          isOutOfRange(); 

          XCur=+X*cos(anl)+Y*sin(anl)-X0;
          YCur=-X*sin(anl)+Y*cos(anl)-Y0;

#ifdef ANGLE_TO_DEFLECTION_CONVERSION_MODE

        CalcBridgDeflection();

#endif

  }
  else
  {
          XCur = X; YCur = Y;

          TCur = YCur; sw.ft5dr = 1;

          if (Ung == 0)
          {
                XCur = XCur + ((Kt-Ktm)*(TCur - 24.f)) - D0;
          }
          else
          {
                double dcor = 0;
                dcor = XCur + ((Kt-Ktm)*(TCur - 24.f)) - D0;
                XCur = Ung*dcor;
                XCur/=1000.f;
          }

  }

  sw=res->sw;
  md=res->md;

  SBuf.res_pn=res->pn;
  SBuf.nmeas=SBuf.res_pn/PACK_SIZE;
  Tm_pn=tm;pn_Tm=SBuf.res_pn;if(sw.fdr) Tm_fdr=tm;

  NewDataFlag=true;

  return ReOpen(P);
}
//****************************************************************************
#define SAFETY_PACKS 8   // ??? предотвращение переполнения буфера за время чтения
//#define READ_PACKS  32 // для уменьшения времени опроса каждого датчика

//#define READ_PACKS   2 //16  // !!! GPRS для уменьшения времени опроса каждого датчика

extern int READ_PACKS;

#define   N_PACKS    1   // !!! GPRS считывать несколько пакетрв сразу READ_PACKS должно быть кратно N_PACKS
#define TIME_AVER   60   // время усреднения
#define MINP_AVER   10   // минимальное количество пакетов для усреднения (1 мин - 19)
//****************************************************************************

//int first_packet_request=1;

int TBTSensor::GetPackets(TCOMPort* P)
{

   if (first_packet_request>0)
   {
           StackOn(P,true,true,0);
           first_packet_request--;
           return 0;
   }

  int bufidx=0;

  int to_read=SBuf.nmeas-SBuf.nread-SBuf.nmiss;   //!!!По этой разнице знаем сколько есть новых пакетов для чтения

  if(to_read==0) return ReOpen(P); // Нет новых данных

  if(to_read<0)                    // Очистка буфера датчика или перезагрузка
  {
    to_read=SBuf.nmeas;
    SBuf.Clr();
    SBuf.nmeas=to_read;
  }

  if(to_read>=(NUM_PACKS-SAFETY_PACKS))       //!!!Это переполнение и надосбрасывать буфер
  {
    to_read=NUM_PACKS-SAFETY_PACKS;
    SBuf.nmiss=SBuf.nmeas-SBuf.nread-to_read;
    if((SBuf.nmiss%N_PACKS)>0) SBuf.nmiss=(SBuf.nmiss/N_PACKS)*N_PACKS+N_PACKS;
  }

  if(to_read>READ_PACKS) to_read=READ_PACKS;

  to_read=to_read-to_read%N_PACKS;



  /*
  for(int i=0;i<to_read/N_PACKS;i++)
  {
    BS4I1_D3Packet *pP=(BS4I1_D3Packet*)P->BS4I1_Cmd
      (adr,203,(SBuf.nread+SBuf.nmiss)%NUM_PACKS,N_PACKS,sizeof(BS4I1_D3Packet)*N_PACKS,TOWCom,TORCom);
    if(pP==NULL) break;
    for(int j=0;j<N_PACKS;j++)
    {
      SBuf.Packet_buf[SBuf.nread%SrsBufPSize]=*(pP+j);
      SBuf.nmeas_buf[SBuf.nread%SrsBufPSize]=SBuf.nread+1+SBuf.nmiss;
      SBuf.nread++;
    }
  }
  */


  for(int i=0;i<to_read/N_PACKS;i++)
  {
        BS4I1_D3Packet_float *pPf=(BS4I1_D3Packet_float*)P->BS4I1_Cmd(adr,203,(SBuf.nread+SBuf.nmiss)%NUM_PACKS,N_PACKS,sizeof(BS4I1_D3Packet_float)*N_PACKS,TOWCom,TORCom);

        if (pPf==NULL)
        {
                pPf=(BS4I1_D3Packet_float*)P->BS4I1_Cmd(adr,203,(SBuf.nread+SBuf.nmiss)%NUM_PACKS,N_PACKS,sizeof(BS4I1_D3Packet_float)*N_PACKS,TOWCom,TORCom);

                if (pPf==NULL) {Sleep(20); pPf=(BS4I1_D3Packet_float*)P->BS4I1_Cmd(adr,203,(SBuf.nread+SBuf.nmiss)%NUM_PACKS,N_PACKS,sizeof(BS4I1_D3Packet_float)*N_PACKS,TOWCom,TORCom);}

                if (pPf==NULL) {Sleep(20); pPf=(BS4I1_D3Packet_float*)P->BS4I1_Cmd(adr,203,(SBuf.nread+SBuf.nmiss)%NUM_PACKS,N_PACKS,sizeof(BS4I1_D3Packet_float)*N_PACKS,TOWCom,TORCom);}

                if (pPf==NULL)
                {
                        break;
                }
        }

//        if ((P->lerr!=403)&&(P->lerr!=2203))
//        {
//                ::Sleep(1);
//        }


        DWORD t1=pPf->stCT;
        DWORD t2=pPf->spCT;
        if (t1>t2) t2+=0x00010000;
        DWORD dt=t2-t1;
        double tpacket=(double)dt/double(1000000000.);
        tpacket*=25.;

        if (tpacket==3.2)
        {
                this->acc_step_per_sec=10;
                this->SBuf.acc_step_per_sec=10;
        }

        if (tpacket==0.64)
        {
                this->acc_step_per_sec=50;
                this->SBuf.acc_step_per_sec=50;
        }


        if(pPf==NULL) break;

        for(int j=0;j<N_PACKS;j++)
        {
                SBuf.Packet_buf_float[SBuf.nread%SrsBufPSize]=*(pPf+j);
                SBuf.nmeas_buf[SBuf.nread%SrsBufPSize]=SBuf.nread+1+SBuf.nmiss;
                SBuf.nread++;
        }
  }


  //if(to_read>=(NUM_PACKS-SAFETY_PACKS))
  //{
  //  to_read=NUM_PACKS-SAFETY_PACKS;
  //  SBuf.nmiss=SBuf.nmeas-SBuf.nread-to_read;
  //}
  //if(to_read>READ_PACKS) to_read=READ_PACKS;
  //for(int i=0;i<to_read;i++)
  //{
  //  BS4I1_D3Packet *pP=(BS4I1_D3Packet*)P->BS4I1_Cmd(adr,203,(SBuf.nread+SBuf.nmiss)%NUM_PACKS,0,sizeof(BS4I1_D3Packet),TOWCom,TORCom);
  //  if(pP==NULL) break;
  //  SBuf.Packet_buf[SBuf.nread%SrsBufPSize]=*pP;
  //  SBuf.nmeas_buf[SBuf.nread%SrsBufPSize]=SBuf.nread+1+SBuf.nmiss;
  //  SBuf.nread++;
  //}

  return ReOpen(P);
}
//****************************************************************************
int TBTSensor::GetWPackets(TCOMPort* P)
{

  int to_read=SWBuf.nmeas-SWBuf.nread-SWBuf.nmiss;
  if(to_read==0) return ReOpen(P); // Нет новых данных
  if(to_read<0)                    // Очистка буфера датчика или перезагрузка
  {
    to_read=SWBuf.nmeas;
    SWBuf.Clr();
    SWBuf.nmeas=to_read;
  }
  if(to_read>=(NUM_PACKS-SAFETY_PACKS))
  {
    to_read=NUM_PACKS-SAFETY_PACKS;
    SWBuf.nmiss=SWBuf.nmeas-SWBuf.nread-to_read;
  }
  if(to_read>READ_PACKS) to_read=READ_PACKS;
  for(int i=0;i<to_read;i++)
  {
    BS4I1_WPacket *pP=(BS4I1_WPacket*)P->BS4I1_Cmd(adr,213,(SWBuf.nread+SWBuf.nmiss)%NUM_PACKS,0,sizeof(BS4I1_WPacket),TOWCom,TORCom);
    if(pP==NULL) break;
    SWBuf.Packet_buf[SWBuf.nread%SrsBufPSize]=*pP;
    SWBuf.nmeas_buf[SWBuf.nread%SrsBufPSize]=SWBuf.nread+1+SWBuf.nmiss;
    SWBuf.nread++;
  }
  return ReOpen(P);
}
//****************************************************************************
//****************************************************************************
//****************************************************************************
// CalcPacket
//****************************************************************************
//-----------------------------------------------------------------------------------------------
BYTE ValPacket(BS4I1_D3Packet* P)
{
  BYTE s=0;
  if(P->drz<324)  s|=1<<0; // заполнение FIR1 c ошибками 324 - n50==50 300 - n50==46
  if(P->ne>0)     s|=1<<1; // ошибки при заполнении
  if(P->AX>=32767)s|=1<<2; // FIR2 не заполнен (!sw.fdr)
  return s;
  //||(SBP[i%MAXP].dCT*25.0*1e-6/PACK_SIZE>100.1)
  //||(SBP[i%MAXP].dCT*25.0*1e-6/PACK_SIZE<99.9)
  //||(SBP[i%MAXP].dCT*25.0*1e-6/PACK_SIZE>2.176)
  //||(SBP[i%MAXP].dCT*25.0*1e-6/PACK_SIZE<2.174)
}

BYTE ValPacket_float(BS4I1_D3Packet_float* P)
{
  BYTE s=0;
  if(P->drz<324)  s|=1<<0; // заполнение FIR1 c ошибками 324 - n50==50 300 - n50==46
  if(P->ne>0)     s|=1<<1; // ошибки при заполнении
  if(P->AX>=32767)s|=1<<2; // FIR2 не заполнен (!sw.fdr)
  return s;
  //||(SBP[i%MAXP].dCT*25.0*1e-6/PACK_SIZE>100.1)
  //||(SBP[i%MAXP].dCT*25.0*1e-6/PACK_SIZE<99.9)
  //||(SBP[i%MAXP].dCT*25.0*1e-6/PACK_SIZE>2.176)
  //||(SBP[i%MAXP].dCT*25.0*1e-6/PACK_SIZE<2.174)
}


//-----------------------------------------------------------------------------------------------
void TBTSensor::CalcPacket(void)
{

//Преобразование в инт с округлкнием
  extern int rint(double d);

//Указатель на хранилище пакетов в стеке
  BS4I1_D3Packet_float* SBPf=SBuf.Packet_buf_float;

//Перебираем все необсчитанные пакеты
  for(int i=SBuf.ncalc;i<SBuf.nread;i++,SBuf.ncalc++)
  {

//Движение по кругу - индекс кратный размеру хранилища пакетов
    int ix=i%SrsBufPSize;

//pn_Tm=SBuf.res_pn присвоилось при получении пакета   DWORD res_pn;// Количество измерений GetAll
//SBuf.nmeas_buf[SBuf.nread%SrsBufPSize]=SBuf.nread+1+SBuf.nmiss;


    //-----------------------------------------------------------------------------------------------
    // Время конца пакета с коррекция 0.1 с на иззмерение // ??? 0.25 с при таймауте не учтено
    double tmBS=(pn_Tm-SBuf.nmeas_buf[ix]*PACK_SIZE)*0.1/3600.0/24.0; // Коррекция 0.1 с на измерение

    //
    SBuf.Time_buf[ix]=Tm_pn-tmBS;
    //-----------------------------------------------------------------------------------------------
    // Запись в линейный массив X,Y
    for(int j=0;j<PACK_SIZE;j++)
    {
      SBuf.Xf[ix*PACK_SIZE+j]=SBPf[ix].x[j];
      SBuf.Yf[ix*PACK_SIZE+j]=SBPf[ix].y[j];
    }
    //-----------------------------------------------------------------------------------------------
    // Validate packet
    BYTE s=ValPacket_float(SBPf+ix);
    SBuf.s[ix]=s;

    if(s>0)
    {
      SBuf.ecount++;
      SBuf.XAver[ix]=0.0;SBuf.YAver[ix]=0.0;
      SBuf.XMax[ix]= 0.0;SBuf.YMax[ix]= 0.0;
      SBuf.XRms[ix]= 0.0;SBuf.YRms[ix]= 0.0; SBuf.XYRms[ix]=0.0;
      continue;
    }
    //-----------------------------------------------------------------------------------------------

    // обработка- вычисление ускорений

    double k=1;

    double AX=SBPf[ix].AX,AY=SBPf[ix].AY; // FIR 2, если = 32767 - фильтр не заполнен

    double X,Y,XAver=0,YAver=0,Max=0,XMax=0,YMax=0,XRms=0,YRms=0,XYRms=0;
    //-----------------------------------------------------------------------------------------------
    for(int j=0;j<PACK_SIZE;j++)
    {
      //-------------------------------------------------------------------------
      X=SBuf.Xf[ix*PACK_SIZE+j]*k;
      Y=SBuf.Yf[ix*PACK_SIZE+j]*k;
      //-------------------------------------------------------------------------
      if(md.noacmp) {X-=AX*k;Y-=AY*k;} // >>> компенсация 0
      //-------------------------------------------------------------------------
      double anl=RotX*M_PI/180.0,x=X,y=Y; // вращение
      if(InvY) Y=-Y; // else anl=-anl;
      //  InvY==0 - вращение новой левой системы координат по часовой стрелке
      //  InvY==1 - вращение новой правой системы координат против часовой стрелки
      X=+x*cos(anl)+y*sin(anl);
      Y=-x*sin(anl)+y*cos(anl);
      //-------------------------------------------------------------------------
      XAver+=X;YAver+=Y;
      XRms+=X*X;YRms+=Y*Y;XYRms+=X*Y;
      if(sqrt(X*X+Y*Y)>=Max) {Max=sqrt(X*X+Y*Y);XMax=X;YMax=Y;}
    }

    SBuf.XAver[ix]=XAver/PACK_SIZE;
    SBuf.YAver[ix]=YAver/PACK_SIZE;
    SBuf.XMax[ix]=XMax;
    SBuf.YMax[ix]=YMax;
    SBuf.XRms[ix]=sqrt(XRms/PACK_SIZE);
    SBuf.YRms[ix]=sqrt(YRms/PACK_SIZE);
    SBuf.XYRms[ix]=XYRms/PACK_SIZE/(SBuf.XRms[ix]+1e-30)/(SBuf.YRms[ix]+1e-30);

  }


}
//-----------------------------------------------------------------------------------------------
double GetAngle(double x,double y,bool is360)
{
  double a;
  x=fabs(x)<1e-30?1e-30:x;
  a=atan(y/x);if(x<0.0) a+=(y>=0.0)?M_PI:-M_PI;
  a*=180.0/M_PI;
  if(is360) if(a<0.0) a+=360.0;
  return a;
}
//-----------------------------------------------------------------------------------------------
void TBTSensor::Calc_FA(void)
{
  double time=Now(),tm1s=1.0/24.0/3600.0,XAver=0,YAver=0,Max=0,XMax=0,YMax=0,XRms=0,YRms=0,XYRms=0;
  //----------------------------------------------------------------------------
  int n=0;
  for(int i=SBuf.ncalc-1;i>=0;i--)
  {
    int ix=i%SrsBufPSize;
    if((time-SBuf.Time_buf[ix])>tm1s*TIME_AVER) break;
    n++;
  }                                         
  if(n==0) {Adataon=false;return;}
  //----------------------------------------------------------------------------
  int N=n;n=0;
  for(int i=0;i<N;i++)
  {
    int ix=(SBuf.ncalc-N+i)%SrsBufPSize;
    if(SBuf.s[ix]>0) continue;
    n++;
    XAver+=SBuf.XAver[ix];YAver+=SBuf.YAver[ix];
    double X=SBuf.XMax[ix],Y=SBuf.YMax[ix],XY;
    if(sqrt(X*X+Y*Y)>=Max) {Max=sqrt(X*X+Y*Y);XMax=X;YMax=Y;}
    X=SBuf.XRms[ix];Y=SBuf.YRms[ix];XY=SBuf.XYRms[ix];
    XRms+=X*X;YRms+=Y*Y;XYRms+=XY*X*Y;
  }
  if(n<MINP_AVER) { Adataon=false;return;}
  //----------------------------------------------------------------------------
  Adataon=true;
  //----------------------------------------------------------------------------
  AXAver=XAver/n;AYAver=YAver/n;
  //----------------------------------------------------------------------------
  AXMax=XMax;AYMax=YMax;
  AMaxAbs=sqrt(XMax*XMax+YMax*YMax);
  AMaxAng=GetAngle(XMax,YMax,true);
  //----------------------------------------------------------------------------
  AXRms=sqrt(XRms/n);AYRms=sqrt(YRms/n);
  AXYRms=XYRms/n/(AXRms+1e-30)/(AYRms+1e-30);
  //----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------------------------
//****************************************************************************
// CalcWPacket
//****************************************************************************
bool Val(BYTE s) {return !(((s<11)&&(s>0))||(s>=128));}
//----------------------------------------------------------------------------
void TBTSensor::CalcWPacket(void)
{
  BS4I1_WPacket* SBP=SWBuf.Packet_buf;
  for(int i=SWBuf.ncalc;i<SWBuf.nread;i++,SWBuf.ncalc++)
  {
    int ix=i%SrsBufPSize;
    //-----------------------------------------------------------------------------------------------
    // Время конца пакета с коррекция 0.1 с на измерение // ??? 0.15 с при таймауте не учтено
    double tmBS=(Wpn_Tm-SWBuf.nmeas_buf[ix]*PACK_SIZE)*0.1/3600.0/24.0;
    SWBuf.Time_buf[ix]=Tm_Wpn-tmBS;
    //-----------------------------------------------------------------------------------------------
    // Запись в линейный массив U,V
    int nerr=0;        // Количество ошибок пакета
    BYTE serr,snoerr;  // Последний статус с ошибкой, без ошибки
    double U,V,UAver=0,VAver=0,Max=0,UMax=0,VMax=0;
    for(int j=0;j<PACK_SIZE;j++)
    {
      extern int rint(double d);
      //------------------------------------------------------------------------
      // WTest
      //U=SWBuf.U[ix*PACK_SIZE+j]=rint(1000+1000*sin(2.0*M_PI*1.23456/10.0*(ix*PACK_SIZE+j)));
      //V=SWBuf.V[ix*PACK_SIZE+j]=rint(rand()%2001);
      //BYTE s=60;
      //------------------------------------------------------------------------
      U=SWBuf.U[ix*PACK_SIZE+j]=SBP[ix].u[j];
      V=SWBuf.V[ix*PACK_SIZE+j]=SBP[ix].v[j];
      BYTE s=SBP[ix].s[j];
      //------------------------------------------------------------------------
      extern bool isCB128;if(isCB128) s=128;
      //------------------------------------------------------------------------
      if(!Val(s)) {nerr++;serr=s;continue;}
      //------------------------------------------------------------------------
      snoerr=s;
      UAver+=U;VAver+=V;
      if(sqrt(U*U+V*V)>=Max) {Max=sqrt(U*U+V*V);UMax=U;VMax=V;}
      //------------------------------------------------------------------------
    }
    // ??? Допустимое количество ошибок
    if(nerr<=PACK_SIZE/2)
    {
      //------------------------------------------------------------------------
      SWBuf.UAver[ix]=UAver/(PACK_SIZE-nerr);SWBuf.VAver[ix]=VAver/(PACK_SIZE-nerr);
      SWBuf.UMax[ix]=UMax;SWBuf.VMax[ix]=VMax;
      SWBuf.s[ix]=snoerr;
      //------------------------------------------------------------------------
    }
    else
    {
      //------------------------------------------------------------------------
      SWBuf.UAver[ix]=0.0;SWBuf.VAver[ix]=0.0;
      SWBuf.UMax[ix]=0.0; SWBuf.VMax[ix]=0.0;
      SWBuf.URms[ix]=0.0; SWBuf.VRms[ix]=0.0;
      SWBuf.s[ix]=serr;
      SWBuf.ecount++;
      //------------------------------------------------------------------------
    }
  }
  //CalcW();
}
void TBTSensor::Calc_FW(void)
{
  double time=Now(),tm1s=1.0/24.0/3600.0,UAver=0,VAver=0,Max=0,UMax=0,VMax=0;
  //----------------------------------------------------------------------------
  int n=0;
  for(int i=SWBuf.ncalc-1;i>=0;i--)
  {
    int ix=i%SrsBufPSize;
    if((time-SWBuf.Time_buf[ix])>tm1s*TIME_AVER) break;
    n++;
  }
  if(n==0) {wstatus=0;Wdataon=false;return;}
  //----------------------------------------------------------------------------
  int N=n;n=0;
  for(int i=0;i<N;i++)
  {
    int ix=(SWBuf.ncalc-N+i)%SrsBufPSize;
    wstatus=SWBuf.s[ix];if(!Val(wstatus)) continue;
    //-----------------------------------------------
    UAver+=SWBuf.UAver[ix];VAver+=SWBuf.VAver[ix];
    double U=SWBuf.UMax[ix],V=SWBuf.VMax[ix];
    if(sqrt(U*U+V*V)>=Max) {Max=sqrt(U*U+V*V);UMax=U;VMax=V;}
    n++;
    //-----------------------------------------------
  }
  if(n<MINP_AVER) { Wdataon=false;return;}
  //-----------------------------------------------
  WUAver=UAver/n;WVAver=VAver/n;
  WUMax=UMax;WVMax=VMax;
  Wdataon=true;
  //----------------------------------------------------------------------------
  // RMS
  //----------------------------------------------------------------------------
  n=0;double UR=0.0,VR=0.0;
  for(int i=0;i<N;i++)
  {
    int ix=(SWBuf.ncalc-N+i)%SrsBufPSize;if(!Val(SWBuf.s[ix])) continue;
    double U,V,URms=0.0,VRms=0.0;
    int m=0;
    for(int j=0;j<PACK_SIZE;j++)
    {
      if(!Val(SWBuf.Packet_buf[ix].s[j])) continue;
      //if(!Val(60)) continue; // WTest
      U=SWBuf.U[ix*PACK_SIZE+j]-WUAver;V=SWBuf.V[ix*PACK_SIZE+j]-WVAver;
      URms+=U*U;VRms+=V*V;
      m++;
    }
    U=SWBuf.URms[ix]=sqrt(URms/m);V=SWBuf.VRms[ix]=sqrt(VRms/m);
    UR+=U*U;VR+=V*V;
    n++;
  }
  WURms=sqrt(UR/n);WVRms=sqrt(VR/n);
}
//****************************************************************************
// TSBuf
//****************************************************************************
void TSBuf::Clr(void)
{

//  return;//!!!

  AccFilesWriteTimeCnt=0;
  startsave=false;

  nread=0;
  nmiss=0;
  ncalc=0;
  ecount=0;
  nsave=0;
  res_pn=0;
  nmeas=0;
  sw=0;

  if(fx!=NULL)
  {
        fclose(fx);
        fx=NULL;
  }

  if(fy!=NULL)
  {
        fclose(fy);
        fy=NULL;
  }


  last_acc_data.t_start=0;
  last_acc_data.t_end=0;
  for (int i=0; i<32*(PACKETS_TO_SAVE); i++)
  {
      last_acc_data.acc_x[i]=-31777;
      last_acc_data.acc_y[i]=-31777;
  }


}
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// Sync SaveBuf
//-----------------------------------------------------------------------------

extern TSNTPTime SNTPCoreTime;
extern int rint(double d);

bool TSBuf::SaveBuf_spectrum(AnsiString dirname,int srs)
{

  AnsiString s,smax,smin,ssrs=srs,xname,yname,dir;

  if(!on) return true;
  if(nsave>=ncalc) return true;
  if(nsave%(SrsBufToSave*SrsBufPSize)==0) // Ограничение размера файла
  {
    BS4I1_D3Packet* P=Packet_buf;
    DWORD tmpCT=P->tmpCT;if(P->stCT>P->spCT) tmpCT--;
    startCT=(P->stCT+tmpCT*65536.0*65536.0)*25.0*1e-9;
    startClock=Clock.GetClock(startCT);
    TimeStart=SNTPCoreTime.GetTime(startClock);
    if(TimeStart==0.0) return true; // нет синхронизации
    if(nsave>0) TimeStart=TimeStop;
    dir=dirname;
    dir+="\\Spectrum";
    if(!DirectoryExists(dir)) if(!CreateDir(dir)) return false;
    dir+="\\AData";
    if(!DirectoryExists(dir)) if(!CreateDir(dir)) return false;
    xyname=dir+"\\"+FormatDateTime("yy_mm_dd_hh_nn_ss_zzz_",TimeStart);
    tmp_xname=xyname+ssrs+"X.tmp";
    tmp_yname=xyname+ssrs+"Y.tmp";
  }
  fx=fopen(tmp_xname.c_str(),"ab");if(fx==NULL) {           return false;}
  fy=fopen(tmp_yname.c_str(),"ab");if(fy==NULL) {fclose(fx);return false;}
  bool FileEnd=false;
  for(;nsave<ncalc;nsave++)
  {
    if(fwrite(Xf+(nsave%SrsBufPSize)*PACK_SIZE,sizeof(float),PACK_SIZE,fx)!=PACK_SIZE) {fclose(fx);fclose(fy);return false;}
    if(fwrite(Yf+(nsave%SrsBufPSize)*PACK_SIZE,sizeof(float),PACK_SIZE,fy)!=PACK_SIZE) {fclose(fx);fclose(fy);return false;}
    if((nsave+1)%(SrsBufToSave*SrsBufPSize)==0)
    {
     FileEnd=true;
     nsave++;
     break;
    }
  }
  fclose(fx);fclose(fy);
  if(FileEnd)
  {
    BS4I1_D3Packet_float* P=Packet_buf_float+(nsave-1)%SrsBufPSize;
    stopCT=(P->spCT+P->tmpCT*65536.0*65536.0)*25.0*1e-9;
    stopClock=Clock.GetClock(stopCT);
    TimeStop=SNTPCoreTime.GetTime(stopClock);

    int dCT=rint((stopCT-startCT)*1000.0);
    int dClock=rint((stopClock-startClock)*1000.0);
    int dt=rint((TimeStop-TimeStart)*24.0*3600.0*1000.0);

    s.printf("%d_",dt);
    smin.printf("%.3f",Clock.LatMin);
    smax.printf("%.3f",Clock.LatMax);
    xname=xyname+s+ssrs;yname=xyname+s+ssrs;

    if(FileEnd)
    {
      xname+="X.wrd";yname+="Y.wrd";

      //if(FMain->CBMemo->Checked)
      //{
      //  s=(s=nsave)+"\t"+(s=dCT)+"\t"+(s=dClock)+"\t"+(s=dt)+"\t"+smin+"\t"+smax;
      //  if(srs%2) FMain->Memo1Sync->Lines->Add(s);
      //  else      FMain->Memo2Sync->Lines->Add(s);
      //}

    }
    else {xname+="X.tmp";yname+="Y.tmp";}

    RenameFile(tmp_xname,xname);tmp_xname=xname;
    RenameFile(tmp_yname,yname);tmp_yname=yname;

  }
  return true;
}

//------------------------------------------------------------------------------
//TDateTime tp=0;
#define T_ONE_MSEC      ((1./(24.*60.*60.))/1000)

/*
DWORD dwst[64];
AnsiString st[64];
int i_dw_idx=0;
int packet_buf_float_idx=0;
*/

//bool startsave=false;


int       testtimer=0;
bool TSBuf::SaveBuf(AnsiString dirname,int srs, bool lic)
{

  AnsiString s,name=dirname;

  if(!on)
  {
   return true;
  }


  if(nsave>=ncalc)
  {
   return true;
  }

        if ((startsave==false) || (bGlobalRunFlag == false)) this->AccFilesReopenFlag=true;

        if (!this->AccFilesReopen(nsave,0,lic)) return false;

        startsave=true;


  int i_fwrite_cnt;

//  int acc_records_num=(AccFileTimeLen*T_ONE_MIN)/(T_ONE_SEC/this->acc_step_per_sec);

          //Временно
  int acc_records_num=(AccFileTimeLen*T_ONE_SEC)/(T_ONE_SEC/this->acc_step_per_sec);

  //!!!Это для нарезки по длине количество записей
  //!!!acc_records_num=4096*SrsBufToSave;

//  i_dw_idx=0;
//  packet_buf_float_idx=0;
//  int initnsave=-1;

  for(;nsave<ncalc;nsave++)
  {


      /*
      if (start_sens_tick_cnt==0)
      {
              BS4I1_D3Packet_float* P=Packet_buf_float;
              start_sens_tick_cnt=P->tmpCT;
              start_sens_tick_cnt*=65536;
              start_sens_tick_cnt*=65536;
              start_sens_tick_cnt+=(unsigned long long)P->stCT;
      }
      */


//В бинарный файл
    //if (SaveAccBin)
    {

            TDateTime tstart=0.0;

            //BS4I1_D3Packet_float* P=&Packet_buf_float[packet_buf_float_idx++];

//            BS4I1_D3Packet_float* P=&Packet_buf_float[nsave];

              BS4I1_D3Packet_float* P=&Packet_buf_float[nsave%SrsBufPSize];



//            dwst[i_dw_idx]=P->stCT;

            DWORD tmpCT=P->tmpCT;if(P->stCT>P->spCT) tmpCT--;
            startCT=(P->stCT+tmpCT*65536.0*65536.0)*25.0*1e-9;
            startClock=Clock.GetClock(startCT);
            tstart=SNTPCoreTime.GetTime(startClock);

//            st[i_dw_idx++]=FormatDateTime("yy_mm_dd_hh_nn_ss_zzz \r\n",tstart);


            /*
            //s.printf("%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f ",tstart,P->x[0],P->x[1],P->x[2],P->x[3],P->x[4],P->x[5],P->x[6],P->x[7],P->x[8],P->x[9]);
            s="";
            s+=FormatDateTime("yy_mm_dd_hh_nn_ss_zzz ",tstart);
            s+=initnsave;
            s+=" \r\n";

            //s.printf("%08X \r\n",P->stCT);

            FILE* fs=fopen("E:\\serv.txt","ab");

            if (fs)
            {
               fwrite(s.c_str(),strlen(s.c_str()),1,fs);
               fclose(fs);
            }
            */

            if (i_packets_saved==0)
            {
                    if (TimeStart_begin==0) TimeStart_begin=tstart;
            }
            i_packets_saved++;

            AttachAccData_float((float*)(Xf+(nsave%SrsBufPSize)*PACK_SIZE),(float*)(Yf+(nsave%SrsBufPSize)*PACK_SIZE),PACK_SIZE,tstart);
            ::Sleep(50);

//Ускорения X
//Указатель на начало сохраняемых данных
            float* fdata_x=Xf+(nsave%SrsBufPSize)*PACK_SIZE;
            float* fdata_y=Yf+(nsave%SrsBufPSize)*PACK_SIZE;

//Скольковсего данных сохранять
            int i_fdata_to_save_cnt=PACK_SIZE;
//Сохраняем по одной записи
            for (int i=0; i<i_fdata_to_save_cnt; i++)
            {

               if (SaveAccBin)
               {

                      
              testtimer++;


                  if ((lic == true) && (bGlobalRunFlag == true))
                  {
                       if (!(fwrite(fdata_x,sizeof(float),1,fx)))
                       {
                                fclose(fx);
                                fclose(fy);
                                return false;
                       }

                       if (!(fwrite(fdata_y,sizeof(float),1,fy)))
                       {
                                fclose(fx);
                                fclose(fy);
                                return false;
                       }
                  }     

               }

               if ((lic == true) && (bGlobalRunFlag == true))
               {
                       if (SaveAccTxt)
                       {

                                char c_par[64];
                                sprintf (c_par,"%.2f\r\n",*fdata_x);
                                fwrite(c_par,strlen(c_par),1,fxt);

                                sprintf (c_par,"%.2f\r\n",*fdata_y);
                                fwrite(c_par,strlen(c_par),1,fyt);

                       }
               }

               fdata_x++;
               fdata_y++;

               //AccFilesWriteTimeCnt+=T_ONE_SEC/this->acc_step_per_sec;

               AccFilesWriteTimeCnt++;

               //if (AccFilesWriteTimeCnt>=AccFileTimeLen*T_ONE_MIN)
               if ((AccFilesWriteTimeCnt>=acc_records_num) || (bGlobalRunFlag == false))
               {
                     this->AccFilesReopenFlag=true;
                     if (!this->AccFilesReopen(nsave,i,lic)) return false;
               }

            }

            /*
            TDateTime tstart=0.0;
            BS4I1_D3Packet_float* P=Packet_buf_float;
            DWORD tmpCT=P->tmpCT;if(P->stCT>P->spCT) tmpCT--;
            startCT=(P->stCT+tmpCT*65536.0*65536.0)*25.0*1e-9;
            startClock=Clock.GetClock(startCT);
            tstart=SNTPCoreTime.GetTime(startClock);
            TimeStart_begin=tstart;
            AttachAccData_float((float*)(Xf+(nsave%SrsBufPSize)*PACK_SIZE),(float*)(Yf+(nsave%SrsBufPSize)*PACK_SIZE),PACK_SIZE,tstart);
            */

    }


    if((nsave+1)%(SrsBufToSave*SrsBufPSize)==0)
    {
        nsave++;
        break;
    }

  }

  fclose(fx);
  fclose(fy);

  if (fxt!=NULL)
        fclose(fxt);
  if (fyt!=NULL)
        fclose(fyt);

//  initnsave=-1;

  return true;


}
//****************************************************************************
// TSWBuf
//****************************************************************************
void TSWBuf::Clr(void)
{
//    return;//!!!

  nread=0;nmiss=0;ncalc=0;ecount=0;nsave=0;res_pn=0;nmeas=0;
  if(fx!=NULL)
   {
         fclose(fx);
         fx=NULL;
   }

   if(fy!=NULL)
   {
         fclose(fy);
         fy=NULL;
   }

}
//-----------------------------------------------------------------------------
bool TSWBuf::SaveBuf(AnsiString dirname,int srs)
{
  AnsiString s,name=dirname;
  if(!on) return true;
  if(nsave>=ncalc) return true;
  if(nsave%(SrsBufToSave*SrsBufPSize)==0) // Ограничение размера файла
  {
    name+="\\History\\Bin\\";
    if(!DirectoryExists(name)) if(!CreateDir(name)) return false;
    name+="\\";
    name+=FormatDateTime("yy_mm_dd_hh_nn_ss_",Now());
    name+=(s=srs);
    xname=name+"U.wrd";
    yname=name+"V.wrd";
  }
  fx=fopen(xname.c_str(),"ab");
  if(fx==NULL)
  {
             return false;
  }

  fy=fopen(yname.c_str(),"ab");
  if(fy==NULL)
  {
        fclose(fx);
        return false;
  }

  for(;nsave<ncalc;nsave++)
  {
    if(fwrite(U+(nsave%SrsBufPSize)*PACK_SIZE,sizeof(short),PACK_SIZE,fx)!=PACK_SIZE) {fclose(fx);fclose(fy);return false;}
    if(fwrite(V+(nsave%SrsBufPSize)*PACK_SIZE,sizeof(short),PACK_SIZE,fy)!=PACK_SIZE) {fclose(fx);fclose(fy);return false;}
    if((nsave+1)%(SrsBufToSave*SrsBufPSize)==0) {nsave++;break;}
  }

  fclose(fx);
  fclose(fy);

  return true;
}

//АН-Д3
int TBTSensor::ExecComs(TCOMPort* CPrt)
{

  int res=1;
  //*******************************************************************************************************************************
  // Команды таймера
  //*******************************************************************************************************************************
  //CalcFA();CalcFW();
  //-------------------------------------------------------------------------------------------------------------------------------


    if (this->AccelOn == true)
    {
            if(UCmd.cs.StackOff)
            {
                    res=StackOn(CPrt,false,UCmd.SClrOff,UCmd.nGroup);if(res!=1) return res;UCmd.cs.StackOff=false;
                    SBuf.on=false;
                    if(UCmd.SProgClrOff) SBuf.Clr();
             }

            if(UCmd.cs.StackOn)
            {
                    res=StackOn(CPrt,true,UCmd.SClrOn,UCmd.nGroup);if(res!=1) return res;UCmd.cs.StackOn=false;
                    if(UCmd.SProgClrOn) SBuf.Clr();
                    SBuf.on=true;
                    StackStartTime=Now();
            }
    }


  //------------------------------------------------------------------------------------------------------------------------------
  if(UCmd.cs.GetAll)
  {
    res=GetAll(CPrt);
    if(res!=1) return res; UCmd.cs.GetAll=false;
    if(isAccel) if(!md.nostack) SBuf.on=true;

    if (this->isSVWG == false)
    {
            if(SBuf.on)
            if (this->isAccel)
            if (this->AccelOn)
            {
              res=GetPackets(CPrt);
              CalcPacket(); // часть пакетов может быть прочитана
              if(res!=1) return res;
            }

            if(sw.frst==0) { SBuf.Clock.isSyncOn=false;res=ClrReset(CPrt);if(res!=1) return res;}     // Sync // была перезагрузка
    }

  }

  //-------------------------------------------------------------------------------------------------------------------------------
   if(UCmd.cs.SyncTime)
   if (this->isAccel)
   if (this->AccelOn)
   {
        res=SyncTime(CPrt);
        UCmd.cs.SyncTime=false;
        if(res!=1)
                 return res;

   } // Sync ExecComs
  //-------------------------------------------------------------------------------------------------------------------------------
  // isMeteo
  //-------------------------------------------------------------------------------------------------------------------------------
  if(UCmd.cs.wstat)
  {
    res=GetMWStat(CPrt);if(res!=1) return res;UCmd.cs.wstat=false;
    if(isMeteo) if(!md.nometeo) SWBuf.on=true;else SWBuf.on=false;
    if(SWBuf.on)
    {
      res=GetWPackets(CPrt);
      CalcWPacket(); // часть пакетов может быть прочитана
      if(res!=1) return res;
    }
  }
  //-------------------------------------------------------------------------------------------------------------------------------
  // CBStat->Checked
  //-------------------------------------------------------------------------------------------------------------------------------
  if(UCmd.cs.d3stat) {res=Getd3Stat(CPrt);          if(res!=1) return res;UCmd.cs.d3stat=false; }
  if(UCmd.cs.t5stat) {res=Gett5Stat(CPrt);          if(res!=1) return res;UCmd.cs.t5stat=false; }
  if(UCmd.cs.ResErr) {res=ResErr(CPrt);             if(res!=1) return res;UCmd.cs.ResErr=false; }
  if(UCmd.cs.GetInfo){res=GetInfo(CPrt,UCmd.infoix);if(res!=1) return res;UCmd.cs.GetInfo=false;}
  //-------------------------------------------------------------------------------------------------------------------------------
  if(UCmd.cs.GetXYCur){res=GetXYCur(CPrt); if(res!=1) return res;UCmd.cs.GetXYCur=false;} // !isBS
  if(UCmd.cs.wres)    {res=GetMWRes(CPrt); if(res!=1) return res;UCmd.cs.wres=false;    } // isMeteo
  //*******************************************************************************************************************************
  // Ручные команды
  //*******************************************************************************************************************************
  if(UCmd.cs.GetStp)   {res=GetStp(CPrt,UCmd.tstp);if(res!=1) return res;UCmd.cs.GetStp=false;  }
  if(UCmd.cs.NVMState) {res=NVMState(CPrt);        if(res!=1) return res;UCmd.cs.NVMState=false;}
  if(UCmd.cs.SetTmp)   {res=SetTmp(CPrt);          if(res!=1) return res;UCmd.cs.SetTmp=false;  }
  if(UCmd.cs.CopyStp)  {res=CopyStp(CPrt);         if(res!=1) return res;UCmd.cs.CopyStp=false; }
  //-------------------------------------------------------------------------------------------------------------------------------
  // Критическая команда: всегда UCmd.cs.WriteStp=false
  //-------------------------------------------------------------------------------------------------------------------------------
  if(UCmd.cs.WriteStp) {res=WriteStp(CPrt);UCmd.cs.WriteStp=false;if(res!=1) return res;}
  //-------------------------------------------------------------------------------------------------------------------------------
  if(UCmd.cs.SetAdr) {res=SetAdr(CPrt,UCmd.adr,0x01);if(res!=1) return res;UCmd.cs.SetAdr=false; }
  if(UCmd.cs.Setn50) {res=Setn50(CPrt,UCmd.n50);     if(res!=1) return res;UCmd.cs.Setn50=false; }
  if(UCmd.cs.SetTmax) {res=SetTmax(CPrt,UCmd.Tmax); if(res!=1) return res;UCmd.cs.SetTmax=false;}
  //-------------------------------------------------------------------------------------------------------------------------------
  if(UCmd.cs.MODE)    {res=MODE(CPrt);    if(res!=1) return res;UCmd.cs.MODE=false;    }
  if(UCmd.cs.SetPulse){res=SetPulse(CPrt);if(res!=1) return res;UCmd.cs.SetPulse=false;}
  //------------------------------------------------------------------------------------------------------------------------------
  if(UCmd.cs.Reset)  {res=Reset(CPrt);if(res!=1) return res;UCmd.cs.Reset=false;}
  //------------------------------------------------------------------------------------------------------------------------------
  if(UCmd.cs.StackSOn)
  {
    res=StackSOn(CPrt);if(res!=1) return res;UCmd.cs.StackSOn=false;
    SBuf.Clr();SBuf.on=true;
    StackStartTime=Now();
  }
  //------------------------------------------------------------------------------------------------------------------------------
  return res;


}

//-----------------------------------------------------------------------------

int TBTSensor::GetXYCur_ASIN_211(TCOMPort* P)
{
  BYTE* cmdrespond=NULL;
  AnsiString s("");


//Тут прописываем запрос и обработку показаний углов из ИН-Д3
          measresASIN* res=NULL;
          //Почитали углы и единицы измерения

          res=P->BS4I1_Cmd_ASIN_MEAS_request(adr);
          if (res)
          {
               XCur=res->angle_x; YCur=res->angle_y;

               if (this->isSVWG == false)
               {

                //Переводим все в секунды
                 if (res->unit_x) XCur*=60;
                 if (res->unit_y) YCur*=60;
                //Применяем настройки поожения датчика
                 double X,Y;
                 X=XCur; Y=YCur;

                 XCur_raw = X;
                 YCur_raw = Y;

                 if (this->on)
                 {
                        /*
                         if (((XCur != -31777)&&(abs(XCur_raw) > i_sensor_range)) || ((YCur != -31777)&&(abs(YCur_raw) > i_sensor_range)))
                         {
                                 b_out_of_range = true;
                         }
                         else
                         {
                                 b_out_of_range = false;
                         }
                         */
                         isOutOfRange();
                 }

                 if(InvY) Y=-Y; // else anl=-anl;

                 // InvY==0 - вращение новой левой системы координат по часовой стрелке
                 // InvY==1 - вращение новой правой системы координат против часовой стрелки

                   if (bGlobalSetZeroFlag == true)
                   {
                        if (X0 == 0)
                        {
                                X0 = XCur;
                        }

                        if (Y0 == 0)
                        {
                                Y0 = YCur;
                        }
                   }

                 double anl=RotX*M_PI/180.0;

                 XCur=+X*cos(anl) + Y*sin(anl)- X0;
                 YCur=-X*sin(anl) + Y*cos(anl)- Y0;

#ifdef ANGLE_TO_DEFLECTION_CONVERSION_MODE

        CalcBridgDeflection();
        
#endif
              }
              else
              {
                TCur = YCur; sw.ft5dr = 1;

                if (Ung == 0)
                {
                        XCur = XCur + ((Kt-Ktm)*(TCur - 24.f)) - D0;
                }
                else
                {
                        double dcor = 0;
                        dcor = XCur + ((Kt-Ktm)*(TCur - 24.f)) - D0;
                        XCur = Ung*dcor;
                        XCur/=1000.f;
                }

              }

                 NewDataFlag=true;
         }
  return ReOpen(P);

}

int TBTSensor::GetXYCur_IND3(TCOMPort* P)
{
  BYTE* cmdrespond=NULL;
  BYTE unitsx, unitsy;
  unsigned int isn=0;
  AnsiString s("");

       if (this->sIND3sn=="Нет данных")
       {
                ReadSerialNumber(P);
       }

//Тут прописываем запрос и обработку показаний углов из ИН-Д3
       cmdrespond=P->BS4I1_Cmd_211_request(adr, cMEAS_VAL);

       if (cmdrespond!=NULL)
       {
//Почитали углы и единицы измерения
               P->Protocol211->ReadIND3SensorMeasureResult(adr,&XCur,&YCur,&unitsx,&unitsy);

//переводим все в секунды
               if (unitsx) XCur*=60;
               if (unitsy) YCur*=60;

                XCur_raw = XCur;
                YCur_raw = YCur;

                 if (this->on)
                 {
                         /*
                         if (((XCur != -31777)&&(abs(XCur_raw) > i_sensor_range)) || ((YCur != -31777)&&(abs(YCur_raw) > i_sensor_range)))
                         {
                                 b_out_of_range = true;
                         }
                         else
                         {
                                 b_out_of_range = false;
                         }
                         */

                         isOutOfRange();

                 }


               if (this->isSVWG == false)
               {

                   if (bGlobalSetZeroFlag == true)
                   {
                        if (X0 == 0)
                        {
                                X0 = XCur;
                        }

                        if (Y0 == 0)
                        {
                                Y0 = YCur;
                        }
                   }

//учитываем настройки положения датчика
                  double X,Y = 0;
                  X=XCur; Y=YCur;
                  if(InvY) Y=-Y; // else anl=-anl;
                  // InvY==0 - вращение новой левой системы координат по часовой стрелке
                  // InvY==1 - вращение новой правой системы координат против часовой стрелки
                  double anl=RotX*M_PI/180.0;
                  XCur=+X*cos(anl)+Y*sin(anl)-X0;
                  YCur=-X*sin(anl)+Y*cos(anl)-Y0;

#ifdef ANGLE_TO_DEFLECTION_CONVERSION_MODE

        CalcBridgDeflection();
        
#endif


              }
              else
              {
                TCur = YCur;

                if (Ung == 0)
                {
                        XCur = XCur + ((Kt-Ktm)*(TCur - 24.f)) - D0;
                }
                else
                {
                        double dcor = 0;
                        dcor = XCur + ((Kt-Ktm)*(TCur - 24.f)) - D0;
                        XCur = Ung*dcor;
                        XCur/=1000.f;

                }

              }

         NewDataFlag=true;
     }

  return ReOpen(P);
}


//ИН-Д3
int TBTSensor::ExecComs_IND3(TCOMPort* CPrt)
{


       if (this->sIND3sn=="Нет данных")
       {
                ReadSerialNumber(CPrt);
       }

       int res=1;

         if(UCmd.cs.GetAll)
         {
                double D1,D2;
                D1=CoreTime();
                res=GetXYCur_IND3(CPrt);
                if (res!=1)    return res;

                D2=CoreTime();
                DTm_GetAll=D2-D1;

                UCmd.cs.GetAll=false;

//Для ИН-Д3 параметры отсутствуют

                UCmd.cs.d3stat=false;
                UCmd.cs.t5stat=false;
                UCmd.cs.ResErr=false;
                UCmd.cs.GetInfo=false;

                if (this->sw.fdr!=1)  this->sw.fdr=1;
                dataon=false;

        }

   return res;


}

//ИН-Д3 АСИН
int TBTSensor::ExecComs_ASIN_211(TCOMPort* CPrt)
{

       if (this->sIND3sn=="Нет данных")
       {
                ReadSerialNumber(CPrt);
       }

        int res=1;

         if(UCmd.cs.GetAll)
         {
                double D1,D2;
                D1=CoreTime();
                res=GetXYCur_ASIN_211(CPrt);
                if (res!=1)    return res;

                D2=CoreTime();
                DTm_GetAll=D2-D1;

                UCmd.cs.GetAll=false;

//Для ИН-Д3 параметры отсутствуют

                UCmd.cs.d3stat=false;
                UCmd.cs.t5stat=false;
                UCmd.cs.ResErr=false;
                UCmd.cs.GetInfo=false;

                if (this->sw.fdr!=1)  this->sw.fdr=1;
                dataon=false;

        }

   return res;


}


//-----------------------------------------------------------------------------

      #define T_ONE_DSEC      ((1./(24.*60.*60.))/10)

void TSBuf::AttachAccData_float(float* fx, float* fy, int pack_size, TDateTime tstart)
{

//      сдвигаем на 32 записи


//     if (tstart==(TDateTime)0) return;

      memcpy(&last_acc_data.acc_x[0] ,&last_acc_data.acc_x[32], 32*(PACKETS_TO_SAVE-1)*sizeof(double));
      memcpy(&last_acc_data.acc_y[0] ,&last_acc_data.acc_y[32], 32*(PACKETS_TO_SAVE-1)*sizeof(double));

      double dd=0;

      for (int i=0; i<32; i++)
      {
        dd=((double)fx[i]);
        last_acc_data.acc_x[i+32*(PACKETS_TO_SAVE-1)]=dd;
        dd=((double)fy[i]);
        last_acc_data.acc_y[i+32*(PACKETS_TO_SAVE-1)]=dd;
      }

      //last_acc_data.t_end=Now()+50*T_ONE_DSEC;
      last_acc_data.t_end=Now()+100*T_ONE_DSEC;
      last_acc_data.t_start=last_acc_data.t_end-(32*acc_step_per_sec)*(T_ONE_SEC/acc_step_per_sec);
}


void TSBuf::GetCurrAccData(TDateTime t, double* accx, double* accy)
{

       for (int i=0; i<acc_step_per_sec; i++)
       {
                accx[i]=-31777; accy[i]=-31777;
       }

       double dtime=last_acc_data.t_end-t;

       if (dtime<0) return;

       dtime/=(T_ONE_SEC/acc_step_per_sec);

       int idx=32*PACKETS_TO_SAVE-dtime;

       for (int i=0; i<acc_step_per_sec; i++)
       {
          if (idx>=32*PACKETS_TO_SAVE) break;
          accx[i]=last_acc_data.acc_x[idx]; accy[i]=last_acc_data.acc_y[idx];
          idx++;
       }


}

//double t_start_file_time_err[1000];
//int iidx=0;

//int testtimer=0;

bool TSBuf::AccFilesReopen(int nsave_idx, int npack_idx, bool lic)
{
  //TODO: Add your source code here

  fclose(fx);
  fclose(fy);
  fclose(fxt);
  fclose(fyt);

  AnsiString s,name=MainCurrentDirectory;

  if (lic == true)
  {


          name+="Base\\";
          if(!DirectoryExists(name)) CreateDir(name);

          name+="History\\";
          if(!DirectoryExists(name)) CreateDir(name);

          name+=GlobalSystemFolderName;
          if(!DirectoryExists(name)) CreateDir(name);

          name+="\\Adata";
          if(!DirectoryExists(name)) CreateDir(name);

          if(!DirectoryExists(name))
          {
                   if(!CreateDir(name))
                   {
                    return false;
                   }
          }

  }

  // Вот тут переключаем имя файла на следующий блок
//  if ((nsave%(SrsBufToSave*SrsBufPSize)==0)||(AccFilesReopenFlag==true))
  if (AccFilesReopenFlag==true)
  {

     AnsiString stmp(""),stmp1(""),stmp2(""),stmp3("");

     BS4I1_D3Packet_float* P=&Packet_buf_float[nsave_idx%SrsBufPSize];

     //if(nsave>0) TimeStart=TimeStop;

//     if(nsave>0)  ::Sleep(1);

     DWORD tmpCT=P->tmpCT;if(P->stCT>P->spCT) tmpCT--;
     startCT=(P->stCT+tmpCT*65536.0*65536.0)*25.0*1e-9;
     startClock=Clock.GetClock(startCT);
     TimeStart=SNTPCoreTime.GetTime(startClock);
     stmp=FormatDateTime("yy_mm_dd_hh_nn_ss_zzz",TimeStop);


      if (TimeStart==0.0)
      {
         SNTPCoreTime.is_sync=false;
         t_acc_file_start_time=(TDateTime)0.0;
         return false;
      }
      else
      {
           if (t_acc_file_start_time==(TDateTime)0.0) t_acc_file_start_time=TimeStart;
      }

      //Предварительное значение расчетной длины файла
     int i_tval=0;
     double d_tval=0;
     d_tval=T_ONE_SEC*(double)AccFileTimeLen/T_ONE_MSEC;
     i_tval=(int)d_tval;

      //Фиксируем время окончания пакета по последнему пакету по времени датчика
     stopCT=(P->spCT+P->tmpCT*65536.0*65536.0)*25.0*1e-9;
     stopClock=Clock.GetClock(stopCT);
     TimeStop=SNTPCoreTime.GetTime(stopClock);

     /*
      //Считаем суммарное время на формирование кратно длине пакета
     int dCT=rint((stopCT-startCT)*1000.0);
     int dClock=rint((stopClock-startClock)*1000.0);
     int dt=rint((TimeStop-TimeStart)*24.0*3600.0*1000.0);
     */

     double d_block_file_step_time=0;
     double d_block_file_step_time_ms=0;

     double d_block_file_time=0;
     double d_block_file_time_total=0;
     double d_block_file_time_total_ms=0;

     /*
      if (TimeStop>TimeStart)
      {
              d_block_file_time=TimeStop-TimeStart;
              d_block_file_time/=T_ONE_MSEC;
              d_block_file_time/=32;
              d_block_file_time*=((AccFileTimeLen*T_ONE_SEC/T_ONE_MSEC)/(1000/acc_step_per_sec));

      }
      */


      if ((TimeStop>TimeStart_begin)&&(i_packets_saved>0)&&(AccFilesReopenFlag==true))
      {

              stmp1=FormatDateTime("yy_mm_dd_hh_nn_ss_zzz",TimeStop);
              stmp2=FormatDateTime("yy_mm_dd_hh_nn_ss_zzz",TimeStart);
              stmp3=FormatDateTime("yy_mm_dd_hh_nn_ss_zzz",TimeStart_begin);

              d_block_file_step_time=TimeStop-TimeStart_begin;
              d_block_file_step_time/=(double)(32.*(double)i_packets_saved);
              d_block_file_time_total=d_block_file_step_time*((double)(AccFileTimeLen*1000)/(1000/acc_step_per_sec));
              d_block_file_time_total_ms=d_block_file_time_total/T_ONE_MSEC;


              d_block_file_time=TimeStop-TimeStart_begin;
              d_block_file_time/=(double)(32.*(double)i_packets_saved);
              d_block_file_time/=T_ONE_MSEC;

              //d_block_file_time*=((AccFileTimeLen*T_ONE_SEC/T_ONE_MSEC)/(1000/acc_step_per_sec));

              //d_block_file_time_total_ms=d_block_file_time*(double)(((double)AccFileTimeLen*T_ONE_SEC/T_ONE_MSEC)/(1000./(double)acc_step_per_sec));

              i_packets_saved=0;
              TimeStart_begin=0.0;
      }
      else
      {
              if (TimeStop>TimeStart)
              {

              d_block_file_step_time=TimeStop-TimeStart;
              d_block_file_step_time/=(double)(32.);
              d_block_file_time_total=d_block_file_step_time*((double)(AccFileTimeLen*1000)/(1000/acc_step_per_sec));
              d_block_file_time_total_ms=d_block_file_time_total/T_ONE_MSEC;


              d_block_file_time=TimeStop-TimeStart_begin;
              d_block_file_time/=(double)(32);
              d_block_file_time/=T_ONE_MSEC;

              //d_block_file_time*=((AccFileTimeLen*T_ONE_SEC/T_ONE_MSEC)/(1000/acc_step_per_sec));

              //d_block_file_time_total_ms=d_block_file_time*(double)(((double)AccFileTimeLen*T_ONE_SEC/T_ONE_MSEC)/(1000./(double)acc_step_per_sec));

              i_packets_saved=0;
              TimeStart_begin=0.0;
              }
      }


              /*
              if (testtimer>500)
              {
                  testtimer=0;
                  d_block_file_time_total_ms*=-1;
              }
              */


              bool is_sync_good=true;

              if (d_block_file_time_total_ms<0)
              {

                    s_old_name_bin_x="";
                    s_old_name_bin_y="";

                    s_old_name_txt_x="";
                    s_old_name_txt_y="";

                       TimeStart_begin=0.;
                       TimeStart=0.;
                       TimeStop=0.;
                       //Clr();
                       SNTPCoreTime.is_sync=false;
                       return false;
              }
              int i_async_factor_ms=(AccFileTimeLen*1000)-d_block_file_time_total_ms;

              /*//!!! принудительно
              if (abs(i_async_factor_ms)>10)
              {
                       TimeStart_begin=0.;
                       TimeStart=0.;
                       TimeStop=0.;
                       //Clr();
                       SNTPCoreTime.is_sync=false;
                       return false;
              }
              */

      /*//!!! принудительно
      if (d_block_file_time_total_ms==0)
      {
              SNTPCoreTime.is_sync=false;
              return false;
      }
      */

      if (t_acc_file_start_time==(TDateTime)0)
      {
            t_acc_file_start_time=Now();
      }
      else
      {

                 //if (d_block_file_time==0)
                 //{
                 //        t_acc_file_start_time+=(T_ONE_SEC*AccFileTimeLen);
                 //}

                 //else
                 {

                   //t_acc_file_start_time+=d_block_file_time*T_ONE_MSEC;//TimeStart+AccFilesWriteTimeCnt*T_ONE_MSEC*acc_step_per_sec;

                   //t_acc_file_start_time=TimeStart+(AccFilesWriteTimeCnt*(T_ONE_SEC/acc_step_per_sec));

                   double t_calc_start_time=t_acc_file_start_time+d_block_file_time_total_ms*T_ONE_MSEC;

                   //t_acc_file_start_time=TimeStart+(AccFilesWriteTimeCnt*((TimeStop-TimeStart)/32));

                   if           (npack_idx<32)          t_acc_file_start_time=TimeStart+((npack_idx)*d_block_file_step_time);
                   else                                 t_acc_file_start_time=TimeStart+((npack_idx%32)*d_block_file_step_time);

                   //t_acc_file_start_time=t_acc_file_start_time+d_block_file_time_total;//_ms*T_ONE_MSEC;

//                   t_start_file_time_err[iidx]=(t_acc_file_start_time-t_calc_start_time);
//                   t_start_file_time_err[iidx]/=T_ONE_MSEC;
//                   iidx++;

                   //stmp2=FormatDateTime("yy_mm_dd_hh_nn_ss_zzz",TimeStart);

                   ::Sleep(1);

                 }


                     char c_oldname[1024];
                     char c_msec[64];
                     AnsiString s_new_file_name("");

                     //Если отключена синхронизация
                     if (SyncTimeOFF)
                     {
                        d_block_file_time_total_ms=AccFileTimeLen*1000;
                     }

                      sprintf(c_msec,"%07d",rint(d_block_file_time_total_ms));

                      if ((SaveAccBin) || (bGlobalRunFlag == false))
                      {

                              if (s_old_name_bin_x!="")
                              {
                                     s_new_file_name=s_old_name_bin_x;
                                     s_new_file_name+=c_msec;
                                     s_new_file_name+="_";

                                     //s_new_file_name+=i_snum;
                                     s_new_file_name+=s_snum;

                                     s_new_file_name+="X";

                                            if (this->acc_step_per_sec==10)
                                            {
                                                s_new_file_name+="_10Hz";
                                            }

                                            if (this->acc_step_per_sec==50)
                                            {
                                                s_new_file_name+="_50Hz";
                                            }

                                            s_new_file_name+=".f32";

                                      RenameFile(xname,s_new_file_name);
                              }


                              if (s_old_name_bin_y!="")
                              {

                                     s_new_file_name=s_old_name_bin_y;
                                     s_new_file_name+=c_msec;
                                     s_new_file_name+="_";

                                     //s_new_file_name+=i_snum;
                                     s_new_file_name+=s_snum;

                                     s_new_file_name+="Y";

                                            if (this->acc_step_per_sec==10)
                                            {
                                                s_new_file_name+="_10Hz";
                                            }

                                            if (this->acc_step_per_sec==50)
                                            {
                                                s_new_file_name+="_50Hz";
                                            }

                                            s_new_file_name+=".f32";

                                      RenameFile(yname,s_new_file_name);

                              }



                      }

                      if ((SaveAccTxt) || (bGlobalRunFlag == false))
                      {
                              if (s_old_name_bin_x!="")
                              {
                                     s_new_file_name=s_old_name_txt_x;
                                     s_new_file_name+=c_msec;
                                     s_new_file_name+="_";

                                     //s_new_file_name+=i_snum;
                                     s_new_file_name+=s_snum;

                                     s_new_file_name+="X";

                                            if (this->acc_step_per_sec==10)
                                            {
                                                s_new_file_name+="_10Hz";
                                            }

                                            if (this->acc_step_per_sec==50)
                                            {
                                                s_new_file_name+="_50Hz";
                                            }

                                            s_new_file_name+=".txt";

                                      if (lic == true) RenameFile(txname,s_new_file_name);
                              }

                              if (s_old_name_bin_y!="")
                              {
                                     s_new_file_name=s_old_name_txt_y;
                                     s_new_file_name+=c_msec;
                                     s_new_file_name+="_";

                                     //s_new_file_name+=i_snum;
                                     s_new_file_name+=s_snum;

                                     s_new_file_name+="Y";

                                            if (this->acc_step_per_sec==10)
                                            {
                                                s_new_file_name+="_10Hz";
                                            }

                                            if (this->acc_step_per_sec==50)
                                            {
                                                s_new_file_name+="_50Hz";
                                            }

                                            s_new_file_name+=".txt";

                                      if (lic == true) RenameFile(tyname,s_new_file_name);

                              }


                      }

                TimeStart==0.0;


        }

    AccFilesReopenFlag=false;
    AccFilesWriteTimeCnt=0;

    name+="\\";

    if (SyncTimeOFF==false)
    {
            name+=FormatDateTime("yy_mm_dd_hh_nn_ss_zzz_",t_acc_file_start_time);
    }
    else
    {
            name+=FormatDateTime("yy_mm_dd_hh_nn_ss_000_",t_acc_file_start_time);
    }

    s_old_name_bin_x=name;
    s_old_name_bin_y=name;

    s_old_name_txt_x=name;
    s_old_name_txt_y=name;

//!!!    s.printf("%07d_",i_tval);

    s.printf("%07d_",0);

    name+=s;

    s.printf("%d",i_tval);
    i_strlen_tval=strlen(s.c_str());

    //name+=i_snum;
    name+=s_snum;

    xname=name+"X";
    yname=name+"Y";

    txname=name+"X";
    tyname=name+"Y";

    if (this->acc_step_per_sec==10)
    {
        xname+="_10Hz"; yname+="_10Hz";
        txname+="_10Hz"; tyname+="_10Hz";
    }

    if (this->acc_step_per_sec==50)
    {
        xname+="_50Hz";  yname+="_50Hz";
        txname+="_50Hz"; tyname+="_50Hz";
    }

/*
     xname+=".f32";
     yname+=".f32";
*/

     xname+=".tmp";
     yname+=".tmp";


     txname+=".txt";
     tyname+=".txt";
  }


  if ((lic == true) && (bGlobalRunFlag == true))
  {
          if (SaveAccTxt)
          {
                     fxt=fopen(txname.c_str(),"ab");
                     if(fxt==NULL)
                     {
                        return false;
                     }

                   fyt=fopen(tyname.c_str(),"ab");
                  if(fyt==NULL)
                  {
                        fclose(fxt);
                        return false;
                  }

          }


          if (SaveAccBin)
          {
                 fx=fopen(xname.c_str(),"ab");
                 if(fx==NULL)
                 {
                      return false;
                 }

                 fy=fopen(yname.c_str(),"ab");
                 if(fy==NULL)
                 {
                      fclose(fx);
                      return false;
                 }

          }
  }

  SNTPCoreTime.is_sync=true;

  return true;

}

//--------------------СИНХРОНИЗАЦИЯ ПО ВРЕМЕНИ----------------------------------

double CalcLine(double *x,double *y, int N, double X)
{
  double S=0,Sx=0,Sy=0,Sxx=0,Sxy=0,d,a,b,X0,Y0;
  X0=x[0];Y0=y[0];
  for(int i=0;i<N;i++) {S+=1;Sx+=(x[i]-X0);Sy+=(y[i]-Y0);Sxx+=(x[i]-X0)*(x[i]-X0);Sxy+=(x[i]-X0)*(y[i]-Y0);}
  d=S*Sxx-Sx*Sx;a=(Sxx*Sy-Sx*Sxy)/d;b=(S*Sxy-Sx*Sy)/d;

  //TDateTime d_res=Y0+a+b*(X-X0);
  //unsigned short year,mon,day,hour,min,sec,msec=0;
  //d_res.DecodeDate(&year,&mon,&day); d_res.DecodeTime(&hour,&min,&sec,&msec);

  return Y0+a+b*(X-X0);
  //return d_res;
}

/*
extern TSNTPTime SNTPCoreTime;
extern int rint(double d);
*/

void TSNTPTime::Update(void)
{
  double D1,D2,D3;

  //Абсолютное время работы системы в секундах
  D1=CoreTime();
  //Текущее время системных часов
  D2=Now();
  //Абсолютное время работы системы в секундах после выполенния запроса системного времени
  D3=CoreTime();
  //Системное время объекта
  Time=D2;
  //Среднее время с учетом выполненияапроса Now() ???Значение CoreTime на середине выполнения Now()
  Clock=(D3+D1)/2.0;
  //Длительность выполнения Now()
  Latency=D3-D1;

  //накопление статистики
  //Текущее время системы
  Time_buf[n_sync%SYNC_BUF_SIZE]=Time;
  //Усредненное время системы с учетом выполнения Now()
  Clock_buf[n_sync%SYNC_BUF_SIZE]=Clock;
  //Время выполнения функции Now()
  Latency_buf[n_sync%SYNC_BUF_SIZE]=Latency;

  //Счетчик буферов статистики
  n_sync++;
}

//Получить текущее время системы
double TSNTPTime::GetTime(double clock)
{
  //Если отключена синхронизация
  if (SyncTimeOFF) return Now();

  //Если текущий счетчик времени в 0 время не готово
  if(clock==0.0)                        return 0.0;
  //Если буфер статистики не обновлен время не готово
  if(n_sync<SYNC_BUF_SIZE)              return 0.0;
  //расчет текущего времени системы с учетом статистики
  return CalcLine(Clock_buf,Time_buf,SYNC_BUF_SIZE,clock);
}

//------------------------------------------------------------------------------

//
double TAND3Clock::GetClock(double and3clock)
{
  int N=SYNC_BUF_SIZE;bool ok=false;
  if(n_sync<N) return 0.0;
  while(!ok)
  {
    memcpy(AND3Clock_buf_tmp,AND3Clock_buf,N*8);
    memcpy(Clock_buf_tmp,Clock_buf,N*8);
    memcpy(Latency_buf_tmp,Latency_buf,N*8);
    int i;for(i=0;i<N;i++)
    {
      if(AND3Clock_buf[i]!=AND3Clock_buf_tmp[i]) break;
      if(Clock_buf[i]!=Clock_buf_tmp[i]) break;
      if(Latency_buf[i]!=Latency_buf_tmp[i]) break;
    }
    if(i==N) ok=true;
  }
  double max=-1e33,min=1e33;
  for(int i=0;i<N;i++)
  {
    if(Latency_buf_tmp[i]<min) min=Latency_buf_tmp[i];
    if(Latency_buf_tmp[i]>max) max=Latency_buf_tmp[i];
  }
  LatMin=min;LatMax=max;

  //double d_tmp=CalcLine(AND3Clock_buf_tmp,Clock_buf_tmp,SYNC_BUF_SIZE,and3clock);
  //AnsiString s_tmp("");
  //s_tmp=FormatDateTime("yyyy_mm_dd_hh_nn_ss_zzz \r\n",d_tmp);

  return CalcLine(AND3Clock_buf_tmp,Clock_buf_tmp,SYNC_BUF_SIZE,and3clock);
}

//------------------------------------------------------------------------------

void TAND3Clock::Update(double CT1,double CT2,double AND3C)
{




  AND3Clock=AND3C;
  Clock=(CT2+CT1)/2.0;
  Latency=CT2-CT1;

  AND3Clock_buf[n_sync%SYNC_BUF_SIZE]=AND3Clock;
  Clock_buf[n_sync%SYNC_BUF_SIZE]=Clock;
  Latency_buf[n_sync%SYNC_BUF_SIZE]=Latency;
  n_sync++;
}


//------------------------------------------------------------------------------


int TBTSensor::SyncTime(TCOMPort* P)
{
   //Если отключена синхронизация
  if (SyncTimeOFF) return ReOpen(P);

  double lo,hi;
  //Время ядра системы до отправки команды
  double CT1;
  //Время ядра системы после отправки команды
  double CT2;
  //пересчитанное время ...
  double AND3C;

  BYTE *b;
  //Зафиксировали время до отправки команды
  CT1=CoreTime();
  b=P->BS4I1_Cmd(adr,240,0,0,8,TOWCom,TORCom);

  if (b==NULL)  {Sleep(20);   b=P->BS4I1_Cmd(adr,240,0,0,8,TOWCom,TORCom);}
  if (b==NULL)  {Sleep(20);   b=P->BS4I1_Cmd(adr,240,0,0,8,TOWCom,TORCom);}

  if (b==NULL)  return ReOpen(P);


 //Зафиксировали время после отправки команды
  CT2=CoreTime();

  if(b==NULL) return ReOpen(P);

  //Младшая часть ответс датчика
  lo=*((DWORD*)b);
  b+=4;
  //Старшая часть ответа датчика
  hi=*((DWORD*)b);

  //Распаковали и перевели в наносекунды
  AND3C=(lo+hi*65536.0*65536.0)*25e-9;

  //Апдейтим часы буфера под новые данные
  SBuf.Clock.Update(CT1,CT2,AND3C);

  return ReOpen(P);
}


int TBTSensor::ReadSerialNumber(TCOMPort* P)
{
                char cpar[64];
                bool bres=false;
                int len=0;
                int res=0;
                unsigned int uipar;
                BYTE buf[1024];
                AnsiString s("");

                if (this->isASIN == true)
                {
                        unsigned char* bufasin = &buf[0];

                        //TProtocol_ASIN* PrASIN=new TProtocol_ASIN;

                        TProtocol_ASIN* PrASIN=P->ProtocolASIN;


                        PrASIN->MakeRequestIND3SerialASIN(&bufasin,&len,adr);
                        bres=P->PortNWrite(len,bufasin);

                        memset(buf,0,1024);
                        len=5;
                        P->PortNRead(len,buf);
                        if (buf[0]!=0x9C) return - 1;

                        int idx=len;

                        while (buf[idx-1]!=0x7E)
                        {
                                P->PortNRead(1,&buf[idx]);
                                if (buf[idx]==0x7E) break;
                                idx++;
                                if (idx>100) return -1;
                        }
                        idx++;
                        len=idx;

                        unsigned int sn = 0;

                        res = PrASIN->PacketProcIND3SerialASIN(buf,&len,adr, &sn);

                        if (res == 0)
                        {
                                s.printf("%d",sn);
                                this->sIND3sn=s;

                                InportSensorPermissions(sn, &lic_prm);
                                if (this->N==0)lic_prm = 0xffffffff;
                        }


                        //delete PrASIN;
                        return res;
                }


                //TProtocol_211* Pr211=new TProtocol_211;

                TProtocol_211* Pr211=P->Protocol211;

                Pr211->RequestSensorMeterSerialNumber(adr,buf,&len);

                bres=P->PortNWrite(len,buf);
                memset(buf,0,1024);
                len=6;
                P->PortNRead(len,buf);
                if (buf[0]!=0x7E) return - 1;


                int idx=len;


                        while (buf[idx-1]!=0x7E)
                        {
                                P->PortNRead(1,&buf[idx]);
                                if (buf[idx]==0x7E) break;
                                idx++;
                                if (idx>100) return -1;
                        }
                        idx++;
                        len=idx;

                        res=Pr211->packet_proc(buf,len);

                if ((res==0)||(res==-5))
                {
                      res=0;
                      uipar=*(unsigned int*)&buf[4];

                s.printf("%d",uipar);
                this->sIND3sn=s;

                //Если зн прочитался заимствуем из лицензии разрешения

                        unsigned int uisn = (unsigned int) s.ToInt();
                        InportSensorPermissions(uisn, &lic_prm);
                        if (this->N==0)lic_prm = 0xffffffff;
                }

                //delete Pr211;
                return res;
}

#define  GSPERMISSIONS_BFILES (0x0001<<0)
#define  GSPERMISSIONS_MODBUS (0x0001<<1)
#define  GSPERMISSIONS_OPC (0x0001<<2)

bool TBTSensor::is_license_bfiles()
{
        if ((lic_prm&GSPERMISSIONS_BFILES)!=0) return true;
        return false;
}

bool TBTSensor::is_license_modbus()
{
        if ((lic_prm&GSPERMISSIONS_MODBUS)!=0) return true;
        return false;
}

bool TBTSensor::is_license_opc()
{
        if ((lic_prm&GSPERMISSIONS_OPC)!=0) return true;
        return false;
}

int TBTSensor::CalcBridgDeflection(void)
{
        //Общая настройка для объекта
        l_prolet_length = iGlobalMostProletLen;

        if (l_prolet_length > 1)
        {
                XCur = XCur/3600; //Перевели в градусы
                XCur = (XCur * M_PI)/180; //Переведи в радианы
                double f  = (l_prolet_length/M_PI*tan(XCur));
                XCur = f;
        }
}

boolean TBTSensor::isOutOfRange(void)
{
        //Общая настройка для объекта
        i_sensor_range = iGlobalMostSensorRang;

        if (((XCur != -31777)&&(abs(XCur_raw) > i_sensor_range)) || ((YCur != -31777)&&(abs(YCur_raw) > i_sensor_range)))
        {
              b_out_of_range = true;
        }
        else
        {
              b_out_of_range = false;
        }

        return b_out_of_range;
}
