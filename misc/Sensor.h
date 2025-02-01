// Sensor.h

//---------------------------------------------------------------------------
#ifndef SENSOR
#define SENSOR
//-----------------------------------------------------------------------------
#include "BTCOM.h"
#include <stdio.h>

#include "defs.h"

//#include "GSPermissions.h"

//-----------------------------------------------------------------------------

#define T_ONE_DAY       (1.)
#define T_ONE_HOUR      (1./24.)
#define T_ONE_MIN       (1./(24.*60.))
#define T_ONE_SEC       (1./(24.*60.*60.))
#define T_ONE_MSEC      ((1./(24.*60.*60.))/1000)
#define T_ONE_DSEC      ((1./(24.*60.*60.))/10)



//сколько пакетов сохранять для скользящего буфера
#define PACKETS_TO_SAVE         50
//#define SYNC_BUF_SIZE 128

#define SYNC_BUF_SIZE 32

//сколько отсчетов за секунду по ускорениям
//#define ACC_POINTS_TO_SEC       50

//Шаг сохранения данных по ускорениям в скользящем буфере
//#define PACKETS_TO_SAVE_STEP    (T_ONE_SEC/ACC_POINTS_TO_SEC)

extern AnsiString GlobalSystemFolderName;

//Данные по ускорению за последние 9,8 с

typedef struct {

TDateTime t_start;
TDateTime t_end;

double acc_x[32*PACKETS_TO_SAVE];
double acc_y[32*PACKETS_TO_SAVE];

} struct_last_acc_data;


