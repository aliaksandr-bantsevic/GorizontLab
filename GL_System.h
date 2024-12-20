//---------------------------------------------------------------------------

#ifndef GL_SystemH
#define GL_SystemH
//---------------------------------------------------------------------------
#include <vcl.h>
#include "GL_List.h"
class TGLPlace;
//---------------------------------------------------------------------------

#define OBJ_TYPE_NONE     0
#define OBJ_TYPE_SYST     1
#define OBJ_TYPE_PLCE     2
#define OBJ_TYPE_PORT     3
#define OBJ_TYPE_SNSR     4

extern WideString g_ws_msg;

class TGLSystem
{

public:

	TGLSystem();
	TGLSystem(TTreeView* t);
	~TGLSystem();

public:

private:

    TGLPlace* pl;

public:

	TGLList<TGLPlace> place_list;

private:

   	TTreeNode* node;

	TGLPlace* 	cur_pl;
	TGLPort* 	cur_pr;
	TGLSensor* 	cur_sn;

public:

	WideString name;
	WideString mark;

   	TTreeView* tree;

public:


public:

	TGLPlace* add_place(WideString nm);
	TTreeNode* get_node(void);
	void View(BYTE state);
	//TGLPlace* GetPlace(int n);
	TObject* GetBrowserElement(TTreeNode* nd, int* type);
	void ProcBrowser�lick(TTreeNode* nd, int* tp);

};


#endif
