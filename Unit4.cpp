//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPassInp *PassInp;
//---------------------------------------------------------------------------
__fastcall TPassInp::TPassInp(TComponent* Owner)
	: TForm(Owner)
{
Result=false;
Password="";
pass->Text="";
}
//---------------------------------------------------------------------------
void __fastcall TPassInp::Button2Click(TObject *Sender)
{
Password="";
pass->Text="";
Close();
}
//---------------------------------------------------------------------------
void __fastcall TPassInp::Button1Click(TObject *Sender)
{
Password=pass->Text;
pass->Text="";
Result=true;
Close();
}
//---------------------------------------------------------------------------
void __fastcall TPassInp::passKeyPress(TObject *Sender, System::WideChar &Key)
{
if(Key==13) Button1->Click();
}
//---------------------------------------------------------------------------
void __fastcall TPassInp::FormShow(TObject *Sender)
{
pass->SetFocus();
}
//---------------------------------------------------------------------------
