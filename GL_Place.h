//---------------------------------------------------------------------------
#ifndef GL_PlaceH
#define GL_PlaceH
//---------------------------------------------------------------------------
#include <vcl.h>
#include "GL_List.h"
class TGLPort;
//---------------------------------------------------------------------------
class TGLPlace
{

public:

	TGLPlace();
	TGLPlace(WideString nm, TTreeNode* nd, int nn);
	~TGLPlace();

public:

	TGLList<TGLPort> port_list;

public:

	WideString name;
	WideString mark;
	TTreeNode* node;
	int num;

public:

	int add_port(WideString nm, TTreeNode* nd, int nn);

private:

    TGLPort* prt;

};
#endif

