//---------------------------------------------------------------------------

#ifndef Modbus_tinyH
#define Modbus_tinyH
//---------------------------------------------------------------------------


typedef enum {

RTU_CMD_Read_Coil_Status 			= 0x01, //	������ DO	Read Coil Status	����������	������
RTU_CMD_Read_Input_Status 			= 0x02, //	������ DI	Read Input Status	����������	������
RTU_CMD_Read_Holding_Register 		= 0x03, //	������ AO	Read Holding Registers	16 ������	������
RTU_CMD_Read_Input_Registers 		= 0x04, //	������ AI	Read Input Registers	16 ������	������
RTU_CMD_Force_Single_Coil 			= 0x05, //	������ ������ DO	Force Single Coil	����������	������
RTU_CMD_Preset_Single_Register 		= 0x06, //	������ ������ AO	Preset Single Register	16 ������	������
RTU_CMD_Force_Multiple_Coils 		= 0x0F, //	������ ���������� DO	Force Multiple Coils	����������	������
RTU_CMD_Preset_Multiple_Registers 	= 0x10, //    ������ ���������� AO	Preset Multiple Registers	16 ������	������

} modbus_rtu_fc;

#define Discrete_Output_Coils 			1		//1-9999	0000 �� 270E	������-������	Discrete Output Coils	DO
#define Discrete_Input_Contacts 		10001	//10001-19999	0000 �� 270E	������	Discrete Input Contacts	DI
#define Analog_Input_Registers 			30001	//30001-39999	0000 �� 270E	������	Analog Input Registers	AI
#define Analog_Output_Holding_Registers 40001	//40001-49999	0000 �� 270E	������-������	Analog Output Holding Registers	AO

typedef struct {

	BYTE addr;
	BYTE fc;
	WORD start_reg_addr;
	WORD numb_regs;

} modbus_rtu_cmd;

extern WORD crc16(uint8_t *buffer, uint16_t buffer_length);
extern int request_modbus_rtu (modbus_rtu_cmd cmd, BYTE* buf, int* len);



#endif
