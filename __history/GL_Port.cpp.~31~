//---------------------------------------------------------------------------

#pragma hdrstop
#include "GL_Port.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#include "GL_Sensor.h"
//---------------------------------------------------------------------------
TGLPort::TGLPort()
{
	 name = L"New Port";
	 mark = L"...";
	 node = NULL;
	 num = 0;

	 com = new TCOMPort(115200);
}

TGLPort::TGLPort(WideString nm, TTreeNode* nd, int nn, int comtype)
{
	name = nm;
	mark = L"...";
	node = NULL;
	num = nn;

	delay_set.delay_debug = 100;
	delay_set.delay_default = 10;
	delay_set.delay_addr_change = 20;
	delay_set.delay_cmd_exec = 20;

	com = new TCOMPort(115200);

	suspend_run_engine = false;
	on = false;
	mark = L"Ïîğò îïğîñà äàò÷èêîâ";
	sys_port_num = 0;
	baud = 115200;
	type = PORT_TYPE_COM;
}

TGLPort::~TGLPort()
{

}

int TGLPort::SetBaud(int b)
{
	//baud = b;

    return 0;
}

TGLSensor* TGLPort::add_sensor(WideString nm, int plnum, int uid)
{
	int nmb = sensor_list.count() + 1;
	sns = new TGLSensor(nm, NULL, nmb);

	if (sensor_list.add(sns) != 0)
	{
		delete sns;
		return NULL;  //fail to add the port
	}
	else
	{
		sns->set_uid(uid);

		WideString ss;
		ss.printf(L"[%03d] %d.%d.%d [", uid, plnum, num, nmb);
		ss = ss+nm;
		ss = ss+L"]";
		TTreeNode* ndd = tree->Items->AddChild(node, ss); ndd->ImageIndex = 3; ndd->SelectedIndex = 3;
		node->Expand(true);

		sns->SetTree(tree);
		sns->SetNode(ndd);

		sns->SetPlnum(plnum);
		sns->SetPrnum(num);

		return sns;
	}
}

void TGLPort::SetTree(TTreeView* t)
{
	tree = t;
}

void TGLPort::SetNode(TTreeNode* n)
{
	node = n;
}

void TGLPort::SetPlnum(int n)
{
	plnum = n;
}

int TGLPort::ReDraw(TTreeNode* n, int plnum, int prnum)
{
	WideString ss;
	ss.printf(L"%d.%d [", plnum, prnum);
	ss = ss + name;
	ss = ss+L"]";
	node = tree->Items->AddChild(n, ss); node->ImageIndex = 2; node->SelectedIndex = 2;
	node->Expand(true);

	this->plnum = plnum;
	num = prnum;

	n->Expand(true);

	return 0;
}

TTreeNode* TGLPort::GetNode(void)
{
	return node;
}

void TGLPort::syspend_cycle ()
{
	cycle_syspend_flag = true;
}

void TGLPort::start_cycle ()
{
	cycle_syspend_flag = false;
}

 int TGLPort::transact_request_XY(TGLSensor* sn)
 {
	  BYTE* buf = NULL;
	  int* len = NULL;

	  sn->clrTX();

 //	  buf = sn->getRX();
 //	  len = sn->getRXidx();

	  if (sn->request_curr_XY(&buf, &len) == 0)
	  {
		 com->Purge();

		 if (!com->PortNWrite((DWORD)*len, buf))
		 {
			 return -1;
         }
	  }

	  Sleep(delay_set.delay_cmd_exec);

	  //sn->clrRX();

	  buf = sn->getRX();
	  len = sn->getRXidx();

	  int i = 1;

	  if (com->PortNRead(1, buf))
	  {
		  *len++;

		  while(com->PortNRead(1, &buf[i]))
		  {
			  *len++; i++;
          }
	  }

	  sn->accept_response_curr_XY();

	  return 0;
 }

 //extended buffers
 int TGLPort::transact_request_XY_ex(TGLSensor* sn)
 {
	  txidx = 0; rxidx = 0;
	  memset(buftx, 0, 8448); memset(buftx, 0, 8448);

	  if (sn->request_curr_XY(buftx, &txidx) == 0)
	  {
		 com->Purge();

		 if (!com->PortNWrite((DWORD)txidx, buftx))
		 {
			 return -1;
         }
	  }

	  Sleep(delay_set.delay_cmd_exec);

	  int i = 1;

	  if (com->PortNRead(1, bufrx))
	  {
		  rxidx++;

		  while(com->PortNRead(1, &bufrx[i]))
		  {
			  rxidx++; i++;
          }
	  }

	  sn->accept_response_curr_XY(bufrx, &rxidx);

	  return 0;
 }


bool TGLPort::is_suspended ()
{
	return cycle_syspend_flag;
}

int TGLPort::cycle ()
{
	while (is_suspended())
	{
		Sleep(delay_set.delay_default);
	}

    com->Close();

	if (com->Open(true, sys_port_num) == false)
	{
		Sleep(delay_set.delay_default);
	}
	else
	{
		for (auto itsn : sensor_list.m_list)
		{
			transact_request_XY_ex(itsn);
		}
	}

	syspend_cycle ();

	return 0;
}

bool TGLPort::is_run_engine_suspended ()
{
	return suspend_run_engine;
}

void TGLPort::run_engine_suspend ()
{
	suspend_run_engine = true;
}

void TGLPort::run_engine_resume ()
{
	suspend_run_engine = false;
}

void TGLPort::set_mark(TCHAR* mr)
{
   mark = mr;
}

TCHAR* TGLPort::get_mark(void)
{
   return mark.c_bstr();
}

void TGLPort::set_sys_port_num(DWORD n)
{
	sys_port_num = n;
}

DWORD TGLPort::get_sys_port_num(void)
{
	return sys_port_num;
}

void TGLPort::set_baud(DWORD b)
{
	baud = b;
}

DWORD TGLPort::get_baud(void)
{
	return baud;
}

void TGLPort::set_on(int n)
{
	on = (bool)n;
}

int TGLPort::get_on(void)
{
	return (int)on;
}

void TGLPort::set_type(int t)
{
	type = t;
}

int TGLPort::get_type(void)
{
	return type;
}

void TGLPort::set_com(void)
{
	com->set_baud(baud);
}
