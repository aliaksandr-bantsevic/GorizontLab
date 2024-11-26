//---------------------------------------------------------------------------

#pragma hdrstop

#include "Time.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TDateTime  g_current_time = 0.;
WideString s_current_time = "";

WideString GetCurrentTimeStr(void)
{
	WideString stime(L"");
	TDateTime t = Now();
	stime = t.FormatString(L"dd-mm-yyyy   hh:mm:ss");
	s_current_time = stime;
	return stime;
}
