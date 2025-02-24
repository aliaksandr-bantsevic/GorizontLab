//---------------------------------------------------------------------------

#ifndef Modbus_tinyH
#define Modbus_tinyH
//---------------------------------------------------------------------------


typedef enum {

RTU_CMD_Read_Coil_Status 			= 0x01, //	Чтение DO	Read Coil Status	Дискретное	Чтение
RTU_CMD_Read_Input_Status 			= 0x02, //	Чтение DI	Read Input Status	Дискретное	Чтение
RTU_CMD_Read_Holding_Register 		= 0x03, //	Чтение AO	Read Holding Registers	16 битное	Чтение
RTU_CMD_Read_Input_Registers 		= 0x04, //	Чтение AI	Read Input Registers	16 битное	Чтение
RTU_CMD_Force_Single_Coil 			= 0x05, //	Запись одного DO	Force Single Coil	Дискретное	Запись
RTU_CMD_Preset_Single_Register 		= 0x06, //	Запись одного AO	Preset Single Register	16 битное	Запись
RTU_CMD_Force_Multiple_Coils 		= 0x0F, //	Запись нескольких DO	Force Multiple Coils	Дискретное	Запись
RTU_CMD_Preset_Multiple_Registers 	= 0x10, //    Запись нескольких AO	Preset Multiple Registers	16 битное	Запись

} modbus_rtu_fc;

#define Discrete_Output_Coils 			1		//1-9999	0000 до 270E	Чтение-запись	Discrete Output Coils	DO
#define Discrete_Input_Contacts 		10001	//10001-19999	0000 до 270E	Чтение	Discrete Input Contacts	DI
#define Analog_Input_Registers 			30001	//30001-39999	0000 до 270E	Чтение	Analog Input Registers	AI
#define Analog_Output_Holding_Registers 40001	//40001-49999	0000 до 270E	Чтение-запись	Analog Output Holding Registers	AO

typedef struct {

	BYTE addr;
	BYTE fc;
	WORD start_reg_addr;
	WORD numb_regs;

} modbus_rtu_cmd;

extern WORD crc16(uint8_t *buffer, uint16_t buffer_length);
extern int request_modbus_rtu (modbus_rtu_cmd cmd, BYTE* buf, int* len);



#endif
