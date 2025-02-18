//---------------------------------------------------------------------------

#ifndef COMPortH
#define COMPortH
//---------------------------------------------------------------------------
#include <windows.h>
#include <Classes.hpp>
//#include "ScktComp.hpp"

#include "defs.h"
//---------------------------------------------------------------------------

#define PORT_TYPE_COM       0
#define PORT_TYPE_TCP       1

#define PORT_TYPE_UNK       255

typedef struct
{

  int p;
  bool on;
  bool run;
  bool res;
  int cmd;
  int baud;

} SCOMState;

class TCOMPort
{

public:

	TCOMPort();
	TCOMPort(int type = PORT_TYPE_COM, DWORD baud = 115200);
	~TCOMPort();

private:

	int type;
	COMMTIMEOUTS to;

	BYTE* buf4100;
	DCB  dcb;
	HANDLE h;
	SCOMState st;
    long int br;long int bw;DWORD lbr;DWORD er;DWORD ew;DWORD ec;DWORD eo;DWORD lerr;DWORD cmdnum;
	DWORD Nr;
	DWORD Nw;
	double TmOpen,TmClose;

public:

	void InitDCB(DCB* PortDCB);
	bool  Open(bool clr_err,DWORD port=255);
	bool  Close(void);
	bool  PortNWrite(DWORD n,BYTE *b);
	bool  PortNRead(DWORD n,BYTE *b);
	bool  SetTimeouts(DWORD rtime,DWORD wtime);
	bool  Purge();
	void  PurgeCom();
	void  Clr();
	bool  CSend(int n,BYTE *pb,DWORD rtime,DWORD wtime);
	bool  CReceive(int n,BYTE *pb,DWORD rtime,DWORD wtime);
	void set_baud(DWORD b);

public:

	void set_buf4100(BYTE* buf);
	//bool TCOMPort::PortNWrite(DWORD n,BYTE *b);




};
#endif
