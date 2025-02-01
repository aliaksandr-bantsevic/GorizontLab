//---------------------------------------------------------------------------

#pragma hdrstop

#include "Protocol.h"
#include "Protocol_and3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void TProtocol_and3::setProtocol(void)
{
   protocol_type = PROTOCOL_TYPE_AND3;
}

int TProtocol_and3::request_curr_XY(BYTE addr)
{
   RequestMeasureResult(addr, buftx, &tx_idx);
}

int  TProtocol_and3::RequestMeasureResult(unsigned char addr, unsigned char* buf, int* len)
{

	int idx = 0;

	buf[idx] = addr; idx++;
	buf[idx] = AND3_CMD_GETALL; idx++;
	buf[idx] = AND3_DAT1_GETALL; idx++;
	buf[idx] = AND3_DAT2_GETALL; idx++;

	*((WORD*)(buf+2+2)) = CRC16CCITT(2+2,buf);

	*len = AND3_LEN_CMD;

	return 0;
}

int TProtocol_and3::accept_response_curr_XY(BYTE addr)
{
	AcceptSensorMeasVal(addr, bufrx, &raw_X, &raw_Y);
}

int TProtocol_and3::AcceptSensorMeasVal(BYTE addr, BYTE* buf, double* x, double* y)
{

	if (CheckPacket(addr, bufrx, &rx_idx) != 0)
	{
		return -1; //bad packet
	}

	and3_res_float* res;
	res = (and3_res_float*) (buf+2);

	 if (res->sw.fdr)
	 {
		  *x= (double) res->X;
		  *y= (double) res->Y;
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
	if (buf[0] != addr) return -1; //wrong address

	if(*((WORD*)(buf+2+*len))!=CRC16CCITT((unsigned short)(2+*len),buf)) return -2; //wrong crc

	return 0;
}
