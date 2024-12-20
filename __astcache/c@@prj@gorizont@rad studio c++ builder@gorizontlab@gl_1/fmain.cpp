﻿//---------------------------------------------------------------------------

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

TGLSystem* 			GLSystem;
TDateTime 			g_global_second_timer = 0.;
WideString 			g_ws_msg;
int                 g_global_system_mode = SYS_MODE_ADJ;

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




void __fastcall TForm_General::Timer_General_1sTimer(TObject *Sender)
{
	//Show Current time
	StatusBar->Panels->Items[0]->Text = GetCurrentTimeStr();

	//Show global time
	StatusBar->Panels->Items[1]->Text = GetGlobalSecondTimerStr(&g_global_second_timer);
}
//---------------------------------------------------------------------------

void TForm_General::InitApplication(void)
{
   GLSystem = new TGLSystem(TreeView_Browser);

   WideString s;
   s.printf(L"Версия: %d.%d.%d", BUILD, VERSION, SUBVERSION);

   StatusBar->Panels->Items[2]->Text = s;
}

void __fastcall TForm_General::Timer_Init_appTimer(TObject *Sender)
{
	Timer_Init_app->Enabled = false;
	InitApplication();
}
//---------------------------------------------------------------------------

int pnum = 1;

void __fastcall TForm_General::ToolButton1Click(TObject *Sender)
{
	WideString ss;
	ss.printf(L"Place #%d", pnum);
	GLSystem->add_place(ss);
	pnum++;
}
//---------------------------------------------------------------------------
void TForm_General::HidePopupMenu(void)
{

	this->N_AddPlace->Visible = false;
	this->N_DeletePlace->Visible = false;
	this->N_AddPort->Visible = false;

	this->N_DeletePort->Visible = false;
	this->N_AddSensor->Visible = false;
	this->N_DeleteSensor->Visible = false;

	this->N_Port->Visible = false;
	this->N_Sensor->Visible = false;

	this->N_AdjustPlace->Visible = false;
	this->N_AdjustPort->Visible = false;
	this->N_AdjustSensor->Visible = false;
	this->N_AdjustSystem->Visible = false;
}
//---------------------------------------------------------------------------


void __fastcall TForm_General::TreeView_BrowserClick(TObject *Sender)
{
	//this->GLSystem->

	TTreeNode* nd = NULL;
	nd = TreeView_Browser->Selected;

	int type = OBJ_TYPE_NONE;

	GLSystem->ProcBrowserСlick(nd, &type);

	HidePopupMenu();

		switch (type)
		{
			case OBJ_TYPE_SYST:
				this->N_AddPlace->Visible = true;
				this->N_AdjustSystem->Visible = true;
			break;

			case OBJ_TYPE_PLCE:
              	this->N_AdjustPlace->Visible = true;
				this->N_DeletePlace->Visible = true;
				this->N_AddPort->Visible = true;
			break;

			case OBJ_TYPE_PORT:
				this->N_DeletePort->Visible = true;
				this->N_AddSensor->Visible = true;
			break;

			case OBJ_TYPE_SNSR:
				this->N_DeleteSensor->Visible = true;
			break;

			default:;

		}

		ShowSystemElement(g_ws_msg);
}
//---------------------------------------------------------------------------
void __fastcall TForm_General::PopupMenuPopup(TObject *Sender)
{
	TreeView_BrowserClick(NULL);
}
//---------------------------------------------------------------------------
void TForm_General::ShowSystemElement(WideString s)
{
   StatusBar->Panels->Items[3]->Text = s;
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::N_AddPortClick(TObject *Sender)
{
	 AddPort();
}
//---------------------------------------------------------------------------

void TForm_General::AddPlace(void)
{
	Form_Place_adjust->start(NULL);
}
//---------------------------------------------------------------------------

void TForm_General::AddSensor(void)
{
	Form_Sensor_adjust->start(NULL);
}

//---------------------------------------------------------------------------

void __fastcall TForm_General::N_AddPlaceClick(TObject *Sender)
{
	AddPlace();
}
//---------------------------------------------------------------------------
void TForm_General::AddPort(void)
{
	Form_Port_adjust->start(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::N_AddSensorClick(TObject *Sender)
{
	AddSensor();
}
//---------------------------------------------------------------------------

