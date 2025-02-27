﻿//---------------------------------------------------------------------------

#ifndef Protocol_Modbus_RTUH
#define Protocol_Modbus_RTUH
//---------------------------------------------------------------------------
 #include "defs.h"
 #include "Protocol.h"
 #include <Modbus_tiny.h>

//---------------------------------------------------------------------------


class TProtocol_Modbus_RTU : public TProtocol
{

public:

	//TProtocol_Modbus_RTU();
	~TProtocol_Modbus_RTU();

public:

	void setProtocol(void) override;
	int request_curr_XY(BYTE addr, BYTE* buf, int* idx, int *exp_response_len, bool* exp_response_regular) override;
	int accept_response_curr_XY(BYTE addr, BYTE* buf, int* idx) override;

private:




};

#endif
