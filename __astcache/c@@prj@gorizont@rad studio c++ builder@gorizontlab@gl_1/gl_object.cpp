//---------------------------------------------------------------------------

#pragma hdrstop

#include "GL_Object.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


TGLObject::TGLObject()
{
	name = L"Новый объект";
	mark = L"...";
	node = NULL;
    num = 0;
}

TGLObject::~TGLObject()
{

}
