//---------------------------------------------------------------------------

#pragma hdrstop

#include "GL_Object.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


TGLObject::TGLObject()
{
	name = L"Новый объект";
	mark = L"...";
	node = NULL;
	num = 0;
}

TGLObject::TGLObject(WideString nm, TTreeNode* nd, int nn)
{
	name = nm;
	mark = L"...";
	node = nd;
	num = nn;
}


TGLObject::~TGLObject()
{

}

int TGLObject::add_port(WideString nm, TTreeNode* nd, int nn)
{/*
	TGLPort* pr = new TGLPort(nm, nd, nn);

	if (prt_list.add(pr) != 0)
	{
		return -1;  //fail to add the port
	}
	else
	{
		delete pr;
		return 0;
	}
*/
}

