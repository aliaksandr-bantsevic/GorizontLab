//---------------------------------------------------------------------------

#ifndef GL_SensorH
#define GL_SensorH
//---------------------------------------------------------------------------
#include "defs.h"
#include "GL_List.h"
//---------------------------------------------------------------------------
class TGLSensor
{

public:

	TGLSensor();
	TGLSensor(WideString nm, TTreeNode* nd, int nn);
	~TGLSensor();

public:

	WideString name;
	WideString mark;
	TTreeNode* node;
	int num;

	int plnum;
	int prnum;

public:

	void SetTree(TTreeView* t);
	void SetNode(TTreeNode* n);
	void SetPlnum(int n);
    void SetPrnum(int n);

private:

	TTreeView* tree;

};


#endif
