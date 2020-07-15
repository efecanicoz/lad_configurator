//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit8.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm8 *Form8;
//---------------------------------------------------------------------------
__fastcall TForm8::TForm8(TComponent* Owner) : TForm(Owner)
{
    varName = "";
    result = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm8::BeforeActivate(TObject *Sender)
{
    this->Label1->Caption = this->Label1->Caption + this->varName;
}
//---------------------------------------------------------------------------
void __fastcall TForm8::Button1Click(TObject *Sender)
{
	this->result = true;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm8::Button2Click(TObject *Sender)
{
	this->result = false;
    Close();
}
//---------------------------------------------------------------------------
