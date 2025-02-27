﻿//---------------------------------------------------------------------------

#ifndef GL_ObjectH
#define GL_ObjectH
//---------------------------------------------------------------------------
#include <vcl.h>

#include "GL_List.h"
#include "GL_Port.h"
#include "GL_Sensor.h"

//---------------------------------------------------------------------------

class TGLPort; //???

class TGLObject
{

public:

	TGLObject();
 	TGLObject(WideString nm, TTreeNode* nd, int nn);
	~TGLObject();

public:

//	TGLList<TGLPort> prt_list;

//	TGLList<TGLPort> p_list;

//	TGLList<TGLSensor> s_list;

	TGLSensor* ss;
	TGLPort* pp;

public:

	WideString name;
	WideString mark;
	TTreeNode* node;
	int num;

public:

	int add_port(WideString nm, TTreeNode* nd, int nn);
};

#endif
