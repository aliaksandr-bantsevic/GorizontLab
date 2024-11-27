//---------------------------------------------------------------------------

#ifndef GL_SensorH
#define GL_SensorH
//---------------------------------------------------------------------------

class TGLSensor
{

public:

	TGLSensor();
	~TGLSensor();

public:

	WideString name;
	WideString mark;
	TTreeNode* node;
	int num;

};


#endif
