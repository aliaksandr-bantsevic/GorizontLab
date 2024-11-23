//---------------------------------------------------------------------------
//#define SET_DEBUG_MODE

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include <Winuser.h>
#include <string.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFMain *FMain;
//---------------------------------------------------------------------------
__fastcall TFMain::TFMain(TComponent* Owner)
	: TForm(Owner)
{

	ApplicationInit();
	SystemInit();


#ifdef DEBUG_DEVELOPMENT_MODE
   ToolButton_test->Visible = true;
#else
   ToolButton_test->Visible = false;
#endif

}
//---------------------------------------------------------------------------


//----------------------------Globals----------------------------------------

bool b_MessageConfirmParamsetShow = false;

TPlace* 	current_place = NULL;
int     	current_place_idx = 0;

TDrill* 	current_drill = NULL;
int     	current_drill_idx = 0;

TDrill* 	selected_drill = NULL;
int     	selected_drill_idx = 0;

TMeas*		current_meas = NULL;
int			current_meas_idx = 0;

TMeas*		selected_meas = NULL;
int			selected_meas_idx = 0;

TStringGrid* global_meas_table = NULL;
bool 		b_system_start = true;


TChartThread* Tc_X_h = NULL;
TChartThread* Tc_Y_h = NULL;

TChartThread* Tc_X_v = NULL;
TChartThread* Tc_Y_v = NULL;
TChartThread* Tc_R_v = NULL;

int screen_mode = 0;
int i_table_col_width = 80;

TSysConfMgr* scmgr = NULL;
TStatusBar* main_status_bar = NULL;
double excel_export_rate = 0;

int g_xmax = 0;
int g_xmin = 0;
int g_ymax = 0;
int g_ymin = 0;
int g_lx = 0;
int g_ly = 0;

bool global_base_convert = false;

//---------------------------------------------------------------------------


void __fastcall TFMain::MMFileDrawItem(TObject *Sender, TCanvas *ACanvas, TRect &ARect,
          bool Selected)
{

	ACanvas->FillRect(ARect);
	ACanvas->Font->Name = "Arial";
	ACanvas->Font->Size = 10;
	ACanvas->Font->Style=ACanvas->Font->Style<<fsBold;

	ACanvas->TextRect(ARect, ARect.Left + 3, ARect.Top + 3, "Ôàéë   ");


}
//---------------------------------------------------------------------------


