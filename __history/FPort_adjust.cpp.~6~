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

	ComboBox_COM->Clear();

	for (int i = 1; i < 255; i++)
	{
		ComboBox_COM->Items->Add(i);
	}

	ComboBox_COM->ItemIndex = 0;

	ShowModal();
}
void __fastcall TForm_Port_adjust::Button_okClick(TObject *Sender)
{
	WideString sport;
	sport.printf(L"COM_%03d", ComboBox_COM->ItemIndex + 1);

		if (GLSystem->add_port(sport))
		{

		}
		else
		{
			//ShowMessage(L"Не удалось добавить порт!");
			//    delete port;
		}

	Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Port_adjust::Timer_startTimer(TObject *Sender)
{
	Timer_start->Enabled = false;
}
//---------------------------------------------------------------------------

