//---------------------------------------------------------------------------

#ifndef Protocol_AND3H
#define Protocol_AND3H

#include "defs.h"
#include "Protocol.h"

//---------------------------------------------------------------------------

typedef struct
{
  WORD  frst:1; // ������������
  WORD   fdr:1; // ���������� ������ �������
  WORD ft5dr:1; // ���������� ������ �����������
  WORD fthdr:1; // ���������� ������ �����������

  WORD   fte:1; // ������ ������ �������
  WORD   fce:1; // ������ CRC �������
  WORD   fre:1; // ������ ��������� �������
  WORD    r1:1; // ������

  WORD ft5te:1; // ������ ������ �����������
  WORD ft5re:1; // ������ ��������� �����������
  WORD fthre:1; // ������ ������� �����������
  WORD ftrme:1; // ������ ���������������

  WORD  fadr:1; // ���� ����� ������
  WORD  ftrm:1; // �������������� �������
  WORD    r2:1; // ������
  WORD    r3:1; // ������
} and3_SW;

typedef struct
{
  WORD nofir1:1;   // ��������� ������ 1
  WORD nofir2:1;   // ��������� ������ 2
  WORD noidle:1;   // ��������� IDLE
  WORD nod3:1;     // ��������� ��������

  WORD not5av:1;   // �� ��������� �� �����������
  WORD notrm:1;    // ��������� ��������������
  WORD nostack:1;  // �� ��������� ����� �������������
  WORD noadrdet:1; // �� ������������� ����� �������

  WORD noacmp:1;   // �� �������������� ��������� �� 0 �������
  WORD nofir3:1;   // ��������� ������ 3
  WORD nopwr:1;    // ��������� ������� �������� ��� ��
  WORD nometeo:1;  // ��������� ������������

   // WORD reserved:4;
  WORD  tostart:1;  // ������ �� ��������      // !!! 1.202
  WORD    br115:1;  // �������� 115200         // !!! 1.202
  WORD     to10:1;  // ������� 10 �� (��� 5 ��)// !!! 1.202
  WORD reserved:1;
} and3_MODE;

typedef struct {

	BYTE addr;
	BYTE cmd_code;
	BYTE cmd_byte1;
	BYTE cmd_byte2;
	int cmd_len;

} and3_cmd;


typedef struct {

float X;
float Y;
short T;
and3_SW sw;
DWORD pn;
and3_MODE md;

}and3_res_float;


#define AND3_CMD_GETALL 		201
#define AND3_LEN_GETALL 		18
#define AND3_DAT1_GETALL 		0
#define AND3_DAT2_GETALL 		0

#define AND3_LEN_CMD            6

class TProtocol_AND3 : public TProtocol {

public:

	TProtocol_AND3();
	~TProtocol_AND3();

	int RequestMeasureResult(unsigned char addr, unsigned char* buf, int* len);
	int CheckPacket(unsigned char addr, unsigned char* buf, int* len);
	int AcceptSensorMeasVal(BYTE* buf,double* x, double* y, BYTE* unitx, BYTE* unity);

private:


	and3_cmd cmd;

};


#endif
