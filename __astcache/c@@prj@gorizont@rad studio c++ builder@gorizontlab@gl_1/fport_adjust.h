//---------------------------------------------------------------------------

#ifndef FPort_adjustH
#define FPort_adjustH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
#include "GL_Port.h"
#include <Vcl.ExtCtrls.hpp>
#include "GL_System.h"
//---------------------------------------------------------------------------
class TForm_Port_adjust : public TForm
{
__published:	// IDE-managed Components
	TButton *Button_ok;
	TButton *Button_cancel;
	TComboBox *ComboBox_COM;
	TTimer *Timer_start;
	TComboBox *ComboBox_type;
	TMemo *Memo_mark;
	TComboBox *ComboBox_baud;
	TLabel *Label1;
	TCheckBox *CheckBox_on;
	void __fastcall Button_okClick(TObject *Sender);
	void __fastcall Timer_startTimer(TObject *Sender);
	void __fastcall Button_cancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_Port_adjust(TComponent* Owner);
public:

	TGLPort* port;
	void start(TGLPort* pr);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Port_adjust *Form_Port_adjust;
extern TGLSystem* GLSystem;
//---------------------------------------------------------------------------
#endif
