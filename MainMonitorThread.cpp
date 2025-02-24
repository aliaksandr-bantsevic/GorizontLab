//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "MainMonitorThread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TMainMonitorThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

extern TDateTime g_monitor_second_timer;

__fastcall TMainMonitorThread::TMainMonitorThread(bool CreateSuspended, TGLSystem* sys)
	: TThread(CreateSuspended)
{
	system = sys;
	cycle_flag = false;
	suspended = true;
	exit_flag = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainMonitorThread::Execute()
{
	NameThreadForDebugging(System::String(L"MainMonitorThread"));
	//---- Place thread code here ----
	while (exit_flag == false)
	{
		TDateTime t;
		WORD h,m,s,ms;

		while (1)
		{
			t = Now();
			t.DecodeTime(&h,&m,&s,&ms);
			if (ms == 0)
			{
			   g_monitor_second_timer = t;
			   cycle_flag = true;
			   break;
			}
        }

		while (suspended)
		{
			Sleep(10);
		}

		if ((cycle_flag)&&(exit_flag == false))
		{
			cycle_flag = false;
			system->cycle();
			system->update_view_ports_status();
			system->update_view_sensors_status();
            system->update_view_data_status();
		}

		Sleep(1);
	}
}
//---------------------------------------------------------------------------
void TMainMonitorThread::start()
{
	cycle_flag = true;
}

void TMainMonitorThread::suspend()
{
	suspended = true;
}

void TMainMonitorThread::resume()
{
	suspended = false;
}

void TMainMonitorThread::exit()
{
	exit_flag = true;
}
