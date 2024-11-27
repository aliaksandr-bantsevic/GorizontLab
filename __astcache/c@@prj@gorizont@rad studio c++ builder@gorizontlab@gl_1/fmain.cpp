//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_General *Form_General;
//---------------------------------------------------------------------------
__fastcall TForm_General::TForm_General(TComponent* Owner)
	: TForm(Owner)
{
	InitMainWindow();
}
//---------------------GLOBALS-----------------------------------------------

TGLSystem* GLSystem;

//--------------------------------------------------------------------------

void TForm_General::InitMainWindow(void)
{
   this->WindowState = wsMaximized;
   DevideMainWindow(30, 85);
}

void TForm_General::DevideMainWindow(int browser_part, int data_part)
{
	int hor_res = Form_General->Width;
	int ver_res = Form_General->Height;

	int browser_width =  hor_res * browser_part / 100;
	int data_height  =  ver_res * data_part / 100;

	this->Panel_Browser->Width = browser_width;
	this->Panel_Data->Height = data_height;

}

WideString oname(L"obj#1");
int iname = 1;

TGLList<TGLObject> o_list;

void __fastcall TForm_General::ToolButton1Click(TObject *Sender)
{
	//StatusBar->Panels->Items[0]->Text = L"Status Bar";
	//StatusBar->Panels->Items[1]->Text = L"Status Bar message 2";

	TGLObject* obj = new TGLObject;

	oname.printf(L"obj#%d", iname); obj->name = oname; iname++;

	o_list.add(obj);

	WideString s("");


	o_list.show();

	Sleep(1);

}
//---------------------------------------------------------------------------

void __fastcall TForm_General::Timer_General_1sTimer(TObject *Sender)
{
	//Show Current time
	StatusBar->Panels->Items[0]->Text = GetCurrentTimeStr();
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::ToolButton2Click(TObject *Sender)
{
	//GLSystem.remove_object(L"obj#2");
	//GLSystem.show_objects();

	o_list.remove(L"obj#2");
	o_list.show();

}
//---------------------------------------------------------------------------
void TForm_General::InitApplication(void)
{
   GLSystem = new TGLSystem;

   WideString s;
   s.printf(L"Версия: %d.%d.%d", BUILD, VERSION, SUBVERSION);

   StatusBar->Panels->Items[1]->Text = s;
}
void __fastcall TForm_General::Timer_Init_appTimer(TObject *Sender)
{
	Timer_Init_app->Enabled = false;
	InitApplication();
}
//---------------------------------------------------------------------------

