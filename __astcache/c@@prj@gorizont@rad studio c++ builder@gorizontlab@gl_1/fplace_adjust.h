//---------------------------------------------------------------------------

#ifndef FPlace_adjustH
#define FPlace_adjustH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include "GL_Place.h"
#include <Vcl.ExtCtrls.hpp>

#include "GL_System.h"
//---------------------------------------------------------------------------
class TForm_Place_adjust : public TForm
{
__published:	// IDE-managed Components
	TButton *Button_ok;
	TButton *Button_cancel;
	TEdit *Edit_name;
	TMemo *Memo_mark;
	TLabel *Label1;
	TLabel *Label2;
	TTimer *Timer_start;
	void __fastcall Button_okClick(TObject *Sender);
	void __fastcall Timer_startTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_Place_adjust(TComponent* Owner);

public:

	TGLPlace* place;
    int mode;

public:

	void start(TGLPlace* pl);
	void set_place(int par);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Place_adjust *Form_Place_adjust;
extern TGLSystem* GLSystem;
//---------------------------------------------------------------------------
#endif
