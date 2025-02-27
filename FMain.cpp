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
    //InitMainWindow();
}
//---------------------GLOBALS-----------------------------------------------

TGLSystem* GLSystem;
TMainMonitorThread*  MainMonitorThread;

TDateTime g_global_second_timer = 0.;
TDateTime g_monitor_second_timer = Now();

WideString g_ws_msg;
int g_global_system_mode = SYS_MODE_ADJ;

TIMECAPS tc;
UINT wTimerRes;

TDateTime g_time_all_data_request = Now();
bool g_system_monitoring_run = false;

/*
TDataStream* dsx = NULL;
TDataStream* dsy = NULL;
TGLSensor* sn_0 = NULL;
TChartThread* tcx = NULL;
TChartThread* tcy = NULL;
*/

std::list<TChartThread*> g_tc_list;

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
	DevideMainWindow(20, 70);

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
	Panel_charts->Visible = true;

	int hor_res = Form_General->Width;
	int ver_res = Form_General->Height;

	int browser_width = hor_res * browser_part / 100;
	int data_height = ver_res * data_part / 100;

	this->Panel_Browser->Width = browser_width;
	this->Panel_Data->Height = data_height;

	Panel_ch1->Height = (Panel_charts->Height - ToolBar_chart->Height)/2;
	//Panel_ch1->Height = (Panel_charts->Height)/2;

    Panel_charts->Visible = false;
}

#include <windows.h>
#include <psapi.h>
#include <iostream>

void __fastcall TForm_General::Timer_General_1sTimer(TObject* Sender)
{
	WideString smonsectime = FormatDateTime(L"yyyy-mm-dd hh:mm:ss:zzz", g_monitor_second_timer);

	//Show Current time
    StatusBar->Panels->Items[0]->Text = smonsectime;//GetCurrentTimeStr();

	//Show global time
	StatusBar->Panels->Items[1]->Text =
		GetGlobalSecondTimerStr(&g_global_second_timer);

	WideString ss("");

	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
	{
		double dm = ((double)pmc.WorkingSetSize) / 1048576.;
		ss.printf(L"mem = %.2f Mb", dm);
		StatusBar->Panels->Items[5]->Text = ss;
	}

}
//---------------------------------------------------------------------------

