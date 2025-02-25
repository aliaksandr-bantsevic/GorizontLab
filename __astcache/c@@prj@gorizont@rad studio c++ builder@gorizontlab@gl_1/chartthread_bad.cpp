//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "ChartThread_bad.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TChartThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TChartThread::TChartThread(bool CreateSuspended) : TThread(CreateSuspended)
{
	//chart = c;
	//exit_flag = false;
	//draw_flag = false;
}
//---------------------------------------------------------------------------
void __fastcall TChartThread::Execute()
{
	NameThreadForDebugging(System::String(L"TChartThread"));
	//---- Place thread code here ----
}
//---------------------------------------------------------------------------

/*
void TChartThread::init_chart()
{


}

void TChartThread::draw_chart(void)
{


}

void TChartThread::accept_data(double* d, int len)
{


}

void TChartThread::exit(void)
{
	exit_flag = true;
}

void TChartThread::draw(void)
{
	draw_flag = true;
}

//void TChartThread::set_data_stream(TDataStream* d)
//{
//	ds = d;
//}

*/
