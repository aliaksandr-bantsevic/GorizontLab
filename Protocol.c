//---------------------------------------------------------------------------

#pragma hdrstop

#include "Protocol.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TProtocol::TProtocol()
{
   clear();
   protocol_type = PROTOCOL_TYPE_NONE; //unknown
   setProtocol();
}

TProtocol::~TProtocol()
{

}

void TProtocol::clear(void)
{
	memset(bufrx,0,1024);
	memset(buftx,0,1024);
	rx_idx = 0;
	tx_idx = 0;
	raw_X = 0;
	raw_Y = 0;
}

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
	return rxbuf;
}

BYTE* TProtocol::geTX(void)
{
	return txbuf;
}



