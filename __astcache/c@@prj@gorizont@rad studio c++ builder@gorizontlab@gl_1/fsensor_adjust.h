//---------------------------------------------------------------------------

#ifndef FSensor_adjustH
#define FSensor_adjustH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include "GL_Sensor.h"
//---------------------------------------------------------------------------
class TForm_Sensor_adjust : public TForm
{
__published:	// IDE-managed Components
	TButton *Button_OK;
	TButton *Button_cancel;
	void __fastcall Button_OKClick(TObject *Sender);
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
//---------------------------------------------------------------------------
#endif
