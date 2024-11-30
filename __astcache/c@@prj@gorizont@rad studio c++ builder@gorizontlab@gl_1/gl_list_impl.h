//---------------------------------------------------------------------------
#ifndef GL_List_implH
#define GL_List_implH
//---------------------------------------------------------------------------

template <typename T>
TGLList<T>::TGLList()
{

}

template <typename T>
TGLList<T>::~TGLList()
{
	for (T* obj : m_list)
	{
		delete obj;
	}

	m_list.clear();
}

template <typename T>
int TGLList<T>::add(T* obj)
{
	if (find(obj->name) != NULL)
	{
		return -1; //object with such a name already exists
	}

	//if (find(obj->node) != NULL)
	//{
	//	return -2; //object with such a node already exists
	//}

	if (find(obj->num) != NULL)
	{
		return -3; //object with such a num already exists
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
T* TGLList<T>::find(TTreeNode* nd)
{
	T* obj = NULL;

	TTreeNode* node = nd;

	auto ff = [&node](const T* ob)
	{
		return ob->node == node;
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
T* TGLList<T>::find(int nn)
{
	T* obj = NULL;

	int num = nn;

	auto ff = [&num](const T* ob)
	{
		return ob->num == num;
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

template <typename T>
int TGLList<T>::add(WideString nm, TTreeNode* nd, int nn)
{
   T* ob = new T;

   ob->name = nm;
   ob->node = nd;
   ob->num = nn;
}

template <typename T>
int TGLList<T>::count(void)
{
	return m_list.size();
}

#endif  //GL_List_implH
