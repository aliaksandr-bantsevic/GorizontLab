//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FMain.h"

#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#pragma comment(lib, "winmm.lib")
#include <algorithm>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_General* Form_General;
//---------------------------------------------------------------------------
__fastcall TForm_General::TForm_General(TComponent* Owner) : TForm(Owner)
{
    InitMainWindow();
}
//---------------------GLOBALS-----------------------------------------------

TGLSystem* GLSystem;
TDateTime g_global_second_timer = 0.;
WideString g_ws_msg;
int g_global_system_mode = SYS_MODE_ADJ;

TIMECAPS tc;
UINT wTimerRes;
//--------------------------------------------------------------------------
void TForm_General::SetGeneralCaption(void)
{
    WideString cap;
    TCHAR* cp = GLSystem->GetConfPath();
    cap.printf(L"GorizontLab [%s]", cp);
    Caption = cap;
}

void TForm_General::InitMainWindow(void)
{
    this->WindowState = wsMaximized;
    DevideMainWindow(30, 85);

    // ����������� ���������� ������� 1 ��
    TIMECAPS tc;
    UINT wTimerRes;

    if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) {
        //std::cerr << "������ ��� ��������� ������������ �������." << std::endl;
        StatusBar->Panels->Items[3]->Text =
            L"������ ��� ��������� ������������ �������.";
    } else {
        //wTimerRes = std::min(std::max(tc.wPeriodMin, 1), tc.wPeriodMax);
        wTimerRes = 1;
        // ������������� ���������� �������
        timeBeginPeriod(wTimerRes);
        StatusBar->Panels->Items[3]->Text =
            L"��������� ������ ���������� �� ���������� 1 ��.";
    }
}

void TForm_General::DevideMainWindow(int browser_part, int data_part)
{
    int hor_res = Form_General->Width;
    int ver_res = Form_General->Height;

    int browser_width = hor_res * browser_part / 100;
    int data_height = ver_res * data_part / 100;

    this->Panel_Browser->Width = browser_width;
    this->Panel_Data->Height = data_height;
}

void __fastcall TForm_General::Timer_General_1sTimer(TObject* Sender)
{
    //Show Current time
    StatusBar->Panels->Items[0]->Text = GetCurrentTimeStr();

    //Show global time
    StatusBar->Panels->Items[1]->Text =
        GetGlobalSecondTimerStr(&g_global_second_timer);
}
//---------------------------------------------------------------------------

void TForm_General::InitApplication(void)
{
    // FMaincaptionPointer& = Form_General->Caption;

    GLSystem = new TGLSystem(TreeView_Browser, XMLDocument_conf);
    GLSystem->set_console(ListBox_console);
    GLSystem->console(L"����������", L"������������� ...");

    WideString s;
    s.printf(L"������: %d.%d.%d", BUILD, VERSION, SUBVERSION);

    StatusBar->Panels->Items[2]->Text = s;

    GLSystem->LoadConf();

    //WideString cap;
    //TCHAR* cp = GLSystem->GetConfPath();
    //cap.printf(L"GorizontLab [%s]", cp);
    //Caption = cap;

    SetGeneralCaption();

    GLSystem->console(L"����������", L"... �������");
}

void __fastcall TForm_General::Timer_Init_appTimer(TObject* Sender)
{
    Timer_Init_app->Enabled = false;
    InitApplication();
}
//---------------------------------------------------------------------------

int pnum = 1;

void __fastcall TForm_General::ToolButton1Click(TObject* Sender)
{
    GLSystem->SaveConf();
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

void __fastcall TForm_General::TreeView_BrowserClick(TObject* Sender)
{
    //this->GLSystem->

    TTreeNode* nd = NULL;
    nd = TreeView_Browser->Selected;

    int type = OBJ_TYPE_NONE;

    GLSystem->ProcBrowser�lick(nd, &type);

    HidePopupMenu();

    switch (type) {
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
            this->N_AdjustPort->Visible = true;
            this->N_AddSensor->Visible = true;
            break;

        case OBJ_TYPE_SNSR:
            this->N_DeleteSensor->Visible = true;
            this->N_AdjustSensor->Visible = true;
            break;

        default:;
    }

    ShowSystemElement(g_ws_msg);
}
//---------------------------------------------------------------------------
void __fastcall TForm_General::PopupMenuPopup(TObject* Sender)
{
    TreeView_BrowserClick(NULL);
}
//---------------------------------------------------------------------------
void TForm_General::ShowSystemElement(WideString s)
{
    StatusBar->Panels->Items[3]->Text = s;
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::N_AddPortClick(TObject* Sender)
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

void __fastcall TForm_General::N_AddPlaceClick(TObject* Sender)
{
    AddPlace();
}
//---------------------------------------------------------------------------
void TForm_General::AddPort(void)
{
    Form_Port_adjust->start(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::N_AddSensorClick(TObject* Sender)
{
	AddSensor();
    GLSystem->ReDraw();
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::ToolButton4Click(TObject* Sender)
{

	//GLSystem->bbf_save_sensor_data_s(Now(), GLSystem->get_cur_sensor());

	//return;

	GLSystem->open_DB();

    double x = 23.456;
	double y = 569.567;

    TDateTime tt = Now();
    TDateTime t1 = Now();

	for (int i = 0; i < 10000; i++) {

		//GLSystem->bbf_store_sensor_data(tt);
		GLSystem->store_sensor_data(tt);
        tt += T_ONE_SEC;
		x += 1.23;
        y += 4.56;
    }

    TDateTime t2 = Now();

    double dt = (double)(t2 - t1) / 4. / 10000. / T_ONE_MSEC;

    WideString msg;

    msg.printf(L"dt = %f", dt);
    GLSystem->console(L"sys", msg);
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::ToolButton3Click(TObject* Sender)
{
    GLSystem->open_DB();

    TGLSensor* sn = GLSystem->get_cur_sensor();

    TDateTime t1 = TDateTime(2025, 2, 3);
    TDateTime t2 = Now();

    std::list<dt_sensor_data_record_s> list;

	list = GLSystem->read_sensor_data_s(sn, t1, t2);
	//list = GLSystem->bbf_read_sensor_data_s(sn, t1, t2);

	int i = 0;

    for (auto r : list) {
        i++;

        TDateTime t = r.t;

        Sleep(1);
    }

	Sleep(1);
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::FormClose(TObject* Sender, TCloseAction &Action)
{
    GLSystem->SaveConf();
    // ��������������� ���������� �������
    timeEndPeriod(wTimerRes);
    StatusBar->Panels->Items[3]->Text = L"��������� ������ ������������.";
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::N_AdjustSensorClick(TObject* Sender)
{
    AdjustSensor();
}
//---------------------------------------------------------------------------
void TForm_General::AdjustSensor(void)
{
    Form_Sensor_adjust->start(GLSystem->GetCurSn());
}
void __fastcall TForm_General::N_DeleteSensorClick(TObject* Sender)
{
    GLSystem->delete_sensor();
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::N_DeletePortClick(TObject* Sender)
{
    GLSystem->delete_port();
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::N_DeletePlaceClick(TObject* Sender)
{
    GLSystem->delete_place();
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::ToolButton2Click(TObject* Sender)
{
    GLSystem->CreateConf(SaveDialog);
    SetGeneralCaption();
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::ToolButton5Click(TObject* Sender)
{
    GLSystem->OpenConf(OpenDialog);
    SetGeneralCaption();
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::ToolButton6Click(TObject* Sender)
{
    GLSystem->SaveConf(SaveDialog);
    SetGeneralCaption();
}
//---------------------------------------------------------------------------