typedef struct
{
/*
  DWORD GetAll:1;
  DWORD Reset:1;
  DWORD SetAdr:1;
  DWORD GetStp:1;
  DWORD WriteStp:1;
  DWORD CopyStp:1;
  DWORD NVMState:1;
  DWORD SetTmp:1;

  DWORD Prog:1;
  DWORD D3Set:1;
  DWORD ResErr:1;
  DWORD GetXYCur:1;
  DWORD d3stat:1;
  DWORD t5stat:1;
  DWORD wstat:1;
  DWORD wres:1;

  DWORD GetInfo:1;
  DWORD MODE:1;
  DWORD StackOn:1;
  DWORD StackSOn:1;
  DWORD StackOff:1;
  DWORD Setn50:1;
  DWORD SetTmax:1;
  DWORD SetPulse:1;

  DWORD r4:1;
  DWORD r5:1;
  DWORD r6:1;
  DWORD r7:1;
  DWORD r8:1;
  DWORD r9:1;
  DWORD r10:1;
  DWORD r11:1;
*/


  DWORD GetAll:1;
  DWORD Reset:1;
  DWORD SetAdr:1;
  DWORD GetStp:1;
  DWORD WriteStp:1;
  DWORD CopyStp:1;
  DWORD NVMState:1;
  DWORD SetTmp:1;

  DWORD Prog:1;
  DWORD D3Set:1;
  DWORD ResErr:1;
  DWORD GetXYCur:1;
  DWORD d3stat:1;
  DWORD t5stat:1;
  DWORD wstat:1;
  DWORD wres:1;

  DWORD GetInfo:1;
  DWORD MODE:1;
  DWORD StackOn:1;
  DWORD StackSOn:1;
  DWORD StackOff:1;
  DWORD Setn50:1;
  DWORD SetTmax:1;
  DWORD SetPulse:1;

  DWORD SyncTime:1;
  DWORD r5:1;
  DWORD r6:1;
  DWORD r7:1;
  DWORD r8:1;
  DWORD r9:1;
  DWORD r10:1;
  DWORD r11:1;


} BS4I1_UserCmdSet;
//-----------------------------------------------------------------------------------------
typedef struct
{
  BS4I1_UserCmdSet cs;
  int  adr;
  int  tstp;
  int  infoix;
  DWORD info;
  WORD n50;
  char Tmax;

  DWORD tick_rate;
  DWORD puls_time;
  BYTE  del_type;

  BYTE mode_com;
  BYTE mode_on;

  bool SClrOn;      // Очищать аппаратный стек при запуске
  bool SClrOff;     // Очищать аппаратный стек при остановке
  bool SProgClrOn;  // Очищать программный стек при запуске
  bool SProgClrOff; // Очищать программный стек при остановке
  WORD nGroup;      // 1.0.2 1...16383 - останавливать после приема каждых nGroup пакетов

  bool bsadr;
} BS4I1_UserCmd;
//-----------------------------------------------------------------------------------------
typedef struct
{
  WORD te; // ошибки чтения датчика
  WORD ce; // ошибки CRC датчика
  WORD re; // ошибки диапазона датчика
  WORD dr; // удачные измерения c циклическим сбросом для углов 
  WORD drz;// удачные измерения с замораживанием для ускорений
} BS4I1_NResErr;
//-----------------------------------------------------------------------------------------
typedef struct
{
  DWORD sn;
  WORD  te;
  WORD  ce;
  WORD  re;
} BS4I1_d3stat;
//-----------------------------------------------------------------------------------------
typedef struct
{
  DWORD sn;
  WORD  te;
  WORD  re;
  WORD  ae;
} BS4I1_t5stat;
//-----------------------------------------------------------------------------------------
typedef struct
{
  short U;
  short V;
  BYTE  s;
} BS4I1_wres;
//-----------------------------------------------------------------------------------------
typedef struct
{
  DWORD  sn; // количество запросов
  WORD   te; // ошибок TimeOut
  WORD   ce; // ошибок CRC
  WORD   se; // ошибок статуса
} BS4I1_wstat;
//-----------------------------------------------------------------------------------------
typedef struct
{
  WORD nofir1:1;   // Отключить фильтр 1
  WORD nofir2:1;   // Отключить фильтр 2
  WORD noidle:1;   // Отключить IDLE
  WORD nod3:1;     // Отключить таймауты

  WORD not5av:1;   // Не усреднять по температуре
  WORD notrm:1;    // Отключить терморегулятор
  WORD nostack:1;  // Не заполнять буфер акселерометра
  WORD noadrdet:1; // Не детектировать адрес датчика

  WORD noacmp:1;   // Не компенсировать ускорение на 0 частоте
  WORD nofir3:1;   // Отключить фильтр 3
  WORD nopwr:1;    // Отключить питание датчиков для БС
  WORD nometeo:1;  // Отключить метеостанцию

   // WORD reserved:4;
  WORD  tostart:1;  // Запрос по таймауту      // !!! 1.202
  WORD    br115:1;  // Скорость 115200         // !!! 1.202
  WORD     to10:1;  // Таймаут 10 мс (или 5 мс)// !!! 1.202
  WORD reserved:1;
} BS4I1_MODE;
//-----------------------------------------------------------------------------------------
typedef struct
{
  WORD  frst:1; // Перезагрузка
  WORD   fdr:1; // Готовность данных датчика
  WORD ft5dr:1; // Готовность данных температуры
  WORD fthdr:1; // Готовность данных вентилятора

  WORD   fte:1; // Ошибки чтения датчика
  WORD   fce:1; // Ошибки CRC датчика
  WORD   fre:1; // Ошибки диапазона датчика
  WORD    r1:1; // Резерв

  WORD ft5te:1; // Ошибки чтения температуры
  WORD ft5re:1; // Ошибки диапазона температуры
  WORD fthre:1; // Ошибка частоты вентилятора
  WORD ftrme:1; // Ошибка терморегулятора

  WORD  fadr:1; // Идет поиск адреса
  WORD  ftrm:1; // Терморегулятор включен
  WORD    r2:1; // Резерв
  WORD    r3:1; // Резерв
} BS4I1_SW;
//-----------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// Пакет /////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------
// 1.0.1
//#define NUM_PACKS 16
//#define PACK_SIZE 256
// 1.0.2

//#define NUM_PACKS 128

#define NUM_PACKS 64

#define PACK_SIZE 32
#define TCP_MAX_SIZE  8192
extern int SrsBufPSize,SrsBufToSave;
//-----------------------------------------------------------------------------------------
typedef struct
{
  short   x[PACK_SIZE];
  short   y[PACK_SIZE];
  DWORD stCT; // Начало пакета CoreTime
  DWORD spCT; // Конец пакета CoreTime
  DWORD tmpCT;// 1.0.2 CoreTime*tmCT
  WORD  ne;   // 1.0.2 количество ошибок
  WORD drz;   // удачные измерения с замораживанием для ускорений до конца пакета
  short  AX;  // 1.0.2
  short  AY;  //
} BS4I1_D3Packet;



///////////////////////////////////////НОВЫЙ ФОРМАТ//////////////////////////////
typedef struct {
float X;
float Y;
short T;
BS4I1_SW sw;
DWORD pn;
BS4I1_MODE md;
}BS4I1_Res_float;

