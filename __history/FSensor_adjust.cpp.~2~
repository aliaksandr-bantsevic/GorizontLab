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
    ShowModal();
}
void __fastcall TForm_Sensor_adjust::Button_OKClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
