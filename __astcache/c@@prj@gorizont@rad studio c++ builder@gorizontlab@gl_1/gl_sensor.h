﻿//---------------------------------------------------------------------------

#ifndef GL_SensorH
#define GL_SensorH
//---------------------------------------------------------------------------
#include "defs.h"
#include "GL_List.h"

#include "Protocol_211.h"
#include "Protocol_AND3.h"
#include "Protocol_MODBUS_RTU.h"

#include "DataStream_IND3.h"

#include <list>

//---------------------------------------------------------------------------
#define SENSOR_TYPE_IND3_IND3       0
#define SENSOR_TYPE_IND3_AND3       1
#define SENSOR_TYPE_IND3_ASIN       2
#define SENSOR_TYPE_IND3_RTU        3
#define SENSOR_TYPE_AND3_AND3       4

#define SENSOR_TYPE_IND3       		0
#define SENSOR_TYPE_AND3       		1

#define SENSOR_TYPE_UNKN_UNKN       255

typedef struct {

TDateTime t;
double x;
double y;

} dt_sensor_data_record_s;

typedef struct {

	int err_tou;
	int err_crc;
	int err_wrt;
	long bytes_tx;
	long bytes_rx;
	long rep;
	double t_req;
	double t_req_mid;
	double t_req_total;
	long total_cnt;

} sns_st;

class TGLSensor
{

public:

	TGLSensor();
	TGLSensor(WideString nm, TTreeNode* nd, int nn);
	~TGLSensor();

public:

	WideString name;

	TTreeNode* node;
	int num;

	int plnum;
	int prnum;

public:

	void SetTree(TTreeView* t);
	void SetNode(TTreeNode* n);
	void SetPlnum(int n);
	void SetPrnum(int n);

	WideString GetName(void);
	BYTE GetAddr(void);
	void SetAddr(Byte ad);
	int ReDraw(TTreeNode* n, int plnum, int prnum, int snnum);

	//BYTE* getRX(void);
	//BYTE* getTX(void);
	//void clrTX(void);
	//int* getRXidx(void);
	//int* getTXidx(void);
	double get_rawX(void);
	double get_rawY(void);


private:

    WideString mark;

	TTreeView* tree;
	BYTE addr;

	double raw_X;
	double raw_Y;

	TProtocol* protocol;

	void set_sensor();

	int type;
	int protocol_type;
	int sensor_type;

	/*
	BYTE* rxbuf;
	BYTE* txbuf;
	int* rxidx;
    int* txidx;
	*/

	int uid;
	bool on;
	sns_st sn_state;


	//TDataStream* ss;
	TListItem* list_item;




public:

    int data_stream_rate;
	int data_stream_rate_cur;


    bool refresh_data_flag;

	std::list<TDataStream*> data_stream_list;

	//int request_curr_XY(BYTE** buf, int** len);
	int request_curr_XY(BYTE* buf, int* len, int *exp_response_len, bool* exp_response_regular);

	//int accept_response_curr_XY();
	int accept_response_curr_XY(BYTE* buf, int* idx);

	void subst(double x, double y);
	void set_uid(int id);
	int get_uid(void);
    void set_on(int n);
	int get_on(void);
	void set_type(int t);
	int get_type(void);
    void set_mark(TCHAR* mr);
	TCHAR* get_mark(void);
	void set_addr(BYTE a);
	BYTE get_addr(void);
	void set_name(TCHAR* nm);
	TCHAR* get_name(void);
	void reset(void);
	void update(TDateTime timestamp);
	sns_st* get_sn_state(void);
	TCHAR* get_str_ID(void);
	void set_list_item( TListItem *it);
	TListItem * get_list_item(void);
	void data_stream_setup(void);
	int get_protocol_type(void);
	void set_protocol_type(int pr);
	int get_sensor_type(void);
	void set_sensor_type(int pr);
	int init_sensor(void);
    bool stream_rate_enable(void);
};


#endif
