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
	WideString s;

	port = pr;

	ComboBox_tout_rd->Clear();
	ComboBox_tout_wr->Clear();
	ComboBox_dl_df->Clear();
	ComboBox_dl_ex->Clear();

	ComboBox_tout_rd->Items->Add(5);
	ComboBox_tout_rd->Items->Add(10);
	ComboBox_tout_rd->Items->Add(20);
	ComboBox_tout_rd->Items->Add(50);
	ComboBox_tout_rd->Items->Add(100);
	ComboBox_tout_rd->Items->Add(500);
	ComboBox_tout_rd->Items->Add(1000);
	ComboBox_tout_rd->Items->Add(5000);
	ComboBox_tout_rd->ItemIndex = 0;

	ComboBox_tout_rd->ItemIndex = 1;

	ComboBox_tout_wr->Items->Add(5);
	ComboBox_tout_wr->Items->Add(10);
	ComboBox_tout_wr->Items->Add(20);
	ComboBox_tout_wr->Items->Add(50);
	ComboBox_tout_wr->Items->Add(100);
	ComboBox_tout_wr->Items->Add(500);
	ComboBox_tout_wr->Items->Add(1000);
	ComboBox_tout_wr->Items->Add(5000);
	ComboBox_tout_wr->ItemIndex = 1;

	ComboBox_dl_df->Items->Add(5);
	ComboBox_dl_df->Items->Add(10);
	ComboBox_dl_df->Items->Add(15);
	ComboBox_dl_df->Items->Add(20);
	ComboBox_dl_df->Items->Add(30);
	ComboBox_dl_df->ItemIndex = 1;

	ComboBox_dl_ex->Items->Add(5);
	ComboBox_dl_ex->Items->Add(10);
	ComboBox_dl_ex->Items->Add(15);
	ComboBox_dl_ex->Items->Add(20);
	ComboBox_dl_ex->Items->Add(30);
	ComboBox_dl_ex->ItemIndex = 3;

	ComboBox_COM->Clear();

	for (int i = 1; i < 255; i++)
	{
		ComboBox_COM->Items->Add(i);
	}

	ComboBox_COM->ItemIndex = 0;

	ComboBox_type->Clear();
	ComboBox_type->Items->Add(L"COM");
	ComboBox_type->Items->Add(L"TCP");
	ComboBox_type->ItemIndex = 0;

	ComboBox_baud->Clear();
	ComboBox_baud->Items->Add(115200);
	ComboBox_baud->Items->Add(9600);
	ComboBox_baud->ItemIndex = 0;

	CheckBox_on->Checked= true;

    Memo_mark->Text = L"Порт опроса датчиков";

	if (port)
	{
	  ComboBox_type->ItemIndex = port->get_type();
	  ComboBox_COM->ItemIndex = port->get_sys_port_num() - 1;
	  CheckBox_on->Checked = (bool)port->get_on();
	  s.printf(L"%d", port->get_baud());
	  ComboBox_baud->Text = s;
	  TCHAR* m = port->get_mark();
	  Memo_mark->Text = port->get_mark();

	  s.printf(L"%d", port->delay_set.tout_rd);
	  ComboBox_tout_rd->Text = s;

	  s.printf(L"%d", port->delay_set.tout_wr);
	  ComboBox_tout_wr->Text = s;

	  s.printf(L"%d", port->delay_set.delay_default);
	  ComboBox_dl_df->Text = s;

	  s.printf(L"%d", port->delay_set.delay_cmd_exec);
	  ComboBox_dl_ex->Text = s;
	}

	ShowModal();
}
void __fastcall TForm_Port_adjust::Button_okClick(TObject *Sender)
{
	WideString sport;

	if (ComboBox_type->ItemIndex == 0)
	{
		sport.printf(L"COM_%03d", ComboBox_COM->ItemIndex + 1);
	}
	else
	{
	   sport.printf(L"TCP_%03d", ComboBox_COM->ItemIndex + 1);
	}

	if (port == NULL)
	{

		if (port = GLSystem->add_port(sport))
		{

		}
		else
		{
			 ShowMessage(L"Не удалось добавить порт!");
			//    delete port;
		}

	}

	if (port)
	{
		if (ComboBox_type->ItemIndex == 0)
		{
			sport.printf(L"COM_%03d", ComboBox_COM->ItemIndex + 1);
		}
		else
		{
		   sport.printf(L"TCP_%03d", ComboBox_COM->ItemIndex + 1);
		}

		port->name = sport;

		port->set_type(ComboBox_type->ItemIndex);
		port->set_sys_port_num(ComboBox_COM->ItemIndex + 1);
		port->set_on(CheckBox_on->Checked);
		port->set_baud(ComboBox_baud->Text.ToInt());
		port->set_mark(Memo_mark->Text.c_str());

		port->delay_set.tout_rd = ComboBox_tout_rd->Text.ToInt();
		port->delay_set.tout_wr = ComboBox_tout_wr->Text.ToInt();
		port->delay_set.delay_default = ComboBox_dl_df->Text.ToInt();
        port->delay_set.delay_cmd_exec = ComboBox_dl_ex->Text.ToInt();


	}

	Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Port_adjust::Timer_startTimer(TObject *Sender)
{
	Timer_start->Enabled = false;
}
//---------------------------------------------------------------------------


void __fastcall TForm_Port_adjust::Button_cancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

