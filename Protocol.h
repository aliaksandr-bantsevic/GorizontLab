//---------------------------------------------------------------------------

#ifndef ProtocolH
#define ProtocolH
//---------------------------------------------------------------------------

#define PROTOCOL_TYPE_IND3          0
#define PROTOCOL_TYPE_AND3          1
#define PROTOCOL_TYPE_MODBUS_RTU    2
#define PROTOCOL_TYPE_ASIN          3

#define PROTOCOL_TYPE_NONE          255

class TProtocol {

public:

		TProtocol();

		//~TProtocol();

private:


protected:

	   virtual void setProtocol() = 0;

	   //BYTE bufrx[1024];
	   //BYTE buftx[1024];
	   //int rx_idx;
	   //int tx_idx;

	   double* raw_X;
	   double* raw_Y;

	   int protocol_type;

public:

	   //virtual int request_curr_XY(BYTE addr) = 0;
	   virtual int request_curr_XY(BYTE addr, BYTE* buf, int* idx, int *exp_response_len, bool* exp_response_regular) = 0;

	   //virtual int accept_response_curr_XY(BYTE addr) =  0;
	   virtual int accept_response_curr_XY(BYTE addr, BYTE* buf, int* idx) =  0;

	   //BYTE* getRX(void);
	   //BYTE* getTX(void);
	   void clear(void);
	   //void clear_rx(void);
	   //void clear_tx(void);
	   //int* getRXidx(void);
	   //int* getTXidx(void);
	   void	set_raw_X(double* x);
	   void	set_raw_Y(double* y);

};

#endif
