//---------------------------------------------------------------------------

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

	/*
	ComboBox_baud->Clear();

	ComboBox_baud->Items->Add(4800);
	ComboBox_baud->Items->Add(9600);
	ComboBox_baud->Items->Add(19200);
	ComboBox_baud->Items->Add(38400);
	ComboBox_baud->Items->Add(115200);

	ComboBox_baud->ItemIndex = 1;
    */

	if (sensor)
	{
	   WideString s;
	   //s.printf(L"%d", sensor->GetBaud());
	   //ComboBox_baud->Text = s;
	   Caption = L"Настроить датчик";
	   s.printf(L"%d", sensor->GetAddr());
	   ComboBox_addr->Text = s;
	   ComboBox_addr->Enabled = false;
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
	WideString ssens;
	ssens.printf(L"ind3#%03d", ComboBox_addr->ItemIndex + 1);
	TGLSensor* sn;

	Update();

	if (sensor == NULL)
	{

	  sn = GLSystem->add_sensor(ssens, GLSystem->get_sens_uid_max());
	  if (sn == NULL)
	  {
		  ShowMessage(L"Не удалось добавить датчик!");
		  Close();
	  }
	  else
	  {
			sn->SetAddr(ComboBox_addr->Text.ToInt());
			//sn->SetBaud(ComboBox_baud->Text.ToInt());
	  }
	}
	else
	{
			sn = sensor;
			//sn->SetBaud(ComboBox_baud->Text.ToInt());
    }

   //	DWORD b = (DWORD)ComboBox_baud->Text.ToInt();



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