void TFMain::SetMenuFont(TRect &ARect, TCanvas* ACanvas, WideString text)
{
	//ACanvas->FillRect(ARect);
	ACanvas->Font->Name = "Arial";
	ACanvas->Font->Size = 10;
	ACanvas->Font->Style=ACanvas->Font->Style<<fsBold;
	ACanvas->TextRect(ARect, ARect.Left + 3, ARect.Top + 3, text);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::MMPropDrawItem(TObject *Sender, TCanvas *ACanvas, TRect &ARect,
		  bool Selected)
{
	SetMenuFont(ARect, ACanvas, "Настройки   ");
}
//---------------------------------------------------------------------------

void __fastcall TFMain::MMViewDrawItem(TObject *Sender, TCanvas *ACanvas, TRect &ARect,
		  bool Selected)
{
	SetMenuFont(ARect, ACanvas, "Вид   ");
}
//---------------------------------------------------------------------------

void __fastcall TFMain::MMToolsDrawItem(TObject *Sender, TCanvas *ACanvas, TRect &ARect,
          bool Selected)
{
	SetMenuFont(ARect, ACanvas, "Инструменты   ");
}
//---------------------------------------------------------------------------

void __fastcall TFMain::MMHelpDrawItem(TObject *Sender, TCanvas *ACanvas, TRect &ARect,
		  bool Selected)
{
	SetMenuFont(ARect, ACanvas, "Справка   ");
}
//---------------------------------------------------------------------------


void __fastcall TFMain::ToolButton_createClick(TObject *Sender)
{
    ClearTable();
	GBISystem->CreateConf(SaveDialog);
	SystemInit();
}
//---------------------------------------------------------------------------


void __fastcall TFMain::ToolButton9Click(TObject *Sender)
{
   this->GBISystem->GetPort()->Close();
   Form_PortAdjust->Setup(GBISystem);
   Form_PortAdjust->StartTimer();
   Form_PortAdjust->ShowModal();
   this->GBISystem->Redraw();
}
//---------------------------------------------------------------------------

void __fastcall TFMain::ToolButton8Click(TObject *Sender)
{
   Form_SensorAdjust->Setup(GBISystem);
   Form_SensorAdjust->StartTimer();
   Form_SensorAdjust->ShowModal();

}
//---------------------------------------------------------------------------

void __fastcall TFMain::ToolButton_syspropClick(TObject *Sender)
{
	Form_SystemAdjust->system = this->GBISystem;
	Form_SystemAdjust->Start();
	Form_SystemAdjust->ShowModal();
}
//---------------------------------------------------------------------------





void __fastcall TFMain::ToolButton_StartClick(TObject *Sender)
{

	if (selected_meas == NULL) {

		utils_ShowMessage(L"Выберите измерение в дереве системы!");
		return;
	}


	if (GBISystem->IsRunning() == false) {
		this->MToolBar->Buttons[0]->ImageIndex = 10;

		this->GBISystem->Start(Edit_measX,Edit_measY);
		this->Timer_system_run->Enabled = true;

		Button_record->Enabled = true;

		Console(L"Система", L"Запуск инклинометра");
		this->MToolBar->Buttons[0]->Caption = L"Стоп";
		MMFile->Enabled = false;



		if (selected_meas != NULL) {

			TDrill* d = NULL;
			d = GBISystem->place_list[selected_meas->pnum-1]->drill_list[selected_meas->dnum-1];

			StringGrid_meas->Col = 3;

			if (d->i_first_request_point == 1) {

				StringGrid_meas->Row = selected_meas->records_cnt;
			 }
			 else
			 {
				StringGrid_meas->Row = 1;
			 }

		}


	}
	else
	{
		this->MToolBar->Buttons[0]->ImageIndex = 9;
		this->MToolBar->Buttons[0]->Caption = L"Старт";

		this->GBISystem->Stop();
		this->Timer_system_run->Enabled = false;

		Button_record->Enabled = false;

		Console(L"Система", L"Останов инклинометра");
		MMFile->Enabled = true;
	}

}
//---------------------------------------------------------------------------

TFMain* fm;

void console(WideString obj, WideString msg)
{
	fm->Console(obj, msg);
}

void TFMain::SystemInit()
{
	fm = this;

	Console(L"Приложение",L"Старт системы ...");

	GBISystem = new TGBISystem(this->TreeView_system);
	GBISystem->console = console;
	scmgr = &GBISystem->SysConfMgr;
	GBISystem->ask_save_conf = &b_MessageConfirmParamsetShow;
	GBISystem->LoadSysConf();
	global_meas_table = StringGrid_meas;

	GBISystem->edit_SKO = Edit_sko;
	GBISystem->ViewSKOlimit();

	TrackBar_sko->Min = 1;
	TrackBar_sko->Max = 1000;
	TrackBar_sko->Position = GBISystem->sko_limit;

	this->Timer_system_run->Interval = GBISystem->ask_sensor_period;

	GBISystem->LoadData();

	WideString stitle(L"");
	stitle.printf(L"GBI [%s]" ,GBISystem->SysConfMgr.GetCurIniPath());
	this->Caption = stitle;

	Console(L"Приложение",L"Инициалиация системы прошла успешно");

	b_system_start = false;

	if (GBISystem->SysConfMgr.auto_backup_flag_completed) {

		Console(L"Приложение",L"Автоматическое резервное копирование БД выполнено");
	}

#ifdef SET_DEBUG_MODE
	ToolButton_test->Visible = true;
#endif

#ifndef SET_DEBUG_MODE
	ToolButton_test->Visible = false;
#endif

}


//---------------------------------------------------------------------------

void TFMain::Console(WideString obj, WideString msg)
{
	TDateTime t = Now();
	WideString s(L"");
	s=t.FormatString(L"dd-mm-yyyy hh:nn:ss ") +L"["+obj+"]" + L" " + msg;
	this->ListBox_Console->Items->Add(s);


	TCHAR log[1024];
	wcscpy(log,s.c_bstr());
	wcscat(log,L"\r\n");

	TCHAR dir[1024];
	GetCurrentDirectoryW(1024,dir);
	wcscat(dir,L"\\gbi16.log");

	CreateTextFile_UTF16LEBOM(dir);
	FILE* fl = _wfopen(dir,L"ab");
	fwrite(log,1,wcslen(log)*2,fl);
	fclose(fl);

}
//---------------------------------------------------------------------------


void TFMain::ApplicationInit()
{
	//Version view
	::FormatSettings.DecimalSeparator = '.';
	main_status_bar = MStatusBar;

	DevideScreen(screen_mode);
	WideString sver(L"");
	sver.printf(L"Версия: %d.%d.%d", BUILD, VERSION, SUBVERSION);
	MStatusBar->Panels->Items[1]->Text = sver;
	Button_record->Enabled = false;

	Tc_X_h = new TChartThread (false, this->Chart_x_h, 0, 0);
	Tc_Y_h = new TChartThread (false, this->Chart_y_h, 1, 1);

	Tc_X_v = new TChartThread (false, this->Chart_x_v, 0, 2);
	Tc_Y_v = new TChartThread (false, this->Chart_y_v, 1, 3);
	Tc_R_v = new TChartThread (false, this->Chart_r, 2, 4);

	this->TabSheet_charts->TabVisible = false;
	this->TabSheet_charts_v->TabVisible = false;


	TStringGrid* table = 	this->StringGrid_meas;

	table->RowCount = MAX_RECORDS_MEAS+1; table->ColCount = 13;

   	table->ColWidths[0]= i_table_col_width/2;

	for (int i = 1; i<13; i++) {


	table->ColWidths[i]= i_table_col_width;


	}


	for (int i = 0; i<MAX_RECORDS_MEAS; i++) {

	table->RowHeights[i]= 20;

	}

	table->FixedCols = 1;
	table->FixedRows = 1;

	Timer_set_screen->Enabled = true;
 }
//---------------------------------------------------------------------------


void TFMain::CurrentTimeView()
{
	WideString stime(L"");
	TDateTime t = Now();
	stime = t.FormatString(L"dd-mm-yyyy   hh:mm:ss");
	MStatusBar->Panels->Items[0]->Text = stime;
}
//---------------------------------------------------------------------------

void __fastcall TFMain::Timer_seconds_system_eventTimer(TObject *Sender)
{
	CurrentTimeView();
}
//---------------------------------------------------------------------------


void TFMain::Test1()
{


}
//---------------------------------------------------------------------------

void __fastcall TFMain::FormDestroy(TObject *Sender)
{

		this->MToolBar->Buttons[0]->ImageIndex = 9;
		this->MToolBar->Buttons[0]->Caption = "Старт";

		this->GBISystem->Stop();
		this->Timer_system_run->Enabled = false;

		Button_record->Enabled = false;

		Console(L"Система", L"Останов инклинометра");
		MMFile->Enabled = true;


		VirtKey_TouchIn_Off();
		GBISystem->SaveSysConf();

		Sleep(100);

		GBISystem->GetPort()->Close();

        Sleep(100);
}
//---------------------------------------------------------------------------


int it = 111;
int irow = 2;
int icol = 2;


bool comerr = true;

void __fastcall TFMain::Timer_system_runTimer(TObject *Sender)
{
		this->Timer_system_run->Enabled = false;

		if (GBISystem->RunProc() != 0)
		{
			if (comerr == true) Console(L"Система", L"Не удалось открыть СОМ-порт");

			comerr = false;
		}
		else
		{
			if (comerr == false) {

				 Console(L"Система", L"Есть ответ СОМ-порта");
			}

			comerr = true;
		}

		this->Timer_system_run->Enabled = true;
}
//---------------------------------------------------------------------------


bool b_table_cleared = false;

void __fastcall TFMain::StringGrid_measDrawCell(TObject *Sender, int ACol, int ARow,
		  TRect &Rect, TGridDrawState State)
{

	WideString s(L"");

	if(ACol==0||((ARow==0)&&(ACol<13))){

	   StringGrid_meas->Canvas->Brush->Color=clSilver;
	   StringGrid_meas->Canvas->FillRect(Rect);


	}

	if(ACol>7){

	   StringGrid_meas->Canvas->FillRect(Rect);
	   StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, StringGrid_meas->Cells[ACol][ARow] );
	}



	if (ARow == 0) {

			StringGrid_meas->Canvas->Font->Color = clBlack;

			if (ACol == 0) StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, L"Номер");
			if (ACol == 1) StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, L"Уровень");
			if (ACol == 2) StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, L"Погр.трб.");

			if (ACol == 3) StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, L"X1");
			if (ACol == 4) StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, L"Y1");

			if (ACol == 5) StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, L"X2");
			if (ACol == 6) StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, L"Y2");

			if (ACol == 7) StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, L"Xрез");
			if (ACol == 8) StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, L"Yрез");

			if (ACol == 9) StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, L"LX");
			if (ACol == 10) StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, L"LY");
			if (ACol == 11) StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, L"Рез.см.");
			if (ACol == 12) StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, L"Рез.уг.");

		}

	if (ACol == 0) {

			StringGrid_meas->Canvas->Font->Color = clBlack;

			for (int i = 0; i < MAX_RECORDS_MEAS; i++) {

				if (ARow ==  i+1)
				{
					s.printf(L"%d",i);
					StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, s);
				}

			}

		}


	if ((ACol >1)&&(ACol <8)&&(ARow>0)) {

	   WideString sc = StringGrid_meas->Cells[ACol][ARow];

	   StringGrid_meas->Canvas->Brush->Color = clWhite;

	   StringGrid_meas->Canvas->FillRect(Rect);

	   StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, sc);
	}


	if (

			(ACol == StringGrid_meas->Col)
			&&
			(ARow == StringGrid_meas->Row)
			&&
			((ACol == 3)||(ACol == 4)||(ACol == 5)||(ACol == 6))

		)
		{


	   WideString sc = StringGrid_meas->Cells[ACol][ARow];
	   StringGrid_meas->Canvas->Brush->Color = clLime;
	   StringGrid_meas->Canvas->FillRect(Rect);
	   StringGrid_meas->Canvas->TextOut(Rect.Left, Rect.Top, sc);



	}

	if (selected_meas == NULL) {

		if (b_table_cleared == false) {

			b_table_cleared = true;

			for (int i = 1; i < 13; i++) {

				for (int j = 1; j < 100; j++) {

					StringGrid_meas->Cells[i][j] = "";

				}

			}
		}
	}
	else
	{
		b_table_cleared = false;
	}


	if (selected_meas !=NULL )
	{

		if (selected_meas->type_drill == DRILL_ORIENT_HORIZONT)
		{
			if (((ACol == 2)||(ACol == 4)||(ACol == 6)||(ACol == 8)||(ACol == 10)||(ACol == 11)||(ACol == 12))&&(ARow>0))
			{
			   StringGrid_meas->Canvas->Brush->Color = clSilver;
			   StringGrid_meas->Canvas->FillRect(Rect);
			}

			if (selected_meas->single_way)
			{
				if (((ACol == 5)||(ACol == 11)||(ACol == 12))&&(ARow>1))
				{
				   StringGrid_meas->Canvas->Brush->Color = clSilver;
				   StringGrid_meas->Canvas->FillRect(Rect);
				}

			}

		}


	}

}
//---------------------------------------------------------------------------
double xprev = -31777.;
double yprev = -31777.;

