//---------------------------------------------------------------------------

#ifndef ChartThread_badH
#define ChartThread_badH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>

#include <tchar.h>
#include <VCLTee.Chart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.Series.hpp>

//#include "DataStream.h"
//---------------------------------------------------------------------------
class TChartThread : public TThread
{
protected:
	void __fastcall Execute();
public:
	__fastcall TChartThread(bool CreateSuspended);


public:

	TChart* chart;
	bool exit_flag;
	bool draw_flag;
	//TDataStream* ds;

public:
	  /*
void init_chart();
void draw(void);
void draw_chart(void);
void accept_data(double* d, int len);
void exit(void);
//void set_data_stream(TDataStream* d);   */

//---------------------------------------------------------------------------
#endif
