//---------------------------------------------------------------------------
#ifndef GL_PlaceH
#define GL_PlaceH
//---------------------------------------------------------------------------
#include "defs.h"
#include "GL_List.h"
class TGLPort;
//---------------------------------------------------------------------------
class TGLPlace
{

public:

	TGLPlace();
    TGLPlace(WideString nm);
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

	TGLPort* add_port(WideString nm);
	void SetTree(TTreeView* t);
	void SetNode(TTreeNode* n);
	void ReDraw(TTreeNode* n, int plnum);
    TTreeNode* GetNode(void);

private:

	TGLPort* prt;

	TTreeView* tree;

};
#endif


