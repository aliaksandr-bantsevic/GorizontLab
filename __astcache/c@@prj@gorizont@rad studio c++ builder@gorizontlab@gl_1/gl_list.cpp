//---------------------------------------------------------------------------

#pragma hdrstop

#include "GL_List.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

template <typename T>
TGLList<T>::TGLList()
{

}

template <typename T>
TGLList<T>::~TGLList()
{

}

template <typename T>
int TGLList<T>::add(T* obj)
{
	if (find(obj->name) != NULL)
	{
		return -1; //object with such a name alreadu exists
	}

	m_list.push_back(obj);

	return 0;
}

template <typename T>
T* TGLList<T>::find(WideString nm)
{
	T* obj = NULL;

	WideString name = nm;

	auto ff = [&name](const T* ob)
	{
		return ob->name == name;
	};

	typename std::list<T*>::iterator it;

	for (auto it : m_list)
	{
		 if (ff(it))
		 {
			   obj = it;
			   break;
		 }
	}

	return obj;
}

template <typename T>
int TGLList<T>::remove(WideString nm)
{

	typename std::list<T*>::iterator it = m_list.begin();

	int res = -1;  //object to be deleted not find

	for (auto it : m_list)
	{
		 if (it->name == nm)
		 {
			delete it;
			m_list.remove(it);
			res = 0;
			break;
		 }
	}

	return res;
}

template <typename T>
int TGLList<T>::show(void)
{

	typename std::list<T*>::iterator it = m_list.begin();
	int res = 0;

	WideString ss;

	for (auto it : m_list)
	{
		ss += it->name; ss += L"\r\n";
	}

	 ShowMessage(ss);

	 return res;
}