void TForm_General::InitApplication(void)
{
	// FMaincaptionPointer& = Form_General->Caption;

	GLSystem = new TGLSystem(TreeView_Browser, XMLDocument_conf);
	MainMonitorThread = new TMainMonitorThread(false , GLSystem);

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
	GLSystem->view_ports_status(ListView_ports);
	GLSystem->view_sensors_status(ListView_sensors);
	GLSystem->view_data_status(ListView_data);
	InitMainWindow();

	/*
		if (tcx == NULL) {

	tcx = new TChartThread(true, Chart1, dsx);
	tcy = new TChartThread(true, Chart2, dsy);

	} */
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

//#include "Protocol_MODBUS_RTU.h"

void __fastcall TForm_General::ToolButton4Click(TObject* Sender)
{

	//TProtocol_Modbus_RTU* prt;

	//prt = new TProtocol_Modbus_RTU;

return;

   //GLSystem->view_ports_status(ListView_ports);
   //GLSystem->view_sensors_status(ListView_sensors);
   GLSystem->view_data_status(ListView_data);

return;

TGLPort* pr = GLSystem->GetCurPr();

	pr->start_cycle();
	pr->cycle();

	return;

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

	//GLSystem->update_view_ports_status();
	//GLSystem->update_view_sensors_status();

	MainMonitorThread->resume();
	return;

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
	MainMonitorThread->suspend(); Sleep(100); MainMonitorThread->exit(); Sleep(100);

    GLSystem->SaveConf();
    // ��������������� ���������� �������
    timeEndPeriod(wTimerRes);
    StatusBar->Panels->Items[3]->Text = L"��������� ������ ������������.";
}
//---------------------------------------------------------------------------

void __fastcall TForm_General::N_AdjustSensorClick(TObject* Sender)
{
	AdjustSensor();
    GLSystem->ReDraw();
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

void __fastcall TForm_General::N_AdjustPortClick(TObject *Sender)
{
	AdjustPort();
}
//---------------------------------------------------------------------------

void TForm_General::AdjustPort(void)
{
   Form_Port_adjust->start(GLSystem->GetCurPr());
   GLSystem->ReDraw();
}
void __fastcall TForm_General::TreeView_BrowserDblClick(TObject *Sender)
{
  	TTreeNode* nd = NULL;
	nd = TreeView_Browser->Selected;
	//nd->Expand(!nd->Expanded);

	//nd->Expanded = (!nd->Expanded);

	/*
	if (nd->Expanded)
	{
		TreeView_Browser->AutoExpand = true;
	}
	else
	{
	   TreeView_Browser->AutoExpand = false;
	}
	*/

	int type = OBJ_TYPE_NONE;

	GLSystem->ProcBrowser�lick(nd, &type);



	switch (type)
	{

		case OBJ_TYPE_SYST:

			break;

		case OBJ_TYPE_PLCE:

			break;

		case OBJ_TYPE_PORT:

			AdjustPort();
			//nd = GLSystem->GetCurPr()->GetNode();

			break;

		case OBJ_TYPE_SNSR:
			AdjustSensor();
			break;

		default:

			;
	}


}
//---------------------------------------------------------------------------

void __fastcall TForm_General::TreeView_BrowserMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{   /*
    if (Button == mbLeft && Shift.Contains(ssDouble)) {
        TTreeView* treeView = dynamic_cast<TTreeView*>(Sender);
        if (treeView) {
            TTreeNode* node = treeView->GetNodeAt(X, Y);
            if (node) {
                // ������� ������ �� ���� ������
				node->Expand(  !node->Expanded);
            }
        }
	} */
}

//---------------------------------------------------------------------------


void __fastcall TForm_General::ToolButton_startClick(TObject *Sender)
{
		refresh_system_dashboard();

		if (g_system_monitoring_run == false)
		{
			g_system_monitoring_run = true;
			MainMonitorThread->resume();
			ToolButton_start->ImageIndex = 8;
			StatusBar->Panels->Items[4]->Text = L"monitoring is running ...";
			TreeView_Browser->Enabled = false;
            Panel_charts->Visible = true;
		}
		else
		{
			g_system_monitoring_run = false;
			MainMonitorThread->suspend();
			ToolButton_start->ImageIndex = 7;
			StatusBar->Panels->Items[4]->Text = L"monitoring STOP";
			TreeView_Browser->Enabled = true;
        }
}
//---------------------------------------------------------------------------

void TForm_General::refresh_system_dashboard(void)
{
	GLSystem->ports_list = NULL;
	GLSystem->sensors_list = NULL;
	GLSystem->data_list = NULL;

	GLSystem->view_ports_status(ListView_ports);
	GLSystem->view_sensors_status(ListView_sensors);
	GLSystem->view_data_status(ListView_data);

	GLSystem->update_view_sensors_status();
	GLSystem->update_data(Now());
	GLSystem->update_view_data_status();


	TGLSensor* sn;
	TDataStream* dsx = NULL;
	TDataStream* dsy = NULL;
	int idx = 0;


   if (sn = GLSystem->GetCurSn()) {

    	for (auto itds : sn->data_stream_list) {

			if (idx++ == 0)
			{
				dsx = itds;
			}
			else
			{
				dsy = itds;
			}
		}  }

		TChartThread* tcx = new TChartThread(false, Chart1, dsx);
		TChartThread* tcy = new TChartThread(false, Chart2, dsy);

		g_tc_list.clear();
		g_tc_list.push_back(tcx);
		g_tc_list.push_back(tcy);

}


void __fastcall TForm_General::ToolButton8Click(TObject *Sender)
{
	TGLSensor* sn;
	TDataStream* dsx = NULL;
	TDataStream* dsy = NULL;
	int idx = 0;


   if (sn = GLSystem->GetCurSn()) {

		for (auto itds : sn->data_stream_list) {

			if (idx++ == 0)
			{
				dsx = itds;
			}
			else
			{
				dsy = itds;
			}
		}

		TChartThread* tcx = new TChartThread(false, Chart1, dsx);
		//TChartThread* tcy = new TChartThread(false, Chart2, dsy);

		g_tc_list.clear();
		g_tc_list.push_back(tcx);
		//g_tc_list.push_back(tcy);
   }
}
//---------------------------------------------------------------------------

