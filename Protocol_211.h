//---------------------------------------------------------------------------

#ifndef Protocol_211H
#define Protocol_211H
//---------------------------------------------------------------------------
#include "Protocol.h"

#define PACKET_START 0x7E       //����������� �������
#define PACKET_END   0x7E       //����������� �������
#define ESCAPE_BYTE  0x7D       //escape ����
#define CODE7D_BYTE  0x5D       //0x7D ��� ����
#define CODE7E_BYTE  0x5E       //0x7E ��� ����

#define PACKETBUF_LEN 1024

typedef enum {

MEAS_VAL=0x01,        //��������� ����������
MEAS_VER=0x0E,        //������ ����������
PACK_ERR=0xFF         //��������� �� ������

} main_packet_211_id;

typedef enum {

MAIN_PR_211=0x9B,     //�������� ��������� 2.11
ADDN_PR_211=0x9C,     //����������������������� 2.11
PACK_PR_210=0x9A,     //����������������������� 2.10
PACK_PR_NVM=0x9D      //����������������������� ������ � NVM

} protocol_211_id;

typedef enum {

GET_BAUD=0x01,        //��������� ��������������� ������
SET_BAUD=0x02,        //���������� ������� �������� ������
GET_NAME=0x03,        //��������� ��� ����������
SET_NAME=0x04,        //���������� ��� ����������
GET_ZOFF=0x05,        //��������� �������� ����
SET_ZOFF=0x06,        //���������� �������� ����
SET_ADDR=0x09,        //���������� ����� �����
GET_SVER=0x0A,        //��������� ����� �������� ��
GET_SERN=0x0B,        //��������� ��������� ���������������
GET_TACT=0x0C,        //��������� ����� ������ ����������
SET_TACT=0x0D,        //���������� ����� ������ ����������
GET_PERD=0x0E,        //��������� ������ ����������
SET_PERD=0x0F         //���������� ������ ����������

} addn_packet_211_id;

//��� ���������� ������� � �������

typedef enum {

cMEAS_VAL=0x01,        //��������� ����������
cMEAS_VER=0x0E,        //������ ����������
cACK_ERR=0xFF,         //��������� �� ������

cGET_BAUD=0x11,        //��������� ��������������� ������
cGET_NAME=0x13,        //��������� ��� ����������
cGET_ZOFF=0x15,        //��������� �������� ����
cGET_SVER=0x1A,        //��������� ����� �������� ��
cGET_SERN=0x1B,        //��������� ��������� ����� ����������
cGET_TACT=0x1C,        //��������� ����� ������ ����������
cGET_PERD=0x1E,        //��������� ������ ����������

} ind3_common_request_type;

//�������� � ������ �� ������ �������� ������
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
