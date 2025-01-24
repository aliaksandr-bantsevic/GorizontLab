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
