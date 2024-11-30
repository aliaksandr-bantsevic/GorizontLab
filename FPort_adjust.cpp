//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FPort_adjust.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_Port_adjust *Form_Port_adjust;
//---------------------------------------------------------------------------
__fastcall TForm_Port_adjust::TForm_Port_adjust(TComponent* Owner)
	: TForm(Owner)
{
		MACRO_FORM_MOVE_TO_THE_CENTER
}
//---------------------------------------------------------------------------

void TForm_Port_adjust::start(TGLPort* pr)
{
	port = pr;

	ShowModal();
}
void __fastcall TForm_Port_adjust::Button_okClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