typedef struct
{
  float   x[PACK_SIZE];
  float   y[PACK_SIZE];
  DWORD stCT; // Начало пакета CoreTime
  DWORD spCT; // Конец пакета CoreTime
  DWORD tmpCT;// 1.0.2 CoreTime*tmCT
  WORD  ne;   // 1.0.2 количество ошибок
  WORD drz;   // удачные измерения с замораживанием для ускорений до конца пакета
  float  AX;  // 1.0.2
  float  AY;  //
} BS4I1_D3Packet_float;


//------------------------------------------------------------------------------

//Объект контроля времени микроконтроллера измерителя
typedef struct
{
  public:
  // Синхронизировать
  bool isSync;
  // Включена синхронизация
  bool isSyncOn;

  double AND3Clock;

  double Clock;
  //задержка прохождения пакета
  double Latency;
  //задержка прохождения пакета минимум
  double LatMin;
  //задержка прохождения пакета максимум
  double LatMax;

  DWORD  n_sync;
  //Буфер статистики
  double AND3Clock_buf[SYNC_BUF_SIZE];

  double Clock_buf[SYNC_BUF_SIZE];
  double Latency_buf[SYNC_BUF_SIZE];

  double AND3Clock_buf_tmp[SYNC_BUF_SIZE];
  double Clock_buf_tmp[SYNC_BUF_SIZE];
  double Latency_buf_tmp[SYNC_BUF_SIZE];

  void Update(double CT1,double CT2,double AND3C);
  double GetClock(double and3clock);
} TAND3Clock;


//------------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------
//  TSBuf
//-----------------------------------------------------------------------------------------
class TSBuf
{
  private:

  public:

    TDateTime  cdold;
    AnsiString s_old_name_bin_x;
    AnsiString s_old_name_bin_y;
    AnsiString s_old_name_txt_x;
    AnsiString s_old_name_txt_y;
    int i_strlen_tval;

  public:
  bool on;
  //****************************************************************************
  FILE* fx;
  FILE* fy;

  AnsiString xyname;
  AnsiString tmp_xname;
  AnsiString tmp_yname;

  FILE* fxt;
  FILE* fyt;

  AnsiString xname;
  AnsiString yname;

  AnsiString txname;
  AnsiString tyname;


  //****************************************************************************
  int    i_packets_saved;
  double TimeStart_begin;
  double TimeStart;
  double TimeStop;
  double startCT;
  double stopCT;
  double startClock;
  double stopClock;
  TAND3Clock Clock;
  //****************************************************************************


  //****************************************************************************
  DWORD res_pn;// Количество измерений GetAll
  DWORD nmeas; // Измерено пакетов
  DWORD nsave; // Сохранено пакетов
  DWORD nread; // Считано и записано пакетов в буфер
  int   nmiss; // Пропущено пакетов
  DWORD ncalc; // Обработано пакетов
  DWORD ecount;// Количестао пакетов с ошибками
  WORD   sw;   // Слово состояния // TODO
  //****************************************************************************
  // Буферы new // 1.0.2
  //****************************************************************************
  //BS4I1_WPacket* Packet_buf; // Оригинальные пакеты
  //short* U;                  // Server2 type==5
  //short* V;                  // Server2 type==6
  //double* Time_buf;          // Server2 type==7
  //DWORD* nmeas_buf;          // Server2 type==8 Для синхронизации времени БПО и PC
  //float *UAver,*VAver,*UMax,*VMax;
  //BYTE* s;

  BS4I1_D3Packet* Packet_buf;           // Оригинальные пакеты
  BS4I1_D3Packet_float* Packet_buf_float;     // Оригинальные пакеты float

  short *X,*Y;         // type==0,type==1

  float *Xf,*Yf;         // type==0,type==1

  double* Time_buf;    // type==4
  DWORD* nmeas_buf;    // Для синхронизации времени БПО и PC
  //float* AccXYrms_buf; // type==3
  float *XAver,*YAver,*XMax,*YMax,*XRms,*YRms,*XYRms;
  BYTE* s;             // status
  //****************************************************************************
  bool SaveBuf(AnsiString dirname,int srs, bool lic);
  bool SaveBuf_spectrum(AnsiString dirname,int srs);

