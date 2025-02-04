//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "PortProcThread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TPortProcThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

class TGLSensor;
class TProtocol;

__fastcall TPortProcThread::TPortProcThread(bool CreateSuspended, TGLPort* prt)
	: TThread(CreateSuspended)
{
   port = prt;
   suspend();
}
//---------------------------------------------------------------------------
void __fastcall TPortProcThread::Execute()
{
	WideString s;
	s.printf(L"port %d.%d", port->plnum, port->num);

	//NameThreadForDebugging(System::String(L"PortProc"));
	NameThreadForDebugging(System::String(s.c_bstr()));
	//---- Place thread code here ----

	while (exit_flag == false)
	{
	   while ((suspend_flag == true)||(port->is_run_engine_suspended() == true))
	   {
			Sleep(10);
	   }

	   port->cycle();

	   Sleep(10);
	}

}
//---------------------------------------------------------------------------

void TPortProcThread::start()
{
	suspend_flag = false;
	exit_flag = false;
	Execute();
}

void TPortProcThread::suspend()
{
   suspend_flag = true;
}

void TPortProcThread::resume()
{
   suspend_flag = false;
}

void TPortProcThread::exit()
{
   exit_flag = true;
}

