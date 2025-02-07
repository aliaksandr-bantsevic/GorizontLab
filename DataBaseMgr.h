//---------------------------------------------------------------------------

#ifndef DataBaseMgrH
#define DataBaseMgrH
//---------------------------------------------------------------------------

#include "sqlite3.h"
//#include "sqlite3ext.h"

#include "defs.h"
#include "GL_Sensor.h"


class TDataBaseMgr
{

public:
	 TDataBaseMgr();
	 ~TDataBaseMgr();


private:

	sqlite3* l_sql_db;
	bool is_open;

public:

	int open_base(TCHAR* path);
	bool is_base_open(void);
	int save_sensor_data_s(TDateTime t, TGLSensor* sn);
	int BEGIN_TRANSACTION (void);
	int END_TRANSACTION (void);
	std::list<dt_sensor_data_record_s> read_sensor_data_s(TGLSensor* sn, TDateTime t1, TDateTime t2);


};






#endif


