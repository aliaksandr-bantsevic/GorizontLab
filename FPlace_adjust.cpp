//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FPlace_adjust.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_Place_adjust *Form_Place_adjust;
//---------------------------------------------------------------------------
__fastcall TForm_Place_adjust::TForm_Place_adjust(TComponent* Owner)
	: TForm(Owner)
{
        MACRO_FORM_MOVE_TO_THE_CENTER
}
//---------------------------------------------------------------------------

void TForm_Place_adjust::start(TGLPlace* pl)
{
	place = pl;
	this->Memo_mark->Text = L"����� ��������� ������ ������/��������";
	this->Edit_name->Text = L"������ �����������";

	Timer_start->Enabled = true;

	ShowModal();
}
void __fastcall TForm_Place_adjust::Button_okClick(TObject *Sender)
{
	set_place(0);

	if (mode == 0)
	{
		if (GLSystem->add_place(place->name))
		{

		}
		else
		{
			ShowMessage(L"�� ������� �������� �����!");
			delete place;
		}
	}
	else
	{

    }

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_Place_adjust::Timer_startTimer(TObject *Sender)
{
	Timer_start->Enabled = false;

	if (place == NULL)
	{
		mode = 0;
		place = new TGLPlace(this->Edit_name->Text);
		set_place(1);
	}
	else
	{
		mode = 1;
		set_place(1);
	}

}
//---------------------------------------------------------------------------
void TForm_Place_adjust::set_place(int par)
{
	if (par == 0)
	{
	   Update();
	   place->name = this->Edit_name->Text;
	   place->mark = this->Memo_mark->Text;
	}
	else
	{
	   this->Edit_name->Text = place->name;
	   Update();
    }

}