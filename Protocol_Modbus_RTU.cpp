//---------------------------------------------------------------------------

#pragma hdrstop

#include "Protocol_Modbus_RTU.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TProtocol_Modbus_RTU::~TProtocol_Modbus_RTU()
{

}

void TProtocol_Modbus_RTU::setProtocol(void)
{
   protocol_type = PROTOCOL_TYPE_MODBUS_RTU;
}

int TProtocol_Modbus_RTU::request_curr_XY(BYTE addr, BYTE* buf, int* idx, int *exp_response_len, bool* exp_response_regular)
{
	//expectedf response len is regular 22 bytes
	*exp_response_len = 13;
	*exp_response_regular = true;

	modbus_rtu_cmd cmd;

#ifdef DEBUG_SUBST_SNS_ADDR
	cmd.addr = DEBUG_SUBST_SNS_ADDR;
#else
		cmd.addr = addr;
#endif
	cmd.fc = RTU_CMD_Read_Input_Registers;
	cmd.start_reg_addr = 0;
	cmd.numb_regs = 4;

	return request_modbus_rtu (cmd, buf, idx);
}

int TProtocol_Modbus_RTU::accept_response_curr_XY(BYTE addr, BYTE* buf, int* idx)
{

	if (*idx != 13)
	{
		return -1; //wrong len
	}

#ifdef DEBUG_SUBST_SNS_ADDR
	if (buf[0] != DEBUG_SUBST_SNS_ADDR)
#else
	if (buf[0] != addr)
#endif
	{
		return -2; //wrong addr
	}



	WORD crc = 0;

	crc = crc16(buf, *idx - 2);

	BYTE crc_l = (BYTE)crc&0x00ff;
	BYTE crc_h = (BYTE)((crc&0xff00)>>8);

		BYTE bb[100];



	int ll = crc_l;
	int hh = crc_h;



	if ((crc_l != buf [*idx - 1]) || (crc_h != buf [*idx - 2]))
	{
		return -3; //wrong crc
	}

	if (buf[1] != RTU_CMD_Read_Input_Registers)
	{
		return -4; //wrong fc
	}

	if (buf[2] != 8)
	{
		return -5; //wrong len
	}

	int y32 = *((int*)(&buf[3]));

		memcpy(bb, buf, 100);
    uint8_t bytes[] = {0xFF, 0xF9, 0xAD, 0x70};

    int32_t value1 = (static_cast<int32_t>(bytes[0]) << 24) |
                    (static_cast<int32_t>(bytes[1]) << 16) |
                    (static_cast<int32_t>(bytes[2]) << 8) |
					 static_cast<int32_t>(bytes[3]);

	int32_t valueY = (static_cast<int32_t>(buf[3]) << 24) |
					(static_cast<int32_t>(buf[4]) << 16) |
					(static_cast<int32_t>(buf[5]) << 8) |
					 static_cast<int32_t>(buf[6]);

	*raw_Y = valueY/(double)1000.;

	int x32 = *((int*)(&buf[7]));

	int32_t		valueX = (static_cast<int32_t>(buf[7]) << 24) |
					(static_cast<int32_t>(buf[8]) << 16) |
					(static_cast<int32_t>(buf[9]) << 8) |
					 static_cast<int32_t>(buf[10]);

	*raw_X = valueX/(double)1000.;

	return 0;
}