  void InitBuf(bool isNew)
  {
    if((isNew)&&(X==NULL))  X=new short[SrsBufPSize*PACK_SIZE];
    if((isNew)&&(Y==NULL))  Y=new short[SrsBufPSize*PACK_SIZE];

    if((isNew)&&(Xf==NULL))  Xf=new float[SrsBufPSize*PACK_SIZE];
    if((isNew)&&(Yf==NULL))  Yf=new float[SrsBufPSize*PACK_SIZE];

    //if((isNew)&&(AccXYrms_buf==NULL)) AccXYrms_buf=new float[SrsBufPSize];
    if((isNew)&&(Time_buf==NULL)) Time_buf=new double[SrsBufPSize];

    if((isNew)&&(Packet_buf==NULL))   Packet_buf=new BS4I1_D3Packet[SrsBufPSize];

//Это для нового датчика плавающей запятой
    if((isNew)&&(Packet_buf_float==NULL))   Packet_buf_float=new BS4I1_D3Packet_float[SrsBufPSize];

    if((isNew)&&(nmeas_buf==NULL)) nmeas_buf=new DWORD[SrsBufPSize];

    if((isNew)&&(XAver==NULL)) XAver=new float[SrsBufPSize];
    if((isNew)&&(YAver==NULL)) YAver=new float[SrsBufPSize];
    if((isNew)&&(XMax==NULL))  XMax=new float[SrsBufPSize];
    if((isNew)&&(YMax==NULL))  YMax=new float[SrsBufPSize];
    if((isNew)&&(XRms==NULL))  XRms=new float[SrsBufPSize];
    if((isNew)&&(YRms==NULL))  YRms=new float[SrsBufPSize];
    if((isNew)&&(XYRms==NULL)) XYRms=new float[SrsBufPSize];
    if((isNew)&&(s==NULL))     s=new BYTE[SrsBufPSize];

    if((!isNew)&&(X!=NULL)) {delete[] X;X=NULL;}
    if((!isNew)&&(Y!=NULL)) {delete[] Y;Y=NULL;}
    //if((!isNew)&&(AccXYrms_buf!=NULL)){delete[] AccXYrms_buf;AccXYrms_buf=NULL;}
    if((!isNew)&&(Time_buf!=NULL)){delete[] Time_buf;Time_buf=NULL;}
    if((!isNew)&&(Packet_buf!=NULL)){delete[] Packet_buf;Packet_buf=NULL;}
    if((!isNew)&&(nmeas_buf!=NULL)){delete[] nmeas_buf;nmeas_buf=NULL;}

    if((!isNew)&&(XAver!=NULL)){delete[] XAver;XAver=NULL;}
    if((!isNew)&&(YAver!=NULL)){delete[] YAver;YAver=NULL;}
    if((!isNew)&&(XMax!=NULL)) {delete[] XMax; XMax=NULL;}
    if((!isNew)&&(YMax!=NULL)) {delete[] YMax; YMax=NULL;}
    if((!isNew)&&(XRms!=NULL)) {delete[] XRms; XRms=NULL;}
    if((!isNew)&&(YRms!=NULL)) {delete[] YRms; YRms=NULL;}
    if((!isNew)&&(XYRms!=NULL)){delete[] XYRms;XYRms=NULL;}

    if((!isNew)&&(s!=NULL)) {delete[] s;s=NULL;}

  }

  void Clr(void);

  TSBuf(void)
  {

//Инициализируем корректно

        Packet_buf=NULL;
        X=NULL;
        Y=NULL;
        Time_buf=NULL;
        nmeas_buf=NULL;
        XAver=NULL;
        YAver=NULL;
        XMax=NULL;
        YMax=NULL;
        XRms=NULL;
        YRms=NULL;
        XYRms=NULL;
        s=NULL;

////////////////////////////////////////////////////

        on=false;
        fx=NULL;
        fy=NULL;
        fxt=NULL;
        fyt=NULL;
        Clr();
  };

  virtual ~TSBuf() {InitBuf(false);if(fx!=NULL) fclose(fx);if(fy!=NULL) fclose(fy);};

  bool ShowInChartDlg;

  struct_last_acc_data last_acc_data;

  void AttachAccData(short* sx, short* sy, int pack_size);

  void AttachAccData_float(float* fx, float* fy, int pack_size, TDateTime tstart);

  void TSBuf::GetCurrAccData(TDateTime t, double* accx, double* accy);

