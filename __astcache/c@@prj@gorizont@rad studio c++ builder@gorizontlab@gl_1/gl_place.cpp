//---------------------------------------------------------------------------

#pragma hdrstop

#include "GL_Place.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#include "GL_Port.h"
//---------------------------------------------------------------------------

TGLPlace::TGLPlace()
{
	name = L"Новый объект";
	mark = L"...";
	node = NULL;
	num = 0;

}

TGLPlace::TGLPlace(WideString nm)
{
	name = nm;
	mark = L"...";
	node = NULL;
	tree = NULL;
	num = 0;
}

TGLPlace::TGLPlace(WideString nm, TTreeNode* nd, int nn)
{
	name = nm;
	mark = L"...";
	node = nd;
	num = nn;
}

TGLPlace::~TGLPlace()
{

}

TGLPort* TGLPlace::add_port(WideString nm)
{
	int nmb = port_list.count() + 1;
	prt = new TGLPort(nm, NULL, nmb);

	if (port_list.add(prt) != 0)
	{
		delete prt;
		return NULL;  //fail to add the port
	}
	else
	{
		WideString ss;
		ss.printf(L"%d.%d [", num, nmb);
		ss = ss+nm;
		ss = ss+L"]";
		TTreeNode* ndd = tree->Items->AddChild(node, ss); ndd->ImageIndex = 2; ndd->SelectedIndex = 2;
		node->Expand(true);

		prt->SetTree(tree);
		prt->SetNode(ndd);
		prt->SetPlnum(num);

		////////////!!!!
		//prt->add_sensor(L"AND3#1", num);
		//prt->add_sensor(L"AND3#2", num);
		//prt->add_sensor(L"AND3#2", num);
		//prt->add_sensor(L"AND3#4", num);
		//prt->add_sensor(L"AND3#5", num);
		//prt->add_sensor(L"IND3#1", num);
		//prt->add_sensor(L"IND3#2", num);
		//prt->add_sensor(L"IND3#3", num);

		return prt;
	}
}

void TGLPlace::SetTree(TTreeView* t)
{
	tree = t;
}

void TGLPlace::SetNode(TTreeNode* n)
{
	node = n;
}

void TGLPlace::ReDraw(TTreeNode* n, int plnum)
{
	WideString ss;
	ss.printf(L"%d [", plnum);
	ss = ss + name;
	ss = ss+L"]";
	node = tree->Items->AddChild(n, ss); node->ImageIndex = 1; node->SelectedIndex = 1;

	num = plnum;

    n->Expand(true);
}

TTreeNode* TGLPlace::GetNode(void)
{
	return node;
}
