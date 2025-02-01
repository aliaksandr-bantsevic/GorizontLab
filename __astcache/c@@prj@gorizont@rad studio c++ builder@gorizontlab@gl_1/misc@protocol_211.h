//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdint.h>

#ifndef Protocol_211H
#define Protocol_211H
//---------------------------------------------------------------------------
#endif

//----------------------ОПРЕДЕЛЕНИЕ ТИПОВ ДАННЫХ-----------------------------

//Тип последнего запроса к датчику

typedef enum {

cMEAS_VAL=0x01,        //показания измерителя
cMEAS_VER=0x0E,        //Версия измерителя
cACK_ERR=0xFF,         //сообщение об ошибке

cGET_BAUD=0x11,        //запросить текущуюскорость обмена
cGET_NAME=0x13,        //Запросить имя измерителя
cGET_ZOFF=0x15,        //Запросить смещение нуля
cGET_SVER=0x1A,        //Запросить номер редакции ПО
cGET_SERN=0x1B,        //Запросить заводской номер измерителя
cGET_TACT=0x1C,        //Запросить число тактов усреднения
cGET_PERD=0x1E,        //Запросить период усреднения

} ind3_common_request_type;



//Число тактов усреднения
typedef enum {
        averaging_tacts_1 = 0x00,
	averaging_tacts_2 = 0x01,
	averaging_tacts_4 = 0x02,
	averaging_tacts_8 = 0x03,
	averaging_tacts_16= 0x04,
	averaging_tacts_32= 0x05
} ind3_averaging_tacts;

//Период усреднения
typedef enum {
        averaging_period_10  = 0x00,
	averaging_period_20  = 0x01,
	averaging_period_50  = 0x02,
	averaging_period_100 = 0x03
} ind3_averaging_period;



//Скорости обмена по сом порту
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



//	0x10 - не совпадает контрольная сумма программной памяти измерителя.
//	0x11 - не совпадает контрольная сумма памяти настроек измерителя.
//	0x12 - не совпадает контрольная сумма программной памяти и памяти настроек изме-рителя.


//Коды ошибок приходят от датчика
typedef enum {

CHSUM_MEAS_MEM_ERR = 0x10,
CHSUM_ADJS_MEM_ERR = 0x11,
CHSUM_PROG_MEM_ERR = 0x12,

} ind3_sensor_err;

//Единицы измерения угла

typedef enum {

ANGLE_SECONDS=0,
ANGLE_MINUTES=1

} angle_units;


//Образ датчика

typedef struct  {

unsigned char   sensor_addr;
char            sensor_vers[6];

double          angle_x;
double          angle_y;
angle_units     units_x;
angle_units     units_y;
ind3_baudrate   baud_rate;
char            sensor_name[16];
double           zoffset_x;
double           zoffset_y;
angle_units     units_zoffset_x;
angle_units     units_zoffset_y;
unsigned short  soft_version;
unsigned int    sensor_serial;

ind3_sensor_err last_err;

unsigned char   averaging_tacts;
unsigned char   averaging_period;

int             timeout_period;           //таймаут в миллисекундах
bool            timeout_alarm;            //наличие тайм-аута при опросе


} ind3_sensor;


//------------------ДАТЧИК ИН-Д3------------------------------------------------

/*
Програмная модель(образ) датчика ИН-Д3
*/

class TIND3sensor
{
private:

//Структурасо всеми данными
ind3_sensor   SensorIND3;

protected:

public:

