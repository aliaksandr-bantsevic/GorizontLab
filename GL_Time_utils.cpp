//---------------------------------------------------------------------------

#pragma hdrstop

#include "GL_Time_utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TDateTime  g_current_time = 0.;
WideString s_current_time = "";
WideString s_global_timer = "";

WideString GetCurrentTimeStr(void)
{
	WideString stime(L"");
	TDateTime t = Now();
	stime = t.FormatString(L"dd-mm-yyyy   hh:mm:ss");
	s_current_time = stime;
	return stime;
}

WideString GetGlobalSecondTimerStr(TDateTime* t)
{
	*t += T_ONE_SEC;

	WideString stime(L"");

	unsigned short h,m,s,ms;

	t->DecodeTime((unsigned short*)&h,(unsigned short*)&m,(unsigned short*)&s,(unsigned short*)&ms);

	stime.printf(L"%d:%02d:%02d", h,m,s);

	s_global_timer = stime;
	return stime;
}

