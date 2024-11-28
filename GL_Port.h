//---------------------------------------------------------------------------

#ifndef GL_PortH
#define GL_PortH
//---------------------------------------------------------------------------
#include "GL_List.h"
#include <vcl.h>

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

public:

	TGLList<TGLSensor> sensor_list;

public:

	void SetBaud(int b);
	int add_sensor(WideString nm, TTreeNode* nd, int nn);

private:

	TGLSensor* sns;
	int baud;
};

#endif