  int  acc_step_per_sec;
  bool AccFilesReopen(int nsave_idx, int npack_idx, bool lic);
  int  i_snum;
  AnsiString s_snum;
  bool                  AccFilesReopenFlag;
  int                   AccFilesWriteTimeCnt;
  TDateTime             t_acc_file_start_time;

//Счетчик миллисекунд на формирование блок-файла
  unsigned short        i_msec;

//Счетчик тиков датчика на начало пакета
  unsigned long long start_sens_tick_cnt;
//Счетчик тиков датчика на конец пакета
  unsigned long long stop_sens_tick_cnt;

  bool startsave;

};
//-----------------------------------------------------------------------------------------
//  TSWBuf
//-----------------------------------------------------------------------------------------
typedef struct
{
  short   u[PACK_SIZE],v[PACK_SIZE];
  BYTE    s[PACK_SIZE];
  DWORD stCT;  // start CoreTime
  DWORD spCT;  // stop  CoreTime
  DWORD tmpCT; // CoreTime*tmpCT
} BS4I1_WPacket;
//-----------------------------------------------------------------------------------------
class TSWBuf
{
  private:
  public:
  bool on;
  //****************************************************************************
  FILE* fx;
  FILE* fy;
  AnsiString xname;
  AnsiString yname;
  //****************************************************************************
  DWORD res_pn;// Количество измерений GetMWStat
  DWORD nmeas; // Измерено пакетов
  DWORD nsave; // Сохранено пакетов
  DWORD nread; // Считано и записано пакетов в буфер
  int   nmiss; // Пропущено пакетов
  DWORD ncalc; // Обработано пакетов
  DWORD ecount;// Количестао пакетов с ошибками
  //****************************************************************************
  // Буферы new // 1.0.2
  //****************************************************************************
  BS4I1_WPacket* Packet_buf; // Оригинальные пакеты
  short* U;                  // Server2 type==5
  short* V;                  // Server2 type==6
  double* Time_buf;          // Server2 type==7
  DWORD* nmeas_buf;          // Server2 type==8 Для синхронизации времени БПО и PC
  float *UAver,*VAver,*UMax,*VMax,*URms,*VRms;
  BYTE* s;
  //****************************************************************************
  bool SaveBuf(AnsiString dirname,int srs);

  void InitBuf(bool isNew)
  {

    if((isNew)&&(U==NULL))  U=new short[SrsBufPSize*PACK_SIZE];
    if((isNew)&&(V==NULL))  V=new short[SrsBufPSize*PACK_SIZE];
    if((isNew)&&(Time_buf==NULL))  Time_buf=new double[SrsBufPSize];
    if((isNew)&&(Packet_buf==NULL))Packet_buf=new BS4I1_WPacket[SrsBufPSize];
    if((isNew)&&(nmeas_buf==NULL)) nmeas_buf=new DWORD[SrsBufPSize];

    if((isNew)&&(UAver==NULL)) UAver=new float[SrsBufPSize];
    if((isNew)&&(VAver==NULL)) VAver=new float[SrsBufPSize];
    if((isNew)&&(UMax==NULL))  UMax=new float[SrsBufPSize];
    if((isNew)&&(VMax==NULL))  VMax=new float[SrsBufPSize];
    if((isNew)&&(URms==NULL))  URms=new float[SrsBufPSize];
    if((isNew)&&(VRms==NULL))  VRms=new float[SrsBufPSize];
    if((isNew)&&(s==NULL))     s=new BYTE[SrsBufPSize];

    /*
    if((!isNew)&&(U!=NULL)) {U=NULL;}
    if((!isNew)&&(V!=NULL)) {V=NULL;}
    if((!isNew)&&(Time_buf!=NULL)){Time_buf=NULL;}
    if((!isNew)&&(Packet_buf!=NULL)){Packet_buf=NULL;}
    if((!isNew)&&(nmeas_buf!=NULL)){nmeas_buf=NULL;}

    if((!isNew)&&(UAver!=NULL)){UAver=NULL;}
    if((!isNew)&&(VAver!=NULL)){VAver=NULL;}
    if((!isNew)&&(UMax!=NULL)) {UMax=NULL;}
    if((!isNew)&&(VMax!=NULL)) {VMax=NULL;}
    if((!isNew)&&(URms!=NULL)) {URms=NULL;}
    if((!isNew)&&(VRms!=NULL)) {VRms=NULL;}
    if((!isNew)&&(s!=NULL))    {s=NULL;}
    return;//!!!
    */

    if((!isNew)&&(U!=NULL)) {delete[] U;U=NULL;}
    if((!isNew)&&(V!=NULL)) {delete[] V;V=NULL;}
    if((!isNew)&&(Time_buf!=NULL)){delete[] Time_buf;Time_buf=NULL;}
    if((!isNew)&&(Packet_buf!=NULL)){delete[] Packet_buf;Packet_buf=NULL;}
    if((!isNew)&&(nmeas_buf!=NULL)){delete[] nmeas_buf;nmeas_buf=NULL;}

    if((!isNew)&&(UAver!=NULL)){delete[] UAver;UAver=NULL;}
    if((!isNew)&&(VAver!=NULL)){delete[] VAver;VAver=NULL;}
    if((!isNew)&&(UMax!=NULL)) {delete[] UMax;UMax=NULL;}
    if((!isNew)&&(VMax!=NULL)) {delete[] VMax;VMax=NULL;}
    if((!isNew)&&(URms!=NULL)) {delete[] URms;URms=NULL;}
    if((!isNew)&&(VRms!=NULL)) {delete[] VRms;VRms=NULL;}
    if((!isNew)&&(s!=NULL))    {delete[] s;s=NULL;}
  }
  void Clr(void);

