﻿//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FSensor_adjust.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_Sensor_adjust *Form_Sensor_adjust;
//---------------------------------------------------------------------------
__fastcall TForm_Sensor_adjust::TForm_Sensor_adjust(TComponent* Owner)
	: TForm(Owner)
{
    MACRO_FORM_MOVE_TO_THE_CENTER
}
//---------------------------------------------------------------------------

void TForm_Sensor_adjust::start(TGLSensor* sn)
{
	sensor = sn;

	ComboBox_addr->Clear();

	for (int i = 1; i < 255; i++)
	{
		ComboBox_addr->Items->Add(i);
	}

	ComboBox_addr->ItemIndex = 0;

	ComboBox_type->Clear();
	ComboBox_type->Items->Add(L"IND3");
	ComboBox_type->Items->Add(L"AND3");
	ComboBox_type->ItemIndex = 0;

	ComboBox_prt->Clear();
	ComboBox_prt->Items->Add(L"IND3");
	ComboBox_prt->Items->Add(L"AND3");
	ComboBox_prt->Items->Add(L"Modbus RTU");
	ComboBox_prt->ItemIndex = 0;

	Memo_mark->Text = L"Датчик сбора данных";
	CheckBox_on->Checked = false;

	if (sensor)
	{
	   WideString s;
	   Caption = L"Настроить датчик";
	   s.printf(L"%d", sensor->GetAddr());
	   ComboBox_addr->Text = s;
	   ComboBox_addr->Enabled = false;

	   Memo_mark->Text = sensor->get_mark();
	   CheckBox_on->Checked = sensor->get_on();

	   ComboBox_type->ItemIndex = sensor->get_type();
	   ComboBox_addr->ItemIndex = sensor->get_addr() - 1;
	}
	else
	{
	  Caption = L"Добавить датчик";
	  ComboBox_addr->Enabled = true;
    }

	ShowModal();
}
void __fastcall TForm_Sensor_adjust::Button_OKClick(TObject *Sender)
{
	WideString ss("");
	TGLSensor* sn;

	Update();

	if (sensor == NULL)
	{
	  sn = GLSystem->add_sensor("", GLSystem->get_sens_uid_max());
	  if (sn == NULL)
	  {
		  ShowMessage(L"Не удалось добавить датчик!");
		  Close();
	  }
	  else
	  {
			sensor = sn;
	  }
	}

	if (sensor)
	{
		switch (ComboBox_type->ItemIndex)
		{
			case 0:
			   ss.printf(L"IND3#%03d", ComboBox_addr->ItemIndex +1);
			break;

			case 1:
			   ss.printf(L"AND3#%03d", ComboBox_addr->ItemIndex +1);
			break;

			default:
			   ss.printf(L"UNKNOWN#%03d", ComboBox_addr->ItemIndex +1);
			break;
		}
	}

	sensor->set_name(ss.c_bstr());
	sensor->set_addr(ComboBox_addr->ItemIndex + 1);
	sensor->set_on(CheckBox_on->Checked);
	sensor->set_mark(Memo_mark->Text.c_str());
    sensor->set_type(ComboBox_type->ItemIndex);

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_Sensor_adjust::Timer_startTimer(TObject *Sender)
{
	Timer_start->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm_Sensor_adjust::Button_cancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

