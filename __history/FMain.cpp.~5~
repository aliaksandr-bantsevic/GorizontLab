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
//---------------------------------------------------------------------------

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
void __fastcall TForm_General::ToolButton1Click(TObject *Sender)
{
	StatusBar->Panels->Items[0]->Text = L"Status Bar";
	StatusBar->Panels->Items[1]->Text = L"Status Bar message 2";
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::Timer_General_1sTimer(TObject *Sender)
{
	//Show Current time
   	StatusBar->Panels->Items[0]->Text = GetCurrentTimeStr();
}
//---------------------------------------------------------------------------

