//---------------------------------------------------------------------------

#pragma hdrstop

#include "GL_Sensor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TGLSensor::TGLSensor()
{
	 name = L"New Sensor";
	 mark = L"...";
	 node = NULL;
	 num = 0;
}

TGLSensor::TGLSensor(WideString nm, TTreeNode* nd, int nn)
{
	name = nm;
	mark = L"...";
	node = nd;
	num = nn;

	baud = 9600;
	addr = 1;
}

TGLSensor::~TGLSensor()
{

}

void TGLSensor::SetTree(TTreeView* t)
{
	tree = t;
}

void TGLSensor::SetNode(TTreeNode* n)
{
	node = n;
}

void TGLSensor::SetPlnum(int n)
{
	plnum = n;
}

void TGLSensor::SetPrnum(int n)
{
	prnum = n;
}

void TGLSensor::SetBaud(DWORD bd)
{
	baud = bd;
}

DWORD TGLSensor::GetBaud(void)
{
	return baud;
}

WideString TGLSensor::GetName(void)
{
	return name;
}

BYTE TGLSensor::GetAddr(void)
{
	return addr;
}

void TGLSensor::SetAddr(Byte ad)
{
	addr = ad;
}

int TGLSensor::ReDraw(TTreeNode* n, int plnum, int prnum, int snnum)
{
   WideString ss;
	ss.printf(L"%d.%d.%d [", plnum, prnum, snnum);
	ss = ss + name;
	ss = ss+L"]";
	node = tree->Items->AddChild(n, ss); node->ImageIndex = 3; node->SelectedIndex = 3;
	n->Expand(true);

	this->plnum = plnum;
	this->prnum = prnum;
	num = snnum;


	return 0;
}
