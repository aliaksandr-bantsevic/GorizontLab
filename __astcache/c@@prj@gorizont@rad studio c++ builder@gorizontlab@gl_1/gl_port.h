//---------------------------------------------------------------------------

#ifndef GL_PortH
#define GL_PortH
//---------------------------------------------------------------------------
#include "defs.h"
#include "GL_List.h"

#include "COMPort.h"

class TGLSensor;
//---------------------------------------------------------------------------

#define PORT_TYPE_COM      0
#define PORT_TYPE_TCP      1

#define OPERATION_ATTEMPS_THRESHOLD_DEFAULT     3

typedef struct {

	DWORD delay_cmd_exec;
	DWORD delay_addr_change;
	DWORD delay_debug;
	DWORD delay_default;

	DWORD tout_rd;
	DWORD tout_wr;

} delays;

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

} prt_st;

class TGLPort
{

public:

	TGLPort();
	TGLPort(WideString nm, TTreeNode* nd, int nn, int comtype = PORT_TYPE_COM);
	~TGLPort();

	WideString name;
	TTreeNode* node;
	int num;
	int plnum;

public:

	TGLList<TGLSensor> sensor_list;

public:

	int SetBaud(int b);
	TGLSensor* add_sensor(WideString nm, int plnum, int uid);
	void SetTree(TTreeView* t);
	void SetNode(TTreeNode* n);
	TTreeNode* GetNode(void);

	void SetPlnum(int n);
	int ReDraw(TTreeNode* n, int plnum, int prnum);
	void syspend_cycle ();
	void start_cycle ();

	//int transact_request_XY(TGLSensor* sn);
	int transact_request_XY_ex(TGLSensor* sn);

	bool is_suspended ();
	int cycle ();
	bool is_run_engine_suspended ();
	void run_engine_suspend ();
	void run_engine_resume ();
	void set_mark(TCHAR* mr);
	TCHAR* get_mark(void);
	void set_sys_port_num(DWORD n);
	DWORD get_sys_port_num(void);
	void set_baud(DWORD b);
	DWORD get_baud(void);
	void set_on(int n);
	int get_on(void);
	void set_type(int t);
	int get_type(void);
	void set_com(void);
	prt_st* get_state(void);
	void set_list_item( TListItem *it);
	TCHAR* get_str_ID(void);
	void set_is_open_in_cycle(bool op);
	bool get_is_open_in_cycle(void);
	//TCHAR* get_mark(void);
	TListItem* get_list_item(void);
    bool sensor_exist (BYTE ad);

public:

delays delay_set;

private:

	TTreeView* tree;
	TGLSensor* sns;


	TCOMPort* com;

	bool cycle_syspend_flag;
	DWORD sys_port_num;
	bool suspend_run_engine;
	bool on;
	WideString mark;
	DWORD baud;
	int type;

	BYTE buftx[8448];
	int txidx;
	BYTE bufrx[8448];
	int rxidx;
	int operation_attempts_tgreshold;
	prt_st port_state;
	TListItem* list_item;
	bool is_open_in_cycle;


};

#endif
