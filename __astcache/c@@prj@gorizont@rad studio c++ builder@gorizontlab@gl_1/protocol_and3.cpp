//---------------------------------------------------------------------------

#pragma hdrstop

#include "Protocol.h"
#include "Protocol_AND3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TProtocol_and3::~TProtocol_and3()
{

}

void TProtocol_and3::setProtocol(void)
{
   protocol_type = PROTOCOL_TYPE_AND3;
}

int TProtocol_and3::request_curr_XY(BYTE addr, BYTE* buf, int* idx, int *exp_response_len, bool* exp_response_regular)
{
	//expectedf response len is regular 22 bytes
	*exp_response_len = 22;
	*exp_response_regular = true;

   return RequestMeasureResult(addr, buf, idx);
}

int  TProtocol_and3::RequestMeasureResult(unsigned char addr, unsigned char* buf, int* len)
{

	int idx = 0;

	buf[idx] = addr; idx++;
	buf[idx] = AND3_CMD_GETALL; idx++;
	buf[idx] = AND3_DAT1_GETALL; idx++;
	buf[idx] = AND3_DAT2_GETALL; idx++;

	*((WORD*)(buf+2+2)) = CRC16CCITT(2+2, buf);

	*len = AND3_LEN_CMD;

	return 0;
}

int TProtocol_and3::accept_response_curr_XY(BYTE addr, BYTE* buf, int* idx)
{
	return AcceptSensorMeasVal(addr, buf, idx);
}

int TProtocol_and3::AcceptSensorMeasVal(BYTE addr, BYTE* buf, int* idx)
{

	if (CheckPacket(addr, buf, idx) != 0)
	{
		return -1; //bad packet
	}

	and3_res_float* res;
	res = (and3_res_float*) (buf+2);

	 if (res->sw.fdr)
	 {
		  *raw_X = (double) res->X;
		  *raw_Y = (double) res->Y;
	 }
	 else
	 {
		 return -2;// no active
     }

   //*unitx = 0;
   //*unity = 0;

   return 0;
}

int TProtocol_and3::CheckPacket(unsigned char addr, unsigned char* buf, int* len)
{
	BYTE bb[100];

	memcpy(bb, buf, 100);

	if (buf[0] != addr) return -1; //wrong address

	WORD crc16 = *(WORD*)(buf+(*len)-2);
	WORD crc16_rc = CRC16CCITT((unsigned short)((*len)-2),buf) ;

	if(*((WORD*)(buf+(*len)-2))!=CRC16CCITT((unsigned short)((*len)-2),buf)) return -2; //wrong crc

	return 0;
}
