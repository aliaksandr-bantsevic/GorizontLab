//---------------------------------------------------------------------------

#pragma hdrstop

#include "GL_System.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#include "GL_Place.h"

//---------------------------------------------------------------------------

TGLSystem::TGLSystem()
{
   name = L"GorizontLab Monitoring System";
   mark = L"...";
   node = NULL;
}

TGLSystem::TGLSystem(TTreeNode* nd)
{
   name = L"GorizontLab Monitoring System";
   mark = L"...";
   node = nd;
}

TGLSystem::~TGLSystem()
{

}

int TGLSystem::add_place(WideString nm, TTreeNode* nd, int nn)
{
	TGLPlace* pl = new TGLPlace(nm, nd, nn);

	if (place_list.add(pl) != 0)
	{
		delete pl;
		return -1;  //fail to add the place
	}
	else
	{
		return 0;
	}
}


