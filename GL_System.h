//---------------------------------------------------------------------------

#ifndef GL_SystemH
#define GL_SystemH
//---------------------------------------------------------------------------
#include <vcl.h>
#include "GL_List.h"
class TGLPlace;
//---------------------------------------------------------------------------
class TGLSystem
{

public:

	TGLSystem();
	TGLSystem(TTreeNode* nd);
	~TGLSystem();

public:

private:

public:

	TGLList<TGLPlace> place_list;

public:

	WideString name;
	WideString mark;
	TTreeNode* node;

public:


public:

	int add_place(WideString nm, TTreeNode* nd, int nn);

};


#endif
