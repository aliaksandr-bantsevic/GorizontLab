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
	TGroupBox *GroupBox1;
	TComboBox *ComboBox_tout_rd;
	TComboBox *ComboBox_tout_wr;
	TLabel *Label2;
	TLabel *Label3;
	TGroupBox *GroupBox2;
	TComboBox *ComboBox_dl_df;
	TComboBox *ComboBox_dl_ex;
	TLabel *Label4;
	TLabel *Label5;
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
