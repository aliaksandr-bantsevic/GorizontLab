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

//---------------------------------------------------------------------------
class TForm_Port_adjust : public TForm
{
__published:	// IDE-managed Components
	TButton *Button_ok;
	TButton *Button_cancel;
	void __fastcall Button_okClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_Port_adjust(TComponent* Owner);
public:

	TGLPort* port;
	void start(TGLPort* pr);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Port_adjust *Form_Port_adjust;
//---------------------------------------------------------------------------
#endif
