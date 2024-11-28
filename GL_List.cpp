//---------------------------------------------------------------------------

#pragma hdrstop

#include "GL_List.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#include "GL_List.h"
#include "GL_Place.h"
#include "GL_Port.h"
#include "GL_Sensor.h"

// Явные инстанцирования шаблонного класса
template class TGLList<TGLPlace>;
template class TGLList<TGLPort>;
template class TGLList<TGLSensor>;


