//---------------------------------------------------------------------------

#pragma hdrstop

#include "Protocol.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TProtocol::TProtocol()
{
   clear();
   protocol_type = PROTOCOL_TYPE_NONE; //unknown
   //setProtocol();
}

//TProtocol::~TProtocol()
//{
//
//}

void TProtocol::clear(void)
{
	//memset(bufrx,0,1024);
	//memset(buftx,0,1024);
	//rx_idx = 0;
	//tx_idx = 0;
	//raw_X = 0;
	//raw_Y = 0;
}

/*
void TProtocol::clear_rx(void)
{
	memset(bufrx,0,1024);
	rx_idx = 0;
	raw_X = 0;
	raw_Y = 0;
}

void TProtocol::clear_tx(void)
{
	memset(buftx,0,1024);
	tx_idx = 0;
}

BYTE* TProtocol::getRX(void)
{
	return bufrx;
}

BYTE* TProtocol::getTX(void)
{
	return buftx;
}

int* TProtocol::getRXidx(void)
{
	return &rx_idx;
}

int* TProtocol::getTXidx(void)
{
	return &tx_idx;
}
*/

void TProtocol::set_raw_X(double* x)
{
	raw_X = x;
}

void TProtocol::set_raw_Y(double* y)
{
	raw_Y = y;
}