void __fastcall TFMain::Button_recordClick(TObject *Sender)
{

	if (selected_meas == NULL) {

		utils_ShowMessage(L"Выберите измрение в дереве системы!");
		return;
	}


	if (selected_meas->finalized) {

		if (Application->MessageBoxW(L"Измерение уже финализировано, хотите повторит рассчет?",L"ВНИМАНИЕ!",1)!=IDOK) {
			return;
		}

		selected_meas->finalized = false;

		for (int i = 0; i < selected_meas->records_cnt; i++) {

			selected_meas->records[i].Xres = 0;
			selected_meas->records[i].Yres = 0;
			selected_meas->records[i].LX = 0;
			selected_meas->records[i].LY = 0;
		}

		ViewSelectedMeas();

	}


	  TSensor* sensor = GBISystem->GetSensor();
	  WideString s(L"");

	  int row = StringGrid_meas->Row;
	  int col = StringGrid_meas->Col;

			if (row>1)
			{
				if (StringGrid_meas->Cells[1][row] == "") {

					StringGrid_measDblClick(NULL);
				  Sleep(100);
				}
			}

	  if (col == 6) col = 5;
	  if (col == 4) col = 3;


	  bool probnotmoved = false;

	  if ((xprev != -31777.)&&(abs(sensor->curr_X - xprev) < 50.))
	  {
		 probnotmoved = true;
	  }

	   if ((yprev != -31777.)&&(abs(sensor->curr_Y - yprev) < 50.))
	  {
		 probnotmoved = true;
	  }



	  if (probnotmoved == true)
	  {
		  if (Application->MessageBoxW(L"Положение датчика не изменилось. Хотите повторно внести даные?",L"ВНИМАНИЕ!",1) != IDOK)
		  {
			return;
		  }
	  }


	   xprev = sensor->curr_X;
	   yprev = sensor->curr_Y;

	  s.printf(L"%.1f",sensor->curr_X);
	  StringGrid_meas->Cells[col][row] = s;

	  s.printf(L"%.1f",sensor->curr_Y);
	  StringGrid_meas->Cells[col+1][row] = s;

	  TDrill* d = NULL;

	  d = GBISystem->place_list[selected_meas->pnum-1]->drill_list[selected_meas->dnum-1];

	  if (d->i_first_request_point == 0) {

			if (StringGrid_meas->Row<selected_meas->records_cnt) StringGrid_meas->Row++;
			else
			{
			   StringGrid_meas->Row = 1;
			   StringGrid_meas->Col = 5;
			}
	  }
	  else
	  {
			if (StringGrid_meas->Row>1) StringGrid_meas->Row--;
			else
			{
			   StringGrid_meas->Row = selected_meas->records_cnt;
			   StringGrid_meas->Col = 5;
            }
	  }



	  selected_meas->SaveData(1);

}
//---------------------------------------------------------------------------


void __fastcall TFMain::StringGrid_measDblClick(TObject *Sender)
{

	if (selected_meas == NULL) {

		utils_ShowMessage(L"Выберите измерение в дереве системы!");
		return;
	}

	if ((StringGrid_meas->Col >= 3)&&(StringGrid_meas->Col <= 6)) {

		WideString s(L"nodata");
		Form_data_enter->s = s.c_bstr();
		Form_data_enter->Edit_data->Text = StringGrid_meas->Cells[StringGrid_meas->Col][StringGrid_meas->Row];
		Form_data_enter->ShowModal();

		double d = 0.0;


		if (s!="nodata") {

				try {

						d = s.ToDouble();

					} catch (...) {

					}

		s.printf(L"%.1f",d);

			StringGrid_meas->Cells[StringGrid_meas->Col][StringGrid_meas->Row] = s;
			selected_meas->SaveData(1);
		}

		return;
	}

	if (StringGrid_meas->Col >2) {


		return;
	}

	if (StringGrid_meas->Row <=1 ) return;

	WideString s(L"");
	WideString s1(L"");

	s = StringGrid_meas->Cells[1][StringGrid_meas->Row];
	s1 = StringGrid_meas->Cells[2][StringGrid_meas->Row];

	if (s == "") {

		s = "0.0";
	}

	if (s1 == "") {

		s1 = "0.0";
	}


	TDrill* d =GBISystem->place_list[selected_meas->pnum-1]->drill_list[selected_meas->dnum-1];

	if (d->drill_orient == DRILL_ORIENT_HORIZONT) {

		s1 = "0.0";
		Form_DepthAdjust->Edit_trerr->Visible = false;
		Form_DepthAdjust->Label_tuberr->Visible = false;
	}
	else
	{
		Form_DepthAdjust->Edit_trerr->Visible = true;
		Form_DepthAdjust->Label_tuberr->Visible = true;
	}

	Form_DepthAdjust->str = s.c_bstr();
	Form_DepthAdjust->str1 = s1.c_bstr();

	Form_DepthAdjust->Edit_depth->Text = s;
	Form_DepthAdjust->Edit_trerr->Text = s1;

	Form_DepthAdjust->ShowModal();

	if (s!="") {


		if (s.ToDouble()) {

			this->StringGrid_meas->Cells[1][StringGrid_meas->Row] = s;

		}
		else
		{
			this->StringGrid_meas->Cells[1][StringGrid_meas->Row] = "";
		}

	}

	if (s1!="") {


		if (s1.ToDouble()) {

			this->StringGrid_meas->Cells[2][StringGrid_meas->Row] = s1;

		}
		else
		{
			this->StringGrid_meas->Cells[2][StringGrid_meas->Row] = "";
		}


	}

	if (selected_meas != NULL)
	{
		selected_meas->SaveData(1);
	}

}

//---------------------------------------------------------------------------

void __fastcall TFMain::PopupMenu_systemPopup(TObject *Sender)
{

	int type = 0;
	int idx = 0;
	void* obj = NULL;

	N_system_adjust->Visible = false;
	N_add_place->Visible = false;
	N_port_adjust->Visible = false;
	N_sensor_adjust->Visible = false;
	N_place_adjust->Visible = false;
	N_place_delete->Visible = false;

	N_drill_add->Visible = false;
	N_drill_delete->Visible = false;
	N_drill_adjust->Visible = false;

	N_meas_add->Visible = false;
	N_meas_delete->Visible = false;
	N_meas_adjust->Visible = false;

	if (GBISystem->CheckTreeItemSelected(&type, &idx, &obj) == 0)
	{

		if (type == TREE_ITEM_TYPE_SYSTEM) {

			N_system_adjust->Visible = true;
			N_add_place->Visible = true;

		}

		if (type == TREE_ITEM_TYPE_PORT) {

			N_port_adjust->Visible = true;
		}

		if (type == TREE_ITEM_TYPE_SENSOR) {

			N_sensor_adjust->Visible = true;
		}

		if (type == TREE_ITEM_TYPE_PLACE) {

			N_place_adjust->Visible = true;
			N_place_delete->Visible = true;
			N_drill_add->Visible = true;
			current_place = (TPlace*)obj;
			current_place_idx = idx;
		}

		if (type == TREE_ITEM_TYPE_DRILL) {

			N_drill_adjust->Visible = true;
			N_drill_delete->Visible = true;
			N_meas_add->Visible = true;
			current_drill = (TDrill*)obj;
			current_drill_idx = idx;
		}

		if (type == TREE_ITEM_TYPE_MEAS) {

			//N_meas_adjust->Visible = true;
			N_meas_delete->Visible = true;

			current_meas = (TMeas*)obj;
			current_meas_idx = idx;
		}

	}

	return;
}
//---------------------------------------------------------------------------