  TSWBuf(void)
  {
// Инициализируем корректно  
        U=NULL;

        fx=NULL;
        fy=NULL;

        Packet_buf=NULL;
        U=NULL;
        V=NULL;
        Time_buf=NULL;
        nmeas_buf=NULL;
        UAver=NULL;
        VAver=NULL;
        UMax=NULL;
        VMax=NULL;
        URms=NULL;
        VRms=NULL;
        s=NULL;
//////////////////////////

        on=false;
        fx=NULL;
        fy=NULL;
        Clr();
  };

  virtual ~TSWBuf()
  {
        InitBuf(false);if(fx!=NULL) fclose(fx);if(fy!=NULL) fclose(fy);
  };


};
//-----------------------------------------------------------------------------------------
// Настройки //////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------
typedef struct
{
  // Ядро
  DWORD sys_frq; //  0:4 частота ядра
  // Линия
  BYTE adr;      //  4:1 адрес устройства
  // Датчик ИН-Д3
  BYTE d3adr;    //  5:1 адрес датчика ИН-Д3 (для посылки 5 байт - от 0 до 124)
  BYTE d3maxadr; //  6:1 максимальный адрес датчика ИН-Д3
  BYTE rb1;      //  7:1 резерв
  // Температура
  WORD t5nav;    //  8:2 Количество точек усреднения по температуре
  WORD t5nerr;   // 10:2 Допустимое количество ошибок по температуре при усреднении
  WORD t5hmin;   // 12:2 Минимальная допустимая частота врвщения вращения вентилятора
  // Ошибки
  WORD neadr;    // 14:2 Попыток найти адрес датчика до выставления флага sw.fte
  WORD nte;      // 16:2 Ошибок чтения до выставления флага sw.fte
  WORD nce;      // 18:2 Ошибок CRC до выставления флага sw.fce
  WORD nre;      // 20:2 Ошибок диапазона до выставления флага sw.fre
  WORD ndr;      // 22:2 Удачных измерений до выставления флага sw.fdr

  WORD ntrm;     // 24:2 Циклов терморегулирования до выставления флага sw.ftrme
  WORD nthrd;    // 26:2 Циклов измерения частоты тахометра до выставления флага sw.fthdr

  WORD n50;      // 28:2 Делитель фильтра
  // Mode
  BS4I1_MODE md; // 1.0.2
  // 32 байта 24.10.2014
} BS4I1_STP;
//-----------------------------------------------------------------------------------------


