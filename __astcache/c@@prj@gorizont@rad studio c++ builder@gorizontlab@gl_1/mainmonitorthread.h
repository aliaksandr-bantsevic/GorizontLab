//---------------------------------------------------------------------------

#ifndef MainMonitorThreadH
#define MainMonitorThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "GL_System.h"
#include "defs.h"
//---------------------------------------------------------------------------
class TMainMonitorThread : public TThread
{
protected:
	void __fastcall Execute();
public:
	__fastcall TMainMonitorThread(bool CreateSuspended, TGLSystem* sys);

private:

	bool cycle_flag;
	bool suspended;
	bool exit_flag;

	TGLSystem* system;

public:

	void start();
	void suspend();
	void resume();
	void exit();

};
//---------------------------------------------------------------------------
#endif
