//---------------------------------------------------------------------------

#ifndef GL_ListH
#define GL_ListH
//---------------------------------------------------------------------------
#include <list>
#include "GL_Object.h"
#include "GL_Port.h"

//---------------------------------------------------------------------------
template<typename T>
class TGLList
{

public:

	TGLList();
	virtual ~TGLList();

public:

	int add(T* obj);
	T* find(WideString nm);
	int remove(WideString nm);
    int show(void);

	private:

	std::list<T*> m_list;
};


#endif