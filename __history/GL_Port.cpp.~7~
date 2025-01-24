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
	name = nm;
	mark = L"...";
	node = NULL;
	num = nn;
}

TGLPort::~TGLPort()
{

}

void TGLPort::SetBaud(int b)
{
	baud = b;
}

int TGLPort::add_sensor(WideString nm, int plnum)
{
	int nmb = sensor_list.count() + 1;
	sns = new TGLSensor(nm, NULL, nmb);

	if (sensor_list.add(sns) != 0)
	{
		delete sns;
		return -1;  //fail to add the port
	}
	else
	{
		WideString ss;
		ss.printf(L"%d.%d.%d [", plnum, num, nmb);
		ss = ss+nm;
		ss = ss+L"]";
		TTreeNode* ndd = tree->Items->AddChild(node, ss); ndd->ImageIndex = 3; ndd->SelectedIndex = 3;
		node->Expand(true);

		sns->SetTree(tree);
		sns->SetNode(ndd);

		sns->SetPlnum(plnum);
		sns->SetPrnum(num);

		return 0;
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

