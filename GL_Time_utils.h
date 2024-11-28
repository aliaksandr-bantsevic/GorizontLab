//---------------------------------------------------------------------------

#ifndef TimeH
#define TimeH
//---------------------------------------------------------------------------
#endif

#define T_ONE_DAY       (1.)
#define T_ONE_HOUR      (1./24.)
#define T_ONE_MIN       (1./(24.*60.))
#define T_ONE_SEC       (1./(24.*60.*60.))
#define T_ONE_MSEC      ((1./(24.*60.*60.))/1000)
#define T_ONE_DSEC      ((1./(24.*60.*60.))/10)

extern TDateTime  g_current_time;
extern WideString s_current_time;
extern WideString GetCurrentTimeStr(void);
extern WideString GetGlobalSecondTimerStr(TDateTime* t);
