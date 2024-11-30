/*
	User definisions
*/

/*
	Version
*/

extern bool b_MessageConfirmParamsetShow;

#define BUILD           0
#define VERSION         0
#define SUBVERSION      4

/*
	Forms control macro definitions
*/

#define 	MACRO_FORM_MOVE_TO_THE_CENTER 	this -> Position = poDesktopCenter;
#define 	MACRO_FORM_CLOSE   this -> Close();
/* Confirm of parameter saving is redundant */
//#define     MACRO_IF_OPERATION_SAVE_PARAM_CONFIRMED     if (b_MessageConfirmParamsetShow) if (utils_confirm_operation_save_adjust() == false) return;
#define     MACRO_IF_OPERATION_SAVE_PARAM_CONFIRMED        {;}

//Default debug pause = 10ms
#define     DbgSleep 0

/*
	needed includes for all the units
*/

#include <vcl.h>
#include <IniFiles.hpp>
#include "utils.h"
#include "minwindef.h"
#include "GL_Time_utils.h"

/*
	For debug development mode
*/

//#define DEBUG_DEVELOPMENT_MODE


