//---------------------------------------------------------------------------

#ifndef GL_SystemH
#define GL_SystemH
//---------------------------------------------------------------------------
#include <vcl.h>
#include "GL_List.h"

#include "SysConfMgr.h"

#include <vcl.h>
#include <Xml.XMLDoc.hpp>
#include <Xml.XMLIntf.hpp>
#include "utils.h"

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
	TGLSystem(TTreeView* t, TXMLDocument* xmlDoc);
	~TGLSystem();

public:

private:

	TGLPlace* pl;
    TSysConfMgr* SysConfMgr;

public:

	TGLList<TGLPlace> place_list;

public:

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
	void ProcBrowserСlick(TTreeNode* nd, int* tp);
	TGLPort* add_port(WideString nm);
	TGLSensor* add_sensor(WideString nm);
	int SaveConf(void);
	int LoadConf(void);
	TGLSensor* GetCurSn(void);
	int ReDraw(void);
	int delete_sensor(void);
	int delete_port(void);
    int delete_place(void);
};


#endif
