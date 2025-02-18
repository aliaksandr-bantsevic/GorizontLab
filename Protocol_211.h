//---------------------------------------------------------------------------

#ifndef Protocol_211H
#define Protocol_211H
//---------------------------------------------------------------------------
#include "Protocol.h"

#define PACKET_START 0x7E       //разделитель пакетов
#define PACKET_END   0x7E       //разделитель пакетов
#define ESCAPE_BYTE  0x7D       //escape байт
#define CODE7D_BYTE  0x5D       //0x7D код байт
#define CODE7E_BYTE  0x5E       //0x7E код байт

#define PACKETBUF_LEN 1024

typedef enum {

MEAS_VAL=0x01,        //показания измерителя
MEAS_VER=0x0E,        //Версия измерителя
PACK_ERR=0xFF         //сообщение об ошибке

} main_packet_211_id;

typedef enum {

MAIN_PR_211=0x9B,     //основные протоколы 2.11
ADDN_PR_211=0x9C,     //дополнительныепротоколы 2.11
PACK_PR_210=0x9A,     //дополнительныепротоколы 2.10
PACK_PR_NVM=0x9D      //дополнительныепротоколы запись в NVM

} protocol_211_id;

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

//смещение в пакете на начало полезных данных
#define DATAOFFSET 4

class TProtocol_211 : public TProtocol
{
public:

	TProtocol_211();
	~TProtocol_211();

	void setProtocol(void) override;

	int request_curr_XY(BYTE addr) override;
	int request_curr_XY(BYTE addr, BYTE* buf, int* idx)  override;

	int accept_response_curr_XY(BYTE addr) override;
    int accept_response_curr_XY(BYTE addr, BYTE* buf, int* idx) override;

private:

	void RequestMainMeterPacket(unsigned char addr, main_packet_211_id packet);
	void RequestMainMeterPacket(unsigned char addr, main_packet_211_id packet, BYTE* buf, int* idx);

	int EscapeBytesEncode(unsigned char* buf, int* len, int frame);
	int EscapeBytesDecode(unsigned char* buf, int* len, int frame);
	int packet_proc();
	int packet_proc(unsigned char* buf, int* len, BYTE addr);
	int ChceckSumCheck(void);
	int ChceckSumCalculate(void);
	int AcceptSensorMeasVal();

};




#endif
