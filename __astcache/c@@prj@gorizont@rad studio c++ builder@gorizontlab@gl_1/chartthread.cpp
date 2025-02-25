//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "ChartThread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TChartThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TChartThread::TChartThread(bool CreateSuspended, TChart* c, TDataStream* datastr)
	: TThread(CreateSuspended)
{
	t_start = 0.;
	chart = c;
	data_stream = datastr;

	TDataStream* da = data_stream;

	exit_flag = false;
	draw_flag = false;

	idx = 0;

	for (int i = 0; i < CHART_BUFFER_SIZE_DEFAULT; i++) {

		buf[i] = DATA_LOST_DBL_CONSTANT;
	}

	chart->View3D = false;

	// Заголовок
	chart->Title->Text->Clear();
	chart->Title->Text->Add(data_stream->name.c_bstr());

    // Настройка нижней оси (X-Axis)
	chart->BottomAxis->Title->Caption = "Дата-время";
	chart->BottomAxis->DateTimeFormat = "dd/mm/yyyy hh:nn:ss";
	chart->BottomAxis->SetMinMax(Now(), Now() + CHART_BUFFER_SIZE_DEFAULT / 86400.0); // CHART_BUFFER_SIZE_DEFAULT секунд = CHART_BUFFER_SIZE_DEFAULT/86400 дней
	//chart->BottomAxis->TickInner = true;
	//chart->BottomAxis->Labels->Angle = 90;
	//chart->BottomAxis->OnGetLabel = DateTimeFormat;

	chart->BottomAxis->DateTimeFormat = L"dd.mm.yyyy hh:nn:ss";	chart->BottomAxis->Automatic = false;
	chart->BottomAxis->Minimum = EncodeDate(2023, 4, 1);
	chart->BottomAxis->Maximum = EncodeDate(2023, 4, 3);
	chart->BottomAxis->Increment = DateTimeStep[dtOneHour];

	// Настройка левой оси (Y-Axis)
	chart->LeftAxis->Title->Caption = "Угловые секунды";
	chart->LeftAxis->SetMinMax(-3600, 3600);
	//chart->LeftAxis->TickInner = true;
	// Настройка правой оси (Y-Axis)
	//chart->RightAxis->Labels->Visible = true;
	//chart->RightAxis->TickInner = true;

	//---line

    // Удаление всех серий данных
	while (chart->SeriesCount() > 0)
	{
		chart->RemoveSeries(0);
	}

	TLineSeries * Ser = new TLineSeries(chart);

	Ser->Pointer->Style = psRectangle;
	Ser->Pointer->Size = 1;

	Ser->DrawStyle = dsSegments;
	Ser->Active = true;

	Ser->Pointer->Visible = false;

	Ser->Legend->Text = L" ";
	Ser->Legend->Visible = false;
	Ser->Pen->Width = 1;

	Ser->YValues->Order = loDescending;
	Ser->XValues->Order = loNone;

	Ser->Color = clBlue;

	chart->AddSeries(Ser);

	ser = Ser;


	chart->BottomAxis->SetMinMax(Now(), Now() + CHART_BUFFER_SIZE_DEFAULT * T_ONE_SEC); // CHART_BUFFER_SIZE_DEFAULT секунд = CHART_BUFFER_SIZE_DEFAULT/86400 дней
	chart->BottomAxis->Increment = T_ONE_SEC;

	 //chart->BottomAxis->LabelsStyle = talText;
	 //cart->BottomAxis->LabelStep = 100;


		ser->Pointer->Style = psRectangle;
		ser->Pointer->Size = 2;

		ser->DrawStyle = dsSegments;
		ser->Active = true;

		ser->Pointer->Visible = false;

		ser->Legend->Text = L" ";
		ser->Legend->Visible = false;
		ser->Pen->Width = 1;

		ser->YValues->Order = loDescending;
		ser->XValues->Order = loNone;

		ser->Color = clBlue;

		//ser->Smoothed = true;
		ser->Stairs = false;
		//ser->LinePen->JoinStyle. = psSolid;//pjsBevel;


		TDateTime t = Now() - T_ONE_SEC * CHART_BUFFER_SIZE_DEFAULT; int itc = 0;

		chart->BottomAxis->Minimum = t;
		chart->BottomAxis->Maximum = Now();
		double v = 0.;

		for (int i = 0; i < CHART_BUFFER_SIZE_DEFAULT; i++)
		{

			String s(" ");

			//if (itc == 10)
			{
				s = FormatDateTime("hh:mm:ss", t);
				itc = 0;
			}

			ser->AddXY(t, 1, s,clBlue);

			t += T_ONE_SEC*1; itc++; v+=100.;

		}

		//for (int i = 0; i < CHART_BUFFER_SIZE_DEFAULT; i ++)
		//{
		//	buf[i] = DATA_LOST_DBL_CONSTANT;
		//}

		//idx = CHART_BUFFER_SIZE_DEFAULT;
		//t_start = 0.;
}
//---------------------------------------------------------------------------
void __fastcall TChartThread::Execute()
{
	NameThreadForDebugging(System::String(L"ChartThread"));
	//---- Place thread code here ----

	while (exit_flag == false)
	{
		Sleep(10);

		if (draw_flag) {

			Synchronize(draw_chart);
		}

		Sleep(10);
    }

}
//---------------------------------------------------------------------------