  __fastcall TIND3sensor();
  __fastcall ~TIND3sensor();

//--------------Установка значений состороны протокола2.11----------------------

//Назначить образу датчика адрес
  void SetAddr(unsigned char addr);

//Применить версию измерителя
  void SetSensVer(char* version);

//Применить результат измерения
  void SetAngleX(double angle);
  void SetAngleY(double angle);

//Применить единицы угла
  void SetUnitsX(angle_units unit);
  void SetUnitsY(angle_units unit);

//Применить код ошибки
  void SetSensLastErr(ind3_sensor_err err);

//Применить скорость обмена
  void SetSensBaudRate(ind3_baudrate baud);

//Применить полученное значение версии ПО
  void SetSensSoftVer(unsigned int version);

//Применить поученное значение имя датчика
  void SetSensName(char* name, int len);

//Применить полученные значения смещения нуля
  void SetAngleOffsetX(double angle);
  void SetAngleOffsetY(double angle);

//Применить полученные значения единиц смещения нуля
  void SetUnitsOffsetX(angle_units unit);
  void SetUnitsOffsetY(angle_units unit);

//Применить заводской номер
  void SetSensSerial(unsigned int serial);

//Применить число тактов усреднения
  void SetSensAveragingTacts(unsigned char tacts);

//Применить период усреднения
  void SetSensAveragingPeriod(unsigned char period);

//Установить таймаут в  миллисекундах
//  void SetSensTimeOutPeriod(int period);

//Установить флаг наличия таймаута
//  void SetSensTimeOutAlarm(bool timeout_alarm);


//----------------------Запрос данных от образа датчика со стороны системы мониторинга--------------------

public:

//Прочитать таймаут в  миллисекундах
//  int GetSensTimeOutPeriod(void);

//Прочитать флаг наличия таймаута
//  bool GetSensTimeOutAlarm(void);


//Получить углы
  double GetSensAngleX(void);
  double GetSensAngleY(void);

//Получить единицыи змерения углов
  int   GetSensUnitsX(void);
  int   GetSensUnitsY(void);

//Получить версию измерителя
  char*  GetSensMeterVersion(void);

//Код ошибки при запросе версии измерителя
  ind3_sensor_err  GetSensLastErr(void);

//Получить скорость обмена
  int              GetSensBaudRate(void);

//Получить имя измерителя
  char*            GetSensName(void);

//Получить адрес измерителя (туфта - мы датчики берем из списка заранее адрес зная)
  unsigned char GetSensAddr(void);

//Получить смещение нуля
  double GetSensZeroOffsetX(void);
  double GetSensZeroOffsetY(void);

//Получить единицыи смещения нуля
  int   GetSensZeroOffsetUnitsX(void);
  int   GetSensZeroOffsetUnitsY(void);

//Получить номер редакции ПО измерителя
  unsigned short GetSensSoftVer(void);

//Получить заводской номер измерителя
  unsigned int GetSensSerialNumber(void);

//Получить число тактов усреднения
  ind3_averaging_tacts GetSensAveragingTacts(void);

//Получить период усреднения
  ind3_averaging_period GetSensAveragingPeriod(void);


};


//-------------------Запись указателя надатчик в списке-------------------------

class TIND3sensor_ptr
{
private:

protected:

public:
   TIND3sensor_ptr();
  ~TIND3sensor_ptr();

   TIND3sensor* sensor;
   TIND3sensor_ptr* next;

};

//-------------------------Список датчиков--------------------------------------
class TIND3SensorsList
{
private:

        int SensorExist(unsigned char addr);
        TIND3sensor_ptr* top;
        TIND3sensor_ptr* bottom;

protected:

public:
        TIND3SensorsList();
        ~TIND3SensorsList();

        void Clear();
        int             AddIND3Sensor(unsigned char addr);
        TIND3sensor*    GetIND3Sensor(unsigned char addr);

};
//------------------------------------------------------------------------------


#define PACKET_START 0x7E       //разделитель пакетов
#define PACKET_END   0x7E       //разделитель пакетов
#define ESCAPE_BYTE  0x7D       //escape байт
#define CODE7D_BYTE  0x5D       //0x7D код байт
#define CODE7E_BYTE  0x5E       //0x7E код байт

#define PACKETBUF_LEN 1024
#define REQUESTBUFLEN 64

//смещение в пакете на начало полезных данных
#define DATAOFFSET 4

typedef enum {

MAIN_PR_211=0x9B,     //основные протоколы 2.11
ADDN_PR_211=0x9C,     //дополнительныепротоколы 2.11
PACK_PR_210=0x9A,      //дополнительныепротоколы 2.10
PACK_PR_NVM=0x9D      //дополнительныепротоколы запись в NVM

} protocol_211_id;


typedef enum {

MEAS_VAL=0x01,        //показания измерителя
MEAS_VER=0x0E,        //Версия измерителя
PACK_ERR=0xFF         //сообщение об ошибке

} main_packet_211_id;