//Структура описания датчика
//??? Тут и объявлеия функций!
typedef struct
{

//Класс P_211 датчик понимает и можно использовать при опросах

//  TProtocol_211* prt_211;

//Датчик включен в опрос
  bool on;

//Порт-хозяинвключен в опрос
  bool porton;

  bool dataon;  // порт включен, опрос БПО включен, был ответ БПО,
                // ошибок чтения и CRC меньше NumRErrSrs NumCErrSrs

  bool Adataon; // is Accel готовность ускорений

  bool Wdataon; // is Meteo готовность скорости ветра
  //****************************************************************************
  double StackStartTime;
  double Tm_Start;
  double Tm_Stop;
  double Tm_dataon;
  double Tm_fdr;

  double Tm_pn; DWORD  pn_Tm; // Для синхронизации времени БПО и PC + nmeas_buf

  double Tm_Wpn;DWORD Wpn_Tm; // Для синхронизации времени БПО и PC + nmeas_buf
  double DTm_GetAll;
  DWORD  d3r_tm;              // d3r.tm
  //****************************************************************************
  AnsiString Name;
  AnsiString SetData;
  AnsiString AdjData;
  AnsiString Note;
  AnsiString Place;
  //****************************************************************************
//???Порядклвыфномер в системе
  int N;
//Адрес
  byte adr;
//Заводской номер
  int SND3;
  //****************************************************************************
//---------------Для ИН-Д3 только это-------------------------------------------
  double  XCur; // Текущие показания датчика по X
  double  YCur; // Текущие показания датчика по Y

  double  XCur_raw; // Текущие показания датчика по X без поправки и пересчета
  double  YCur_raw; // Текущие показания датчика по Y без поправки и пересчета

//------------------------------------------------------------------------------

  double  TCur; // Текущие показания темрературы
  WORD   ThCur; // Текущие показания тахометра, Гц
  double PwCur; // Текущая мощность
  //****************************************************************************
  double T0;   //Коррекция 0 датчикатемпературы
  double X0;   //Смещение 0 по Y
  double Y0;   //Смещение 0 по X
  bool   InvY; //Инвертировать осьY системы координат датчика
  double RotX; // Угол вращения системы координат датчика по отношению к системе координат объекта, угл. град.
  //****************************************************************************
  double AXAver,AYAver,AXMax,AYMax,AMaxAbs,AMaxAng,AXRms,AYRms,AXYRms;
  //****************************************************************************
  double WUAver,WVAver,WUMax,WVMax,WURms,WVRms;

  //???
  BYTE wstatus;

  //****************************************************************************
  int SrsState,oldSrsState;

  BS4I1_SW  sw,oldsw;   // Слово состояния

  BS4I1_MODE md,oldmd;  // Режим
  DWORD er;DWORD ec;    // Ошибки блока обработки
  BS4I1_NResErr res_err;// Счетчик ошибок измерений
  BS4I1_d3stat d3stat;
  BS4I1_t5stat t5stat;
  BS4I1_wres mwres;
  BS4I1_wstat mwstat;

  BS4I1_UserCmd UCmd;    // Однократные команды

  BS4I1_STP stp;
  byte NVMCur;
  byte NVMSize;
  byte B1;
  byte B2;
  //****************************************************************************
  bool isBS;    // Блок сопряжения
  bool isTermo; // Подключен нагреватель
  bool isTMP05; // Подключен датчик температуры
  bool isMeteo; // Подключена метеостанция
  bool isAccel; // Подключен акселерометр
  bool isHoriz; // Измерение горизонтальных отклонений // >>>

  bool isSVWG;  // Подключен струнный датчик
  double Kt;
  double Ktm;

  double Ung;
  double D0;

  bool isInd3_211;  // Датчик ИН-Д3 протокол 2.11

  //****************************************************************************
  TSBuf   SBuf;  // Буфер пакетов
  TSWBuf SWBuf;  // Буфер пакетов
  //****************************************************************************
  // Команды с индивидуальным адресом
  //****************************************************************************
  int ReOpen(TCOMPort* P);
  int GetTCur(TCOMPort* P);
  int GetXYCur(TCOMPort* P);

  int GetXYCur_IND3(TCOMPort* P);

  int GetThCur(TCOMPort* P);
  int GetPwCur(TCOMPort* P);
  int GetAll(TCOMPort* P);
  int SetAdr(TCOMPort* P,BYTE a,BYTE c);
  int GetStp(TCOMPort* P,BYTE tstp);
  int WriteStp(TCOMPort* P);
  int CopyStp(TCOMPort* P);
  int SetTmp(TCOMPort* P);
  int NVMState(TCOMPort* P);
  int Setn50(TCOMPort* P,WORD n);
  int SetTmax(TCOMPort* P,char T);
  int Prog(TCOMPort* P);
  int D3Set(TCOMPort* P);
  int ResErr(TCOMPort* P);
  int Getd3Stat(TCOMPort* P);
  int Gett5Stat(TCOMPort* P);
  int GetMWStat(TCOMPort* P);
  int GetMWRes(TCOMPort* P);
  int GetInfo(TCOMPort* P,int ix);
  int MODE(TCOMPort* P);
  int ClrReset(TCOMPort* P);
  int SetPulse(TCOMPort* P);
  //****************************************************************************
  // Общие команды с адресом 0
  //****************************************************************************
  int Reset(TCOMPort* P);
  int StackOn(TCOMPort* P,bool on,bool clr,WORD nGroup);
  int StackSOn(TCOMPort* P);
  int StackOff(TCOMPort* P);
  //****************************************************************************
  // 1.0.2
  //****************************************************************************
  int GetPackets(TCOMPort* P);
  int GetWPackets(TCOMPort* P);
  int ExecComs(TCOMPort* CPrt);
  void CalcPacket(void);
  void CalcWPacket(void);
  void Calc_FA(void);
  void Calc_FW(void);

  int ExecComs_IND3(TCOMPort* CPrt);

  bool AccelOn;

  bool NewDataFlag;

  int data_empty_cnt;

  int acc_step_per_sec;

  bool b_is_temperature_sensor;
  //****************************************************************************

  int SyncTime(TCOMPort* P);
  int first_packet_request;

  AnsiString sIND3sn;

  int ReadSerialNumber(TCOMPort* P);

  int l_prolet_length;

//Помещать данные по углам при формировании отчета Excel
  bool b_show_excel_X;
  bool b_show_excel_Y;

  int ExecComs_ASIN_211(TCOMPort* CPrt);
  int GetXYCur_ASIN_211(TCOMPort* P);

  bool isASIN;

  unsigned short lic_prm;

  bool is_license_bfiles();
  bool is_license_modbus();
  bool is_license_opc();

  int CalcBridgDeflection(void);

  int i_sensor_range;
  int i_sensor_range_raw;      
  bool b_out_of_range;

  boolean isOutOfRange(void);

} TBTSensor;
//-----------------------------------------------------------------------------------------
extern DWORD ExP2(DWORD l);
class TBSData // !!! FFT+
{
  private:
  public:

