//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit9.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TForm9 *Form9;
//---------------------------------------------------------------------------
__fastcall TForm9::TForm9(TComponent* Owner)
	: TForm(Owner)
{
	result = "";
    reset_val = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm9::Button1Click(TObject *Sender)
{
	result = this->counterDDL->Text;
	reset_val = this->input_reset->Text.ToInt();
	this->Close();
}

//---------------------------------------------------------------------------
void __fastcall TForm9::Button2Click(TObject *Sender)
{
	result = "";
	reset_val = -1;
	this->Close();
}
//---------------------------------------------------------------------------
