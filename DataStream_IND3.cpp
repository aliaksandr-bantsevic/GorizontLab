//---------------------------------------------------------------------------

#pragma hdrstop

#include "DataStream_IND3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TDataStream_IND3::TDataStream_IND3(TCHAR* nm,TCHAR* psfx, TCHAR* mark, double* raw, int* uid, TCHAR* units) : TDataStream(nm,psfx, mark, raw, uid, units)
{


}

TDataStream_IND3::~TDataStream_IND3()
{

}

void TDataStream_IND3::calculate(void)
{
    val = *raw;
}
