//---------------------------------------------------------------------------

#ifndef GL_ListH
#define GL_ListH
//---------------------------------------------------------------------------
#include <list>
#include <algorithm>

#include "GL_Object.h"
#include "GL_Port.h"
#include "GL_Sensor.h"

//---------------------------------------------------------------------------
template<typename T>
class TGLList
{

public:

	TGLList();
	virtual ~TGLList();

public:

	int add(T* obj);
	int add(WideString nm, TTreeNode* nd, int nn);

	T* find(WideString nm);
	T* find(TTreeNode* nd);
	T* find(int nn);

	int remove(WideString nm);
	int show(void);


private:

	std::list<T*> m_list;
};


#endif
