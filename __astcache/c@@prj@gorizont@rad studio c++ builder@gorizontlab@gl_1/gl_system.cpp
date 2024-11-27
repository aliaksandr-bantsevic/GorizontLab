//---------------------------------------------------------------------------

#pragma hdrstop

#include "GL_System.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


TGLSystem::TGLSystem()
{
   name = L"GorizontLab Monitoring System";
}

TGLSystem::~TGLSystem()
{

}

int TGLSystem::add_object(TGLObject* obj)
{

	if (find_object(obj->name) != NULL)
	{
		return -1; //object with such a name alreadu exists
	}

	objects_list.push_back(obj);

	return 0;
}

TGLObject* TGLSystem::find_object(WideString& nm)
{
	TGLObject* obj = NULL;

	WideString name = nm;

	auto ff = [&name](const TGLObject* ob)
	{
		return ob->name == name;
	};

	std::list<TGLObject*>::iterator it;

	//for (it = objects_list.begin(); it != objects_list.end(); ++it)
	for (auto it : objects_list)
	{
		 if (ff(it))
		 {
			   obj = it;
			   break;
		 }
	}

	return obj;
 }

int TGLSystem::remove_object(WideString nm)
{

	std::list<TGLObject*>::iterator it = objects_list.begin();
	int res = -1;  //object to be deleted not find

	for (auto it : objects_list)
	{
		 if (it->name == nm)
		 {
            delete it;
			objects_list.remove(it);
			res = 0;
			break;
		 }
	}

	return res;
}

int TGLSystem::show_objects(void)
{

	std::list<TGLObject*>::iterator it = objects_list.begin();
	int res = 0;

	WideString ss;

	for (auto it : objects_list)
	{
		ss += it->name; ss += L"\r\n";
	}

	 ShowMessage(ss);

	 return res;
}


