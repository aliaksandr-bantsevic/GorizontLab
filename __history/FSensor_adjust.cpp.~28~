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

	ComboBox_rate->Clear();
	ComboBox_rate->Items->Add(1);
	ComboBox_rate->Items->Add(5);
	ComboBox_rate->Items->Add(10);
	ComboBox_rate->Items->Add(30);
	ComboBox_rate->Items->Add(60);
	ComboBox_rate->ItemIndex = 0;


	ComboBox_addr->Clear();
	ComboBox_addr1->Clear();

	for (int i = 1; i < 255; i++)
	{
		ComboBox_addr->Items->Add(i);
		ComboBox_addr1->Items->Add(i);
	}

	ComboBox_addr->ItemIndex = 0;
	ComboBox_addr1->ItemIndex = 0;

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

	ComboBox_addr1->Visible = true;
	Edit_list->Visible = true;
	CheckBox_list->Visible = true;

	TabSheet_ind3->Enabled = false;
	TabSheet_and3->Enabled = false;

	if (sensor)
	{
	   WideString s;
	   Caption = L"Настроить датчик";
	   s.printf(L"%d", sensor->GetAddr());
	   ComboBox_addr->Text = s;

	   ComboBox_type->Enabled = false;
	   ComboBox_addr->Enabled = false;
	   ComboBox_addr1->Visible = false;
	   Edit_list->Visible = false;
	   CheckBox_list->Visible = false;

	   ComboBox_type->ItemIndex = sensor->get_type();
	   ComboBox_prt->ItemIndex = sensor->get_protocol_type();

	   if (sensor->get_sensor_type() == SENSOR_TYPE_IND3)
	   {
			TabSheet_ind3->Enabled = true;
			PageControl_ext->ActivePage = TabSheet_ind3;
	   }
	   else if (sensor->get_sensor_type() == SENSOR_TYPE_AND3)
	   {
            TabSheet_and3->Enabled = true;
			PageControl_ext->ActivePage = TabSheet_and3;
       }

	   Memo_mark->Text = sensor->get_mark();
	   CheckBox_on->Checked = sensor->get_on();

	   ComboBox_type->ItemIndex = sensor->get_type();
	   ComboBox_addr->ItemIndex = sensor->get_addr() - 1;

	   s.printf(L"%d", sensor->data_stream_rate);
	   ComboBox_rate->Text = s;
	}
	else
	{
	  Caption = L"Добавить датчик";
	  ComboBox_addr->Enabled = true;
    }

	Edit_list->Visible = false;

	ShowModal();
}


void __fastcall TForm_Sensor_adjust::Button_OKClick(TObject *Sender)
{
	WideString ss("");
	TGLSensor* sn;
	bool snew = false;
	std::list<BYTE> addr_list;
	addr_list.clear();

	BYTE ad1 = ComboBox_addr->Text.ToInt();
	BYTE ad2 = 0;

	Update();

	if (sensor == NULL)
	{
	  ad2 = ComboBox_addr1->Text.ToInt();
	  snew = true;

	  if (CheckBox_list->Checked)
	  {

		TCHAR* tstr = Edit_list->Text.c_str();
		std::vector<BYTE> result;
		parseString(tstr, result);
		// Преобразуем std::vector<BYTE> в BYTE массив
		BYTE* alisd = new BYTE[result.size()];
		std::copy(result.begin(), result.end(), alisd);
		// Выводим результат
		for (size_t i = 0; i < result.size(); ++i)
		{
			wprintf(L"%d ", alisd[i]);
			if (GLSystem->GetCurPr()->sensor_exist(alisd[i]) == false) addr_list.push_back(alisd[i]);
		}
		// Освобождаем память
		delete[] alisd;
	  }
	  else
	  for (BYTE a = ad1; a <= ad2; a++)
	  {
			if (GLSystem->GetCurPr()->sensor_exist(a) == false) addr_list.push_back(a);
	  }

	  /*
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
	  */

	}
	else
	{
		addr_list.push_back(ComboBox_addr->Text.ToInt());
    }


	for (auto ad : addr_list)
	{

		if (snew)
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
		//sensor->set_addr(ComboBox_addr->ItemIndex + 1);

		sensor->set_addr(ad);

		sensor->set_on(CheckBox_on->Checked);
		sensor->set_mark(Memo_mark->Text.c_str());
		sensor->set_type(ComboBox_type->ItemIndex);

		sensor->set_type(ComboBox_type->ItemIndex);
		sensor->set_protocol_type(ComboBox_prt->ItemIndex);
        sensor->data_stream_rate = ComboBox_rate->Text.ToInt();

		sensor->init_sensor();
	}

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

void __fastcall TForm_Sensor_adjust::ComboBox_addrChange(TObject *Sender)
{
    ComboBox_addr1->Text = ComboBox_addr->Text;
}
//---------------------------------------------------------------------------

void __fastcall TForm_Sensor_adjust::CheckBox_listClick(TObject *Sender)
{
	if (CheckBox_list->Checked)
	{
		Edit_list->Visible = true;
		ComboBox_addr1->Visible = false;
		ComboBox_addr->Visible = false;
	}
	else
	{
		Edit_list->Visible = false;
		ComboBox_addr1->Visible = true;
		ComboBox_addr->Visible = true;
	}
}
//---------------------------------------------------------------------------

