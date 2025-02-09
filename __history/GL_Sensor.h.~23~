//---------------------------------------------------------------------------

#ifndef GL_SensorH
#define GL_SensorH
//---------------------------------------------------------------------------
#include "defs.h"
#include "GL_List.h"

#include "Protocol_211.h"
#include "Protocol_and3.h"
//---------------------------------------------------------------------------
#define SENSOR_TYPE_IND3_IND3       0
#define SENSOR_TYPE_IND3_AND3       1
#define SENSOR_TYPE_IND3_ASIN       2
#define SENSOR_TYPE_AND3_AND3       3

#define SENSOR_TYPE_AND3_AND3       3


#define SENSOR_TYPE_UNKN_UNKN       255

typedef struct {

TDateTime t;
double x;
double y;

} dt_sensor_data_record_s;

class TGLSensor
{

public:

	TGLSensor();
	TGLSensor(WideString nm, TTreeNode* nd, int nn);
	~TGLSensor();

public:

	WideString name;
	WideString mark;
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

	BYTE* getRX(void);
	BYTE* getTX(void);
	void clrTX(void);
	int* getRXidx(void);
	int* getTXidx(void);
	double get_rawX(void);
	double get_rawY(void);

private:

	TTreeView* tree;
	BYTE addr;

	double raw_X;
	double raw_Y;

	TProtocol* protocol;

	void set_sensor(int type);

	int type;

	BYTE* rxbuf;
	BYTE* txbuf;
	int* rxidx;
    int* txidx;

public:

	int request_curr_XY(BYTE* buf, int* len);
	int accept_response_curr_XY();
    void subst(double x, double y);

};


#endif
