//---------------------------------------------------------------------------

#ifndef FSensor_adjustH
#define FSensor_adjustH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include "GL_Sensor.h"
#include <Vcl.ExtCtrls.hpp>
#include "GL_System.h"
//---------------------------------------------------------------------------
class TForm_Sensor_adjust : public TForm
{
__published:	// IDE-managed Components
	TButton *Button_OK;
	TButton *Button_cancel;
	TTimer *Timer_start;
	TComboBox *ComboBox_addr;
	TLabel *Label1;
	TComboBox *ComboBox_type;
	TMemo *Memo_mark;
	TCheckBox *CheckBox_on;
	TComboBox *ComboBox_prt;
	TLabel *Протокол;
	void __fastcall Button_OKClick(TObject *Sender);
	void __fastcall Timer_startTimer(TObject *Sender);
	void __fastcall Button_cancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_Sensor_adjust(TComponent* Owner);

public:

	TGLSensor* sensor;

public:

    void start(TGLSensor* sn);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Sensor_adjust *Form_Sensor_adjust;
//extern PACKAGE TForm_Port_adjust *Form_Port_adjust;
extern TGLSystem* GLSystem;
//---------------------------------------------------------------------------
#endif
