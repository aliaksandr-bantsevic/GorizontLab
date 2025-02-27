//---------------------------------------------------------------------------

#ifndef DataStreamH
#define DataStreamH
//---------------------------------------------------------------------------

#include "defs.h"
//#include "GL_Sensor.h"

#include <list>

#define DATA_LOST_CNT_THRESHOLD         5
#define DATA_LOST_DBL_CONSTANT          999999999.99

typedef struct {

	WideString name;
	double val;

} calc_param;

class TDataStream
{

public:

	TDataStream();
	TDataStream(TCHAR* nm,TCHAR* psfx, TCHAR* mark, double* raw, int* uid, TCHAR* units);
	~TDataStream();

	virtual void calculate(void) = 0;

public:

	WideString name;
	double* raw;
	double val;

	WideString mark;
	WideString units;
	TDateTime last_update_timestamp;

protected:


	int* uid;

	//TGLSensor* sensor;


	int data_lost_threshold;
	int data_lost_cnt;
	std::list<calc_param> calc_param_list;


public:

	void view(TListView* list);
	void update(TDateTime timestamp);

	TListItem *item;

};


#endif
