//---------------------------------------------------------------------------

#ifndef FMainH
#define FMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>

#include <Vcl.Menus.hpp>
#include <list>

#include "defs.h"

#include "GL_System.h"
#include "GL_Object.h"
#include "GL_Port.h"
#include "GL_Sensor.h"

//---------------------------------------------------------------------------
class TForm_General : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar;
	TStatusBar *StatusBar;
	TPanel *Panel_Browser;
	TSplitter *Splitter1;
	TPanel *Panel_Work;
	TPanel *Panel_Data;
	TSplitter *Splitter2;
	TPanel *Panel_Console;
	TToolButton *ToolButton1;
	TImageList *ImageList_Tool_bar;
	TTreeView *TreeView_Browser;
	TPageControl *PageControl_Data;
	TPageControl *PageControl_Console;
	TTimer *Timer_General_1s;
	TPopupMenu *PopupMenu;
	TMenuItem *N1;
	TMenuItem *N2;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TToolButton *ToolButton4;
	TToolButton *ToolButton5;
	TToolButton *ToolButton6;
	TTimer *Timer_Init_app;
	void __fastcall ToolButton1Click(TObject *Sender);
	void __fastcall Timer_General_1sTimer(TObject *Sender);
	void __fastcall ToolButton2Click(TObject *Sender);
	void __fastcall Timer_Init_appTimer(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TForm_General(TComponent* Owner);

private:     //Window property management

	void InitMainWindow(void);
	void DevideMainWindow(int browser_part, int data_part);

    void InitApplication(void);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_General *Form_General;
//---------------------------------------------------------------------------
#endif
