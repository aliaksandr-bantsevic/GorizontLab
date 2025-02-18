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

typedef struct {

	DWORD delay_cmd_exec;
	DWORD delay_addr_change;
	DWORD delay_debug;
	DWORD delay_default;

} delays;

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

	int transact_request_XY(TGLSensor* sn);
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

private:

	TTreeView* tree;
	TGLSensor* sns;
	delays delay_set;

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

};

#endif
