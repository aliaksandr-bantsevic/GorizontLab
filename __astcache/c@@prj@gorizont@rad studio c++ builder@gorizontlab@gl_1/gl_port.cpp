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
}

TGLPort::TGLPort(WideString nm, TTreeNode* nd, int nn)
{
	name = L"Новый объект";
	mark = L"...";
	node = NULL;
	num = 0;
}

TGLPort::~TGLPort()
{

}

void TGLPort::SetBaud(int b)
{
	baud = b;
}

int TGLPort::add_sensor(WideString nm, TTreeNode* nd, int nn)
{
	sns = new TGLSensor(nm, nd, nn);

	if (sensor_list.add(sns) != 0)
	{
		delete sns;
		return -1;  //fail to add the sensor
	}
	else
	{
		return 0;
	}

}