typedef enum {

GET_BAUD=0x01,        //запросить текущуюскорость обмена
SET_BAUD=0x02,        //Установить текущую скорость обмена
GET_NAME=0x03,        //Запросить имя измерителя
SET_NAME=0x04,        //Установить имя измерителя
GET_ZOFF=0x05,        //Запросить смещение нуля
SET_ZOFF=0x06,        //Установить смещение нуля
SET_ADDR=0x09,        //Установить новый адрес
GET_SVER=0x0A,        //Запросить номер редакции ПО
GET_SERN=0x0B,        //Запросить заводской номеризмерителя
GET_TACT=0x0C,        //Запросить число тактов усреднения
SET_TACT=0x0D,        //Установить число тактов усреднения
GET_PERD=0x0E,        //Запросить период усреднения
SET_PERD=0x0F         //Установить период усреднения


} addn_packet_211_id;

typedef enum {

MEAS_VAL10=0x01,      //показания измерителя
MEAS_PNG10=0x03       //пинг змерителя

} addn_packet_210_id;

class TProtocol_211 {

 public:

TProtocol_211(void);
~TProtocol_211(void);

 public:

           TIND3sensor*                 sensor_IND3;                      //указательна образ датчика ИН-Д3 скоторым былпоследний обмен данными
           ind3_common_request_type     request_TYPE;                     //тип запроса при последнем опросе датчика

private:

           int CheckSumCalculate();

//           unsigned char*       bufRX;                  //указатель на буфер приема
//           unsigned char*       bufTX;                  //указател на буфер передачи

           unsigned char        packetBUF[PACKETBUF_LEN];         //временный буфер обработки пакета
           int                  packetLEN;                        //длина пакета

           unsigned char        sensorADDR;                       //адресдатчикадля текущих операция

           int ChceckSumCheck(void);
           int ChceckSumCalculate(void);

//Функции обработки пакетов по типам и внесение данных в образ датчика

           int AcceptSensorMeasVal(void);   //пакет   MEAS_VAL=0x01,        //показания измерителя
           int AcceptSensorMeasVer(void);
           int AcceptSensorLastErr(void);
           int AcceptSensorBaudRate(void);
           int AcceptSensorSoftVer(void);
           int AcceptSensorName(void);
           void AcceptSensorAveragingTacts(void);
           void AcceptSensorAveragingPeriod(void);
           int AcceptSensorZeroOffset(void);
           int AcceptSensorSerialNumber(void);

//Формирование запросов к датчику
private:

        void ClearRequestBuf(void);
        void RequestMainMeterPacket(unsigned char addr, main_packet_211_id packet, unsigned char* buf, int* len);
        void RequestAddnMeterPacket(unsigned char addr, addn_packet_211_id packet, unsigned char* buf, int* len);

public:

