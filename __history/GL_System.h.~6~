//---------------------------------------------------------------------------

#ifndef GL_SystemH
#define GL_SystemH
//---------------------------------------------------------------------------
#include <list>
#include "GL_Object.h"
#include <algorithm>

class TGLSystem
{

public:

	TGLSystem();
	~TGLSystem();

public:

private:

	std::list<TGLObject*> objects_list;
    WideString name;

public:

	int add_object(TGLObject* obj);
	TGLObject* find_object(WideString& nm);
    int remove_object(WideString& nm);
};


#endif