void __fastcall TFMain::N_system_adjustClick(TObject *Sender)
{
   ToolButton_syspropClick(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::N3Click(TObject *Sender)
{
   ToolButton_syspropClick(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::N_add_placeClick(TObject *Sender)
{

	TPlace* place = new TPlace(L"Новая площадка");

	Form_PlaceAdjust->mode = 0;
	Form_PlaceAdjust->place = place;
	Form_PlaceAdjust->OK = false;

	Form_PlaceAdjust->Start();

    Form_PlaceAdjust->ShowModal();

	if (Form_PlaceAdjust->OK == true)
	{

			GBISystem->AddPlace(place->name);
			selected_meas = NULL;
			selected_meas_idx = 0;
	}

    delete place;
}
//---------------------------------------------------------------------------

void __fastcall TFMain::N_port_adjustClick(TObject *Sender)
{
	 ToolButton9Click(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::N_sensor_adjustClick(TObject *Sender)
{
	ToolButton8Click(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::N_place_adjustClick(TObject *Sender)
{

	Form_PlaceAdjust->place = current_place;
	Form_PlaceAdjust->mode = 1;
	Form_PlaceAdjust->OK = false;
	Form_PlaceAdjust->Start();

	Form_PlaceAdjust->ShowModal();

	if (Form_PlaceAdjust->OK == true)
	{
		GBISystem->KillBase();
		GBISystem->Redraw();
		GBISystem->ResaveData();
	}

}
//---------------------------------------------------------------------------

void __fastcall TFMain::N_place_deleteClick(TObject *Sender)
{
	WideString s(L"");

	s.printf(L"Хотите удалить площадку [%s] ?",current_place->GetName().c_bstr());

    WideChar mes[1024];
	StringToWideChar(s.c_bstr(),mes,1024);

	if (Application->MessageBoxW(mes,L"ВНИМАНИЕ!",1)!=IDOK) return;

	GBISystem->KillBase();

	GBISystem->DeletePlace(current_place,current_place_idx);

	GBISystem->ResaveData();
}
//---------------------------------------------------------------------------

void __fastcall TFMain::TreeView_systemDblClick(TObject *Sender)
{
	int type = 0;
	int idx = 0;
	void* obj = NULL;


	if (GBISystem->CheckTreeItemSelected(&type, &idx, &obj) == 0)
	{

		if (type == TREE_ITEM_TYPE_SYSTEM) {
			N_system_adjustClick(NULL);
		}

		if (type == TREE_ITEM_TYPE_PORT) {

			N_port_adjustClick(NULL);
		}

		if (type == TREE_ITEM_TYPE_SENSOR) {

			N_sensor_adjustClick(NULL);
		}

		if (type == TREE_ITEM_TYPE_PLACE) {

			current_place = (TPlace*)obj;
			current_place_idx = idx;
			N_place_adjustClick(NULL);
		}

		if (type == TREE_ITEM_TYPE_DRILL) {

			current_drill = (TDrill*)obj;
			current_drill_idx = idx;
			N_drill_adjustClick(NULL);
		}

		if (type == TREE_ITEM_TYPE_MEAS) {

			current_meas = (TMeas*)obj;
			current_meas_idx = idx;
			N_meas_adjustClick(NULL);
		}

	}


	return;

}
//---------------------------------------------------------------------------

void __fastcall TFMain::MMAboutClick(TObject *Sender)
{

	TCHAR cdate[64];
	WideString sd(L"");
	WideString sdate(L"");

	sdate=L"г. Москва, 2023";

	Form_about->Label_year->Caption = sdate;

	WideString s;
	s.printf(L"Версия: %d.%d.%d", BUILD, VERSION, SUBVERSION);
	Form_about->Label_version->Caption = s;

	Form_about->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFMain::N1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFMain::N_drill_addClick(TObject *Sender)
{
	TPlace* p = current_place;
	TDrill* d = new TDrill();
	int cnt = 21;

	Form_DrillAdjust->records_cnt = &cnt;
	Form_DrillAdjust->mode = 0;

	Form_DrillAdjust->OK = false;
	Form_DrillAdjust->drill = d;
	Form_DrillAdjust->Start();
	Form_DrillAdjust->ShowModal();

	if (Form_DrillAdjust->OK == true )
	{
		if (p->AddDrill(d) != 0)
		{
			delete d;
		}

		GBISystem->Redraw();
	}

	current_place = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFMain::N_drill_adjustClick(TObject *Sender)
{

	TDrill* d = new TDrill();
	WideString s(L"");
	int records_cnt = 0;

	int init_start = current_drill->start_point;

	current_place->UpdateDrill(d, current_drill);

	records_cnt = d->records_cnt;
	Form_DrillAdjust->records_cnt = &records_cnt;

	Form_DrillAdjust->drill = d;
	Form_DrillAdjust->mode = 1;


	Form_DrillAdjust->Start();

	Form_DrillAdjust->OK = false;
	Form_DrillAdjust->Start();
	Form_DrillAdjust->ShowModal();

	if (Form_DrillAdjust->OK == true)
	{

		current_place->UpdateDrill(current_drill, d);

		GBISystem->KillBase();

		GBISystem->Reindex();
		GBISystem->Redraw();
		GBISystem->ResaveData();

		if (init_start != current_drill->start_point)
		{
			if(Application->MessageBoxW(L"Параметры скважины изменились. Выполнить перерасчет всех измерений?",L"ВНИМАНИЕ!",1) == IDOK )
			{
			   current_drill->ReCalc();
			}
		}

	   current_drill->UpdateMeas();
	   this->ViewSelectedDrill();
   }

   delete d;
   current_drill = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFMain::N_drill_deleteClick(TObject *Sender)
{
	WideString s(L"");
	TDrill* d = current_drill;

	TPlace* p = GBISystem->place_list[d->pnum-1];

	s.printf(L"Удалить скважину [%s] ?",d->name.c_bstr());

    WideChar mes[1024];
	StringToWideChar(s.c_bstr(),mes,1024);

	if (Application->MessageBoxW(mes,L"Внимание!",1)!=IDOK) return;

	GBISystem->KillBase();

	p->DeleteDrill(current_drill,current_drill_idx);

	GBISystem->Redraw();

	GBISystem->ResaveData();

	current_drill = NULL;
	current_meas = NULL;
	current_place = NULL;

	selected_drill = NULL;
	selected_meas = NULL;

	GBISystem->Redraw();

}
//---------------------------------------------------------------------------

void __fastcall TFMain::N_meas_addClick(TObject *Sender)
{
	TDrill* d = current_drill;

	d->AddMeas(StringGrid_meas, L"Измерение");

	GBISystem->Redraw();


	if (d->meas_list_idx>1) {

		d->meas_list[d->meas_list_idx-1]->ImportDepthTable(d->meas_list[d->meas_list_idx-2]);
	}

	GBISystem->last_place_idx = d->meas_list[d->meas_list_idx-1]->pnum;
	GBISystem->last_drill_idx = d->meas_list[d->meas_list_idx-1]->dnum;

	d->meas_list[d->meas_list_idx-1]->SaveData(0);

	GBISystem->Redraw();
}
//---------------------------------------------------------------------------

void __fastcall TFMain::N_meas_adjustClick(TObject *Sender)
{
	if (selected_meas != NULL)
	{
		Form_MeasAdjust->meas = selected_meas;
		Form_MeasAdjust->StartShow();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFMain::N_meas_deleteClick(TObject *Sender)
{
	WideString s(L"");
	TMeas* m = current_meas;
	TPlace* p = GBISystem->place_list[m->pnum - 1];
	TDrill* d = p->drill_list[m->dnum - 1];

	s.printf(L"Удалить измерение [%s] ?",m->name.c_bstr());

    WideChar mes[1024];
	StringToWideChar(s.c_bstr(),mes,1024);

	if (Application->MessageBoxW(mes,L"Внимание!",1)!=IDOK) return;

	GBISystem->KillBase();

	d->DeleteMeas(current_meas,current_meas_idx);

	GBISystem->Redraw();

	GBISystem->ResaveData();

	GBISystem->last_place_idx = -1;
	GBISystem->last_drill_idx = -1;


	selected_meas = NULL;
	ViewSelectedMeas();

}
//---------------------------------------------------------------------------

void __fastcall TFMain::TrackBar_skoChange(TObject *Sender)
{
	if (b_system_start) {

		return;
	}

	GBISystem->sko_limit = (double)TrackBar_sko->Position;
	GBISystem->ViewSKOlimit();
}
//---------------------------------------------------------------------------


void __fastcall TFMain::TreeView_systemClick(TObject *Sender)
{


	int type = 0;
	int idx = 0;
	void* obj = NULL;

	if (GBISystem->CheckTreeItemSelected(&type, &idx, &obj) == 0)
	{
		if (GBISystem->IsRunning() == true)
		{
			ToolButton_StartClick(NULL);
		}

		if (type == TREE_ITEM_TYPE_SYSTEM) {

			N_system_adjust->Visible = true;
			N_add_place->Visible = true;

		}

		if (type == TREE_ITEM_TYPE_PORT) {

			N_port_adjust->Visible = true;
		}

		if (type == TREE_ITEM_TYPE_SENSOR) {

			N_sensor_adjust->Visible = true;
		}

		if (type == TREE_ITEM_TYPE_PLACE) {

			N_place_adjust->Visible = true;
			N_place_delete->Visible = true;
			N_drill_add->Visible = true;
			current_place = (TPlace*)obj;
			current_place_idx = idx;
		}

		if (type == TREE_ITEM_TYPE_DRILL) {

			N_drill_adjust->Visible = true;
			N_drill_delete->Visible = true;
			N_meas_add->Visible = true;
			current_drill = (TDrill*)obj;
			current_drill_idx = idx;

			if (selected_drill) {

				selected_drill->Unselect();
			}

			selected_drill = (TDrill*)obj;
			selected_drill_idx = idx;
			selected_drill->Select();

			GBISystem->last_place_idx = selected_drill->pnum;
			GBISystem->last_drill_idx = selected_drill->num;

			ViewSelectedDrill();

		}

		if (type == TREE_ITEM_TYPE_MEAS) {

			N_meas_delete->Visible = true;

			current_meas = (TMeas*)obj;
			current_meas_idx = idx;

			if (selected_meas!=NULL) {

				selected_meas->Unselect();
			}

			selected_meas = (TMeas*)obj;
			selected_meas_idx = idx;
			selected_meas->Select();
			selected_meas->table = this->StringGrid_meas;
			selected_meas->LoadData();

			GBISystem->last_place_idx = selected_meas->pnum;
			GBISystem->last_drill_idx = selected_meas->dnum;

			ViewSelectedMeas();
		}

	}

}
//---------------------------------------------------------------------------


#define  TABLE_COL_COUNT 13

void TFMain::ViewSelectedMeas(void)
{

	UnselectDrill();

	WideString s(L"");
	WideString ss(L"");


	if (selected_meas == NULL) {

		this->Edit_meas_time->Text = s;
		this->Edit_time_create->Text = s;
		this->Edit_meas_place->Text = s;
		this->Edit_meas_drill->Text = s;

		TStringGrid* table = 	this->StringGrid_meas;

		table->RowCount = MAX_RECORDS_MEAS+1; table->ColCount = TABLE_COL_COUNT;


		for (int i = 0; i < 10 ; i++) {

		table->ColWidths[i]= 100;


		}

		for (int i = 0; i<MAX_RECORDS_MEAS; i++) {

			table->RowHeights[i]= 20;

		}

		table->FixedCols = 1;
		table->FixedRows = 1;

		for (int col = 1; col < TABLE_COL_COUNT; col++) {

			for (int row = 1; row < MAX_RECORDS_MEAS; row++) {

				table->Cells[col][row]="";
			}
		}

	}
	else
	{

		selected_meas->DataToTable();

		s = FormatDateTime(L"dd-mm-yyyy hh:nn:ss",selected_meas->create_time);
		this->Edit_time_create->Text = s;

		if (selected_meas->finalized == true) {

			s = FormatDateTime(L"dd-mm-yyyy hh:nn:ss",selected_meas->finalize_time);
		}
		else
		{
			s.printf(L"НЕТ");
		}

		this->Edit_meas_time->Text = s;
		this->Edit_meas_place->Text = selected_meas->name_place;

		TDrill* d = GBISystem->place_list[selected_meas->pnum-1]->drill_list[selected_meas->dnum-1];

		if (d->drill_orient == DRILL_ORIENT_VERTICAL)
		{
			s.printf(L"%s [вертикальная][%.1f]",d->name.c_bstr(), d->drill_asimut);
		}
		else
		{
			s.printf(L"%s [горизонтальная]",d->name.c_bstr());
		}

		if (d->single_way) {

			s = s+L"[один проход]";
		}

		this->Edit_meas_drill->Text = s;

	}

		this->LPageControl_console->Visible = true;
		this->TabSheet_charts->TabVisible = false;
		this->TabSheet_charts_v->TabVisible = false;
		this->TabSheet_meas_table->TabVisible = true;
		this->MPageControl->ActivePage = TabSheet_meas_table;
}

void __fastcall TFMain::Button_finishClick(TObject *Sender)
{
	if (selected_meas == NULL) {

		utils_ShowMessage(L"Выберите измрение в дереве системы!");
		return;
	}

	if (selected_meas->finalized) {

		if (Application->MessageBoxW(L"Данное измерение уже завершено, время завершения будет изменено. Хотите продолжить?",L"ВНИМАНИЕ!",1) != IDOK)  {

			return;

		}

	}

	current_meas->Calculate();

	GBISystem->SysConfMgr.Backup(0,0);

	ViewSelectedMeas();
}
//---------------------------------------------------------------------------

void TFMain::SetWindowSize(int par)
{

	unsigned int chars_panel_width = this->Panel_chats->Width;
	unsigned int chars_panel_height = this->Panel_chats->Height;

	this->Panel_charty_h->Visible = false;
	this->Panel_charty_h->Height = chars_panel_height*50/1000;
	this->Panel_chartx_h->Height = chars_panel_height*950/1000;


	unsigned int chars_panel_v_width = this->Panel_charts_v->Width;

	this->Panel_chart_x_v->Width = chars_panel_v_width*330/1000;
	this->Panel_chart_y_v->Width = chars_panel_v_width*330/1000;
	this->Panel_chart_r->Width = chars_panel_v_width*330/1000;

	this->Panel_chart_x_v->Height = chars_panel_height*950/1000;
	this->Panel_chart_y_v->Height = chars_panel_height*950/1000;
	this->Panel_chart_r->Height = chars_panel_height*950/1000;
}

//---------------------------------------------------------------------------

void __fastcall TFMain::TabSheet_charts_oldShow(TObject *Sender)
{
	screen_mode =1;
	Timer_set_screen->Enabled = true;
}
//---------------------------------------------------------------------------

void TFMain::DevideScreen(int par)
{

		HDC hDCScreen = GetDC(NULL);
		unsigned int Horres = GetDeviceCaps(hDCScreen, HORZRES);
		unsigned int Vertres = GetDeviceCaps(hDCScreen, VERTRES);
		ReleaseDC(NULL, hDCScreen);

		this->Panel_tree->Width = Horres/4;
		this->Panel_meas_table->Width = Horres - this->Panel_tree->Width - 10;
		this->Panel_console_meas_control->Width = this->Panel_meas_table->Width;
		this->Panel_console_meas_control->Height = (Vertres*20)/100;

		i_table_col_width = (this->Panel_meas_table->Width/14)+3;

			if (par) {

				this->Panel_work_area->Height = Vertres*836/1000;
				this->Panel_chats->Height = Vertres*836/1000;
				this->Panel_charts_v->Height = Vertres*836/1000;
			}
			else
			{

				this->Panel_work_area->Height = Vertres*500/1000;
				this->Panel_chats->Height = Vertres*500/1000;
				this->Panel_charts_v->Height = Vertres*500/1000;

			}

			this->Panel_console_meas_control->Height = (Horres - Panel_work_area->Height - MToolBar->Height - MStatusBar->Height)*900/1000;

		SetWindowSize(par);

		TRect r;
		::GetWindowRect((HWND)this->Panel_console_meas_control->Handle,&r);

		unsigned int top,left,width,height = 0;

		width = r.Width()*250/1000;
		height = (this->Height - Panel_work_area->Height - MToolBar->Height - MStatusBar->Height)*550/1000;//r.Height()*800/1000;
		Button_record->Width = width;
		Button_record->Height = height;

		top = Button_record->Top;
		left = Button_record->Left;
		width = Button_record->Width;
		height = Button_record->Height;

		Label_X->Top = top;
		Label_X->Left = left+width+20;
		Label_X->Width = width*100/1000;
		Label_X->Height = height*250/1000;

		Edit_measX->Top = top;
		Edit_measX->Left = left+width+Label_X->Width*2;
		Edit_measX->Width = width*850/1000;
		Edit_measX->Height = Button_record->Height*200/1000;

		Label_Y->Top = Label_X->Top + Label_X->Height;
		Label_Y->Left = left+width+20;
		Label_Y->Width = width*100/1000;
		Label_Y->Height = height*400/1000;

		Edit_measY->Top = Label_Y->Top;;
		Edit_measY->Left = left+width+Label_X->Width*2;
		Edit_measY->Width = width*850/1000;
		Edit_measY->Height = Button_record->Height*200/1000;;

		top = Edit_measY->Top+Edit_measY->Height;
		left = Edit_measY->Left;
		width = Edit_measY->Width;
		height = Button_record->Height - Edit_measY->Height*2;

		GroupBox_sko->Top =  top+10;
		GroupBox_sko->Left = left;
		GroupBox_sko->Width = width;
		GroupBox_sko->Height = Button_record->Height-(GroupBox_sko->Top - Button_record->Top);

		Edit_sko->Top =  (GroupBox_sko->Height*250/1000);
		Edit_sko->Width = width*400/1000;
		Edit_sko->Height = height*250/1000;
		Edit_sko->Left = (GroupBox_sko->Width - Edit_sko->Width)/2;

		TrackBar_sko->Width = GroupBox_sko->Width*950/1000;
		TrackBar_sko->Height = GroupBox_sko->Height*200/1000;
		TrackBar_sko->Left = (GroupBox_sko->Width*25/1000);
		TrackBar_sko->Top =  (GroupBox_sko->Height*700/1000);

		Label_skomin->Left = (GroupBox_sko->Width*20/1000);
		Label_skomax->Left = GroupBox_sko->Width - (GroupBox_sko->Width*150/1000);

		Label_skomin->Top =  (GroupBox_sko->Height*500/1000);
		Label_skomax->Top =  (GroupBox_sko->Height*500/1000);

		Label_place->Left = Edit_measX->Left+Edit_measX->Width+Edit_measX->Width*50/1000;
		Edit_meas_place->Left = Label_place->Left+Edit_measX->Width*450/1000;
		Edit_meas_place->Top = Edit_measX->Top;
		Edit_meas_place->Height = Button_record->Height/6;
		Edit_meas_place->Width = (Panel_console_meas_control->Width-Edit_meas_place->Left)*950/1000;

		Label_place->Top = Edit_meas_place->Top+Edit_meas_place->Height/2-Label_place->Height/2;

		Label_drill->Left = Label_place->Left;
		Edit_meas_drill->Left = Edit_meas_place->Left;
		Edit_meas_drill->Top = Edit_meas_place->Top + Edit_meas_place->Height+Edit_meas_place->Height*200/1000;
		Edit_meas_drill->Height = Button_record->Height/6;
		Edit_meas_drill->Width = Edit_meas_place->Width;
		Label_drill->Top = Edit_meas_drill->Top+Edit_meas_drill->Height/2-Label_drill->Height/2;

		Label_tstart->Left = Label_place->Left;
		Edit_time_create->Left = Edit_meas_place->Left;
		Edit_time_create->Top = Edit_meas_drill->Top + Edit_meas_drill->Height+Edit_meas_drill->Height*200/1000;
		Edit_time_create->Height = Button_record->Height/6;
		Edit_time_create->Width = Edit_meas_place->Width;
		Label_tstart->Top = Edit_time_create->Top+Edit_time_create->Height/2-Label_tstart->Height/2;

		Label_end->Left = Label_place->Left;
		Edit_meas_time->Left = Edit_meas_place->Left;
		Edit_meas_time->Top = Edit_time_create->Top + Edit_time_create->Height+Edit_time_create->Height*200/1000;
		Edit_meas_time->Height = Button_record->Height/6;
		Edit_meas_time->Width = Edit_meas_place->Width;
		Label_end->Top = Edit_meas_time->Top+Edit_meas_time->Height/2-Label_end->Height/2;

		Button_finish->Left = Edit_meas_place->Left;
		Button_finish->Width = Edit_meas_place->Width;
		Button_finish->Height = Button_record->Height*225/1000;
		Button_finish->Top = Button_record->Top+Button_record->Height-Button_finish->Height;

}
 //---------------------------------------------------------------------------
void __fastcall TFMain::TabSheet_meas_tableShow(TObject *Sender)
{
	screen_mode =0;
	Timer_set_screen->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFMain::Timer_set_screenTimer(TObject *Sender)
{
	Timer_set_screen->Enabled = false;
	DevideScreen(screen_mode);
	//SetWindowSize(screen_mode);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::Panel_chats_oldResize(TObject *Sender)
{
	SetWindowSize(screen_mode);
}
//---------------------------------------------------------------------------
void TFMain::ViewSelectedDrill(void)
{
   UnselectMeas();

   this->TabSheet_meas_table->TabVisible = false;
   this->LPageControl_console->Visible = false;

	Tc_X_h->drill = selected_drill;
	Tc_Y_h->drill = selected_drill;

	if (selected_drill->drill_orient == DRILL_ORIENT_HORIZONT) {

		this->TabSheet_charts->TabVisible = true;
		this->TabSheet_charts_v->TabVisible = false;
		this->MPageControl->ActivePage = TabSheet_charts;


		Tc_X_h->drill = selected_drill;
		Tc_Y_h->drill = selected_drill;

		Tc_X_h->Redraw();
		Tc_Y_h->Redraw();

	}

	if (selected_drill->drill_orient == DRILL_ORIENT_VERTICAL) {

		this->TabSheet_charts->TabVisible = false;
		this->TabSheet_charts_v->TabVisible = true;
		this->MPageControl->ActivePage = TabSheet_charts_v;


		Tc_X_v->drill = selected_drill;
		Tc_Y_v->drill = selected_drill;
		Tc_R_v->drill = selected_drill;

		Tc_X_v->Redraw();
		Tc_Y_v->Redraw();
		Tc_R_v->Redraw();

	}

	screen_mode =1;
	Timer_set_screen->Enabled = true;

}
void __fastcall TFMain::Chart_x_hDblClick(TObject *Sender)
{
		unsigned int chars_panel_height = this->Panel_chats->Height;

		 if (this->Panel_chartx_h->Height > chars_panel_height*800/1000)
		 {
				 //SetWindowSize(screen_mode);
				 this->Panel_chartx_h->Height = chars_panel_height*480/1000;
  				 this->Panel_charty_h->Height = chars_panel_height*480/1000;
		 }
		 else
		 {
				this->Panel_chartx_h->Height = chars_panel_height*900/1000;
		 }

}
//---------------------------------------------------------------------------

void __fastcall TFMain::Panel_chartx_h_oldDblClick(TObject *Sender)
{
		unsigned int chars_panel_height = this->Panel_chats->Height;

		 if (this->Panel_chartx_h->Height > chars_panel_height/3)
		 {
				 SetWindowSize(screen_mode);
		 }
		 else
		 {
				this->Panel_chartx_h->Height = (chars_panel_height*4)/5;
		 }
}
//---------------------------------------------------------------------------

void __fastcall TFMain::TabSheet_charts_vShow(TObject *Sender)
{
	screen_mode =1;
	Timer_set_screen->Enabled = true;
}
//---------------------------------------------------------------------------

void TFMain::UnselectDrill()
{
	if (selected_drill) {

		selected_drill->Unselect();
		selected_drill = NULL;
	}
}
//---------------------------------------------------------------------------
void TFMain::UnselectMeas()
{
	if (selected_meas) {

		selected_meas->Unselect();
		selected_meas = NULL;
	}
}
//---------------------------------------------------------------------------


void __fastcall TFMain::N_create_confClick(TObject *Sender)
{
	ClearTable();
	GBISystem->CreateConf(SaveDialog);
	SystemInit();
}
//---------------------------------------------------------------------------

void __fastcall TFMain::N_open_confClick(TObject *Sender)
{
	ClearTable();
	GBISystem->OpenConf(OpenDialog);
	SystemInit();
}
//---------------------------------------------------------------------------

void __fastcall TFMain::N_save_confClick(TObject *Sender)
{
	ClearTable();
	GBISystem->SaveConf(SaveDialog);
	SystemInit();
}
//---------------------------------------------------------------------------

void __fastcall TFMain::ToolButton_baseresClick(TObject *Sender)
{
	GBISystem->SysConfMgr.Backup(0, 1);
	Console(L"резервная копия базы данных сохранена",L"");
}
//---------------------------------------------------------------------------


void __fastcall TFMain::ToolButton12Click(TObject *Sender)
{
		Timer_excel_export_progress->Enabled = false;

		if (selected_drill != NULL)
		{
			this->MStatusBar->Panels->Items[2]->Text = L"Экспорт в Excel ...";
			selected_drill->Excel();
			this->MStatusBar->Panels->Items[2]->Text = "";
			Form_excel_progress->StopShow();
			return;
		}
		else
		{
			if (selected_meas != NULL)
			{
				this->MStatusBar->Panels->Items[2]->Text = "Экспорт в Excel ...";
				selected_meas->Excel(1);
				this->MStatusBar->Panels->Items[2]->Text = "";
				Form_excel_progress->StopShow();
				return;
			}
			else
			{
			utils_ShowMessage(L"Для экспорта в Excel выбрать скважину или измерение в дереве системы!");
			}
		 }
}
//---------------------------------------------------------------------------

void __fastcall TFMain::Timer_excel_export_progressTimer(TObject *Sender)
{
		  WideString s(L"");
		  s.printf(L"Экспорт в Excel ... %.1f%",excel_export_rate);
		  this->MStatusBar->Panels->Items[2]->Text = s;

}
//---------------------------------------------------------------------------

void __fastcall TFMain::Panel1DblClick(TObject *Sender)
{
    Sleep(1);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::Chart_y_hDblClick(TObject *Sender)
{
		unsigned int chars_panel_height = this->Panel_chats->Height;

		 if (this->Panel_charty_h->Height > chars_panel_height*800/1000)
		 {
				 this->Panel_chartx_h->Height = chars_panel_height*480/1000;
				 this->Panel_charty_h->Height = chars_panel_height*480/1000;
		 }
		 else
		 {
				this->Panel_chartx_h->Height = chars_panel_height*100/1000;
		 }

}
//---------------------------------------------------------------------------


#include "CalcTest.h"

void __fastcall TFMain::ToolButton_testClick(TObject *Sender)
{

return;//!!!

GBISystem->ImportFromDataFile(L"c:\\Prj\\Gorizont\\RAD Studio C++ Builder\\GBI16\\Win32\\Debug\\Data\\IZDAT.CSV");

return;

TDataFile df;

df.ParsDaTaFile(L"c:\\Prj\\Gorizont\\RAD Studio C++ Builder\\GBI16\\Win32\\Debug\\Data\\IZDAT.CSV");

return;


df.OpenFile(L"c:\\Prj\\Gorizont\\RAD Studio C++ Builder\\GBI16\\Win32\\Debug\\Data\\IZDAT.CSV");

df.CheckFile();

df.pars_en = true;

	while (df.GetStr() == 0)
	{
		while (df.GetWord() == 0);

	}

return;

this->GBISystem->place_list[0]->drill_list[0]->meas_list[0]->SaveData(0);

return;

 ConvertTextFile_UTF16LEBOM (L"c:\\Prj\\Gorizont\\RAD Studio C++ Builder\\GBI_U16\\Win32\\Debug\\utf8.txt");

return;

FILE* f = NULL;

f = fopen("utf8.txt", "rb");

char c[8448];


int idxu8 = 0;

while (1)  {

if (!fread(&c[idxu8++],1,1,f)) break;

}


fclose(f);

WideString sutf16(L"АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя");

TCHAR  tcutf16[65];

wcscpy(tcutf16, sutf16.c_bstr());

TCHAR tc[8448];

int idx = 0;

for (int i = 0; i < idxu8; i++) {

	BYTE code = 0;

	code = c[i];


	if (code>191) {

	   idx = code - 192;

	   tc[i] = (TCHAR)tcutf16[idx];
	}
	else
	{
		tc[i] = (TCHAR) c[i];
	}


     tc[i+1] = '\0';

}



	ShowMessage(tc);

return;

	TPlace* p = this->GBISystem->place_list[0];
	TDrill* d = p->drill_list[0];
	TMeas* m = d->meas_list[0];

	for (int i = 0; i < m->records_cnt; i++) {

		m->records[i].X1 = test_dataset_h_s_3 [i];
		m->records[i].depth = 0.5*(double)(i);

	}

	m->DataToTable();

return;


}

//---------------------------------------------------------------------------

void __fastcall TFMain::Chart_x_vDblClick(TObject *Sender)
{

	if (Panel_chart_x_v->Width < Panel_charts_v->Width * 500/1000) {

		Panel_chart_x_v->Width = Panel_charts_v->Width * 900/1000;
		Panel_chart_y_v->Width = Panel_charts_v->Width * 85/1000;
		Panel_chart_r->Width = Panel_charts_v->Width * 85/1000;

	}
	else
	{
		Panel_chart_x_v->Width = Panel_charts_v->Width * 330/1000;
		Panel_chart_y_v->Width = Panel_charts_v->Width * 330/1000;
		Panel_chart_r->Width = Panel_charts_v->Width * 330/1000;
	}

}
//---------------------------------------------------------------------------

void __fastcall TFMain::Chart_y_vDblClick(TObject *Sender)
{
	if (Panel_chart_y_v->Width < Panel_charts_v->Width * 500/1000) {

		Panel_chart_y_v->Width = Panel_charts_v->Width * 900/1000;
		Panel_chart_x_v->Width = Panel_charts_v->Width * 85/1000;
		Panel_chart_r->Width = Panel_charts_v->Width * 85/1000;

	}
	else
	{
		Panel_chart_x_v->Width = Panel_charts_v->Width * 330/1000;
		Panel_chart_y_v->Width = Panel_charts_v->Width * 330/1000;
		Panel_chart_r->Width = Panel_charts_v->Width * 330/1000;
	}

}
//---------------------------------------------------------------------------

void __fastcall TFMain::Chart_rDblClick(TObject *Sender)
{
	if (Panel_chart_r->Width < Panel_charts_v->Width * 500/1000) {

		Panel_chart_r->Width = Panel_charts_v->Width * 900/1000;
		Panel_chart_y_v->Width = Panel_charts_v->Width * 85/1000;
		Panel_chart_x_v->Width = Panel_charts_v->Width * 85/1000;

	}
	else
	{
		Panel_chart_x_v->Width = Panel_charts_v->Width * 330/1000;
		Panel_chart_y_v->Width = Panel_charts_v->Width * 330/1000;
		Panel_chart_r->Width = Panel_charts_v->Width * 330/1000;
	}

}
//---------------------------------------------------------------------------

void __fastcall TFMain::Chart_x_hMouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y)
{

			ShowChartHint_v(Chart_x_h, X, Y);

}
//---------------------------------------------------------------------------

void TFMain::ShowChartHint_v(TChart* chart, int X, int Y)
{
			 bool sel = false;

			 for (int i = 0; i < 10; i++)
			 {

				 if (chart->Series[i]->Clicked(X,Y)!=-1)
				 {

				   Application->HintHidePause=10000;
				   Screen->Cursor=crDrag;	//crCross;

					WideString s(L"");
					double x,y=0;
					double& xx=x; double& yy=y;
					chart->Series[i]->GetCursorValues(xx,yy);
					WideString st(L"");

					if (chart == Chart_x_h) {

						st.printf(L"%.1f ì",xx);
						s.printf(L"[%.1f ìì]\r\n",yy);

					}
					else
					{
						st.printf(L"%.1f ì",yy);
						s.printf(L"[%.1f ìì]\r\n",xx);
					}


					int id = 0;

					id = chart->Series[i]->GetCursorValueIndex();

                    Sleep(1);

					s+=st;
					chart->Hint=s;
					chart->ShowHint=true;
					sel = true;
					break;
				 }
			 }

			 if (sel == false) {

				chart->ShowHint=false;
				Screen->Cursor=(TCursor)0;
				Application->HintHidePause=1;
			 }

}
void __fastcall TFMain::Chart_x_vMouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y)
{
	ShowChartHint_v(Chart_x_v, X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::Chart_y_vMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y)
{
	ShowChartHint_v(Chart_y_v, X, Y);
}
//---------------------------------------------------------------------------



void __fastcall TFMain::Chart_rMouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y)
{
	ShowChartHint_v(Chart_r, X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::MManualClick(TObject *Sender)
{
	TCHAR dir[1024];
	GetCurrentDirectoryW(1024,dir);
	ShellExecuteW(NULL,NULL,L"GBI.pdf",L"",dir,0);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::ToolButton_helpClick(TObject *Sender)
{
	//MManualClick(NULL);
	MMAboutClick(NULL);
}
//---------------------------------------------------------------------------

int TFMain::ClearTable(void)
{
	for (int i = 0; i < 20; i++) {

		for (int j = 1; j < 100; j++) {

				StringGrid_meas->Cells[j][j] = "";
		}
	}

	return 0;
}

void TFMain::VirtKey_TouchIn_On(void)
{
		TCHAR dir[1024];
		GetCurrentDirectoryW(1024,dir);
		wcscat(dir, L"\\Utils\\FreeVK\\");
		ShellExecuteW(NULL,NULL,L"FreeVK.exe",L"",dir,0);
}
//---------------------------------------------------------------------------


void TFMain::VirtKey_TouchIn_Off(void)
{
	TCHAR dir[1024];
	GetCurrentDirectoryW(1024,dir);
	wcscat(dir, L"\\Utils\\Touch-It\\");
	ShellExecute(NULL,NULL,L"KillIt.exe",L"",dir,0);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::ToolButton_saveClick(TObject *Sender)
{
	GBISystem->SaveSysConf();
}
//---------------------------------------------------------------------------

void __fastcall TFMain::ToolButton_openClick(TObject *Sender)
{
	ClearTable();
	GBISystem->OpenConf(OpenDialog);
	SystemInit();
}
//---------------------------------------------------------------------------

void __fastcall TFMain::ToolButton_collapseClick(TObject *Sender)
{
    GBISystem->Collapse();
}
//---------------------------------------------------------------------------

void __fastcall TFMain::ToolButton_kbdClick(TObject *Sender)
{
      VirtKey_TouchIn_On();
}
//---------------------------------------------------------------------------

void __fastcall TFMain::TreeView_systemMouseMove(TObject *Sender, TShiftState Shift,
		  int X, int Y)
{
	TTreeNode *node = TreeView_system->GetNodeAt(X,Y);
	TMeas* meas = NULL;
	char cnode[1024];

	if (node != NULL)
	{
		meas = GBISystem->GetMeasByNode(node);
		if (meas != NULL)
		{
			Hint = meas->mark;
			Application->ShowHint = true;
		}
		else
		{
           	Application->ShowHint = false;
        }
	}
	else
	{
		Application->ShowHint = false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFMain::ToolButton2Click(TObject *Sender)
{
    LPageControl_console->ActivePage = TabSheet_console;
	GBISystem->ImportDataCSV(OpenDialog);
}
//---------------------------------------------------------------------------


void __fastcall TFMain::ToolButton_save_asClick(TObject *Sender)
{
	GBISystem->SaveConf (SaveDialog);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::ToolButton_exportClick(TObject *Sender)
{
	LPageControl_console->ActivePage = TabSheet_console;
	GBISystem->ExportConfTxt(SaveDialog);
}
//---------------------------------------------------------------------------