  DWORD size;

  DWORD count;
  double WTime;

  //short  *WS;

  float  *WS;

  double *WD1;
  double *WD2;

  //Init(DWORD sz) {size=sz;WS =new  short[sz];WD1=new double[sz];WD2=new double[sz];};
  void Init(DWORD sz) {size=sz;WS =new  float[sz];WD1=new double[sz];WD2=new double[sz];};

  void SetCount(DWORD cnt,bool even,bool n2)
  {
    if(cnt>size) cnt=size;
    if(even) if(cnt%2>0) cnt--;
    if(n2)   cnt=ExP2(cnt);
    count=cnt;
  }

  TBSData(void) {size=0;count=0;};
  virtual ~TBSData() {if(size>0) {delete[] WS;delete[] WD1;delete[] WD2;}};
};


//--------------------СИНХРОНИЗАЦИЯ ПО ВРЕМЕНИ----------------------------------

//#define SYNC_BUF_SIZE 128

/*
double TimeStart;
double TimeStop;
double startCT;
double stopCT;
double startClock;
double stopClock;
*/

/*
typedef struct
{
  private:
  public:
  bool isSync;    // Синхронизировать
  bool isSyncOn;  // Включена синхронизация

  double AND3Clock;
  double Clock;
  double Latency;
  double LatMin;
  double LatMax;

  DWORD  n_sync;
  double AND3Clock_buf[SYNC_BUF_SIZE];
  double Clock_buf[SYNC_BUF_SIZE];
  double Latency_buf[SYNC_BUF_SIZE];

  double AND3Clock_buf_tmp[SYNC_BUF_SIZE];
  double Clock_buf_tmp[SYNC_BUF_SIZE];
  double Latency_buf_tmp[SYNC_BUF_SIZE];

  void Update(double CT1,double CT2,double AND3C);
  double GetClock(double and3clock);
} TAND3Clock;
*/

//!!!TAND3Clock Clock;

class TSNTPTime
{
  public:
  double Time;
  double Clock;
  double Latency;

  DWORD n_sync;
  double Time_buf[SYNC_BUF_SIZE];
  double Clock_buf[SYNC_BUF_SIZE];
  double Latency_buf[SYNC_BUF_SIZE];

  void Update(void);
  double GetTime(double clock);
  bool is_sync;
  TSNTPTime(void) {is_sync=false;};
  virtual ~TSNTPTime() {};
};



//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

