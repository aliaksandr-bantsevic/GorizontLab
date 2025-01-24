//---------------------------------------------------------------------------

#ifndef GL_PortH
#define GL_PortH
//---------------------------------------------------------------------------
#include "defs.h"
#include "GL_List.h"

class TGLSensor;
//---------------------------------------------------------------------------

class TGLPort
{

public:

	TGLPort();
	TGLPort(WideString nm, TTreeNode* nd, int nn);
	~TGLPort();

	WideString name;
	WideString mark;
	TTreeNode* node;
	int num;
	int plnum;

public:

	TGLList<TGLSensor> sensor_list;

public:

	void SetBaud(int b);
	TGLSensor* add_sensor(WideString nm, int plnum);
	void SetTree(TTreeView* t);
	void SetNode(TTreeNode* n);

    void SetPlnum(int n);

private:

    TTreeView* tree;
	TGLSensor* sns;
	int baud;
};

#endif
