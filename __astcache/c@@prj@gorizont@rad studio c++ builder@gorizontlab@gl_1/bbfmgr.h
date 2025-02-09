//---------------------------------------------------------------------------

#ifndef BBFMgrH
#define BBFMgrH

#include "GL_Sensor.h"
#include "defs.h"
//---------------------------------------------------------------------------


class TBBFMgr
{

public:

   TBBFMgr();
   ~TBBFMgr();

public:

	int save_sensor_data_s(TCHAR* bdir, TDateTime t, TGLSensor* sn);
    std::list<dt_sensor_data_record_s> read_sensor_data_s(TCHAR* bdir, TGLSensor* sn, TDateTime t1, TDateTime t2);


};
















#endif