bool over = false;

void  __fastcall TChartThread::draw_chart(void)
{
	draw_flag = false;

	calc_limits();

	String s("");
	TDateTime t = t_start;
	ser->Clear();

	if (over) chart->BottomAxis->Maximum = data_stream->last_update_timestamp - T_ONE_SEC*10;

	//chart->BottomAxis->Minimum = 0;
	//chart->BottomAxis->Maximum = t + T_ONE_SEC*CHART_BUFFER_SIZE_DEFAULT;
	//chart->BottomAxis->Minimum = t;

	//for (int i = 0; i < CHART_BUFFER_SIZE_DEFAULT; i ++)
	for (int i = 0; i < idx; i ++)
	{
		if (buf[i] != DATA_LOST_DBL_CONSTANT)
		{
			s = FormatDateTime("hh:mm:ss", t);
			ser->AddXY(t, buf[i], s, clBlue);
			//ser->AddXY(t, 0, s, clBlue);
			t += T_ONE_SEC;
		}
		else
		{
			ser->AddNullXY(t,buf[i],"");
		}
    }

}

void TChartThread::accept_data(double* d, int len)
{

	if ((double)t_start == 0.) t_start = data_stream->last_update_timestamp;
	//if (over) chart->BottomAxis->Maximum = data_stream->last_update_timestamp + T_ONE_SEC*10;

	for (int i = 0; i < len; i++)
	{
		buf[idx++] = d [i];

		if (idx == CHART_BUFFER_SIZE_DEFAULT) {

			idx = CHART_BUFFER_SIZE_DEFAULT - 1;

			for (int j = 1; j < CHART_BUFFER_SIZE_DEFAULT; j++) {

				buf [j-1] = buf [j];

			}

			t_start += T_ONE_SEC;
			over = true;

		}
	}

	draw();
}

void TChartThread::exit(void)
{
	exit_flag = true;
}

void TChartThread::draw(void)
{
	draw_flag = true;
}


void __fastcall TChartThread::DateTimeFormat(TChartAxis *Sender, TChartSeries *Series, int ValueIndex, AnsiString &LabelText) {
	TDateTime Date = Series->XValue[ValueIndex];
    LabelText = Date.FormatString("dd.mm.yyyy hh:nn:ss");
}

void TChartThread::calc_limits()
{
	 chart->BottomAxis->Minimum = 0;
	 chart->BottomAxis->Maximum = t_start + T_ONE_SEC * CHART_BUFFER_SIZE_DEFAULT;
	 chart->BottomAxis->Minimum = t_start;

	  double dmin = 9999999999.;
	  double dmax = -9999999999.;

	  for (int i = 0; i < CHART_BUFFER_SIZE_DEFAULT; i++)
	  {
		   if (buf[i] != DATA_LOST_DBL_CONSTANT )
		   {
				if (buf[i] < dmin) dmin = buf[i];
				if (buf[i] > dmax) dmax = buf[i];
		   }
	  }

	  double dd = abs(dmax - dmin);

	  dmax += dd*0.1;
	  dmin -= dd*0.1;

	 chart->LeftAxis->Minimum = -999999999;
	 chart->LeftAxis->Maximum = dmax;
	 chart->LeftAxis->Minimum = dmin;

}

void TChartThread::update_data()
{
	accept_data(&data_stream->val, 1);

}