//---------------------------------------------------------------------------

#pragma hdrstop

#include "DataStream.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TDataStream::TDataStream()
{

}

TDataStream::TDataStream(TCHAR* nm,TCHAR* psfx, TCHAR* mark, double* raw, int* uid, TCHAR* units)
{
  this->name.printf(L"[%03d] %s %s", *uid, nm, psfx);
  this->uid = uid;
  this->mark = mark;
  this->raw = raw;
  this->units = units;

  item = NULL;

  last_update_timestamp = 0.;
  *raw = DATA_LOST_DBL_CONSTANT;
  val =  DATA_LOST_DBL_CONSTANT;

}

TDataStream::~TDataStream()
{

}

void TDataStream::update(TDateTime timestamp)
{
   last_update_timestamp = timestamp;

   if (*raw == DATA_LOST_DBL_CONSTANT)
   {
		if (data_lost_cnt < data_lost_threshold)
		{
		   data_lost_cnt++;
		}
		else
		{
			val = DATA_LOST_DBL_CONSTANT;
        }
   }
   else
   {
	   data_lost_cnt = 0;
	   this->calculate();
   }
}