           TIND3SensorsList*    sensorsList;                      //Список дпрашиваемых датчиков

public:

//Найти в списке датчик с заданным адресом
        TIND3sensor* GetIND3Sensor(unsigned char addr);

public:

//frame - это если обрабатываем заведомо корректный пакет

//Кодировать ESCAPE последовательнсти для передачи
 int EscapeBytesEncode(unsigned char* buf, int* len, int frame);

//Раскодировать ESCAPE последовательности на приеме
 int EscapeBytesDecode(unsigned char* buf, int* len, int frame);

/////////////////////////////////////////////////////////////////////////////////////////
//----------ВНЕШНИЙ ПРОГРАММНЫЙ ИНТЕРФЕЙС ДЛЯ СВЯЗИ С ВЫЗЫВАЮЩИМ ОБЪЕКТОМ---------------

public:

//Указатель на функцию для обработкипринятых даннных отдатчика (находится в вызывающем объекте, вызываеься по окончании обработки пакета)
        void(*accept_data_func)(void);

//Список датчиков ИН-Д3 создается автоматически при создании экземпляра класса протокола 2.11

//Функция добавления датчика в список по адресу
        int AddIND3Sensor(unsigned char addr);

//Очистить спичок датчиков
        void ClearSensorsList();

//Обработка принятого пакета от датчика (указатель на буфер с принятым пакетом и длина пакета)
        int  packet_proc(unsigned char* buf, int len);

//////////////////////ЧТЕНИЕ ДАННЫХ ИЗ ОБРАЗА ДАТЧИКА////////////////////////////

//Прочитать результат измерения из образа датчика вызывающим объектом
int ReadIND3SensorMeasureResult(unsigned char addr, double* angle_X, double* &angle_Y, unsigned char* units_X, unsigned char* units_Y);

//Прочитать версию измерителя из образа датчика вызывающим объектом
int ReadIND3SensorMeterVerion(unsigned char addr, char* version);

//Прочитать скорость обмена из образа датчика вызывающим объектом
int ReadIND3SensorBaudRate(unsigned char addr, int* baud);

//Прочитать имя измерителя из образа датчика вызывающим объектом
int ReadIND3SensorMeterName(unsigned char addr, char* name);

//Прочитать смещение нуля из образа датчика вызывающим объектом
int ReadIND3SensorZeroOffset(unsigned char addr, double* angle_X, double* &angle_Y, unsigned char* units_X, unsigned char* units_Y);

//Прочитать номер редакции ПО из образа датчика вызывающим объектом
int ReadIND3SensorSoftVersion(unsigned char addr, unsigned short* ver);


//Прочитать заводской номер измерителя из образа датчика вызывающим объектом
int ReadIND3SensorSerialNumber(unsigned char addr, unsigned int* serial);

//Прочитать количество тактов усреднения из образа датчика вызывающим объектом
int ReadIND3SensorAveragingTacts(unsigned char addr, int* tacts);

//Прочитать период усреднения из образа датчика вызывающим объектом
int ReadIND3SensorAveragingPeriod(unsigned char addr, int* period);


//Прочитать кодо шибки
int ReadIND3SensorLastErr(unsigned char addr, unsigned char* err);

//////////////////ФОРМИРОВАНИЕ ПАКЕТА ЗАПРОСА В СТОРОНУ ДАТЧИКА/////////////////


//В этот буфер помещается созданный пакет запроса к измерителю
        unsigned char    request_packetBUF[REQUESTBUFLEN];
//Длинапакета запроса к измерителю
        int              request_packetLEN;

//------------------Основной протокол 2.11-------------------------------------

//Запросить результат измерения (сформированный пакет копируется в buf число байт len)

void RequestMeasureResult(unsigned char addr, unsigned char* buf, int* len);

//Запросить версию измерителя
void RequestMeterVerion(unsigned char addr, unsigned char* buf, int* len);

//------------------Дополнтельный протокол 2.11--------------------------------

//Запросить скорость обмена
void RequestSensorBaudRate(unsigned char addr, unsigned char* buf, int* len);

//Запросить имя измерителя
void RequestSensorMeterName(unsigned char addr, unsigned char* buf, int* len);

//Запросить смещение нуля
void RequestSensorZeroOffset(unsigned char addr, unsigned char* buf, int* len);

//Запросить номер редакции ПО
void RequestSensorSoftVersion(unsigned char addr, unsigned char* buf, int* len);

//Запросить заводской номер измерителя
void RequestSensorMeterSerialNumber(unsigned char addr, unsigned char* buf, int* len);

//Запросить количество тактов усреднения
void RequestSensorAveragingTacts(unsigned char addr, unsigned char* buf, int* len);

//Запросить период усреднения
void RequestSensorAveragingPeriod(unsigned char addr, unsigned char* buf, int* len);

//////////ФОРМИРОВАНИЕ ПАКЕТА УСТАНОВКИ ПАРАМЕТРОВ В СТОРОНУ ДАТЧИКА////////////

//Установить скорость обмена
void SetSensorBaudRate(unsigned char addr, ind3_baudrate baud, unsigned char* buf, int* len);

//Установить имя измерителя  передаваемая в функцию строка неболее 16 значащих символов должна заканчиваться нулем
void SetSensorName(unsigned char addr, char* name, unsigned char* buf, int* len);

//Установить смещение нуля
void SetsensorZeroOffset(unsigned char addr, double angle_X, double angle_Y, angle_units units_X, angle_units units_Y, unsigned char* buf, int* len);

//Установить новый адрес
void SetSensorNewAddress(unsigned char addr, unsigned char newaddr, unsigned char* buf, int* len);

//Установить количество тактов усреднения
void SetSensorAveragingTacts(unsigned char addr, ind3_averaging_tacts tacts, unsigned char* buf, int* len);

//Установить период усреднения
void SetSensorAveragingPeriod(unsigned char addr, ind3_averaging_period period, unsigned char* buf, int* len);

void RequestSaveSettingsNVM(unsigned char addr, unsigned char* buf, int* len);

};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


