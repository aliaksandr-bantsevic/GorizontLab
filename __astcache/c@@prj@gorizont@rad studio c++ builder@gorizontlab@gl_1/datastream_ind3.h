﻿//---------------------------------------------------------------------------

#ifndef DataStream_IND3H
#define DataStream_IND3H

#include "DataStream.h"

//---------------------------------------------------------------------------

class TDataStream_IND3 : public TDataStream
{

public:

	TDataStream_IND3(TCHAR* nm,TCHAR* psfx, TCHAR* mark, double* raw, int* uid, TCHAR* units);
	~TDataStream_IND3();

	void calculate(void) override;

};


#endif


