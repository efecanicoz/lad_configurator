//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
Password="";
Result=false;
pass1->Text="";
pass2->Text="";
}
//---------------------------------------------------------------------------
void __fastcall TForm5::CancClick(TObject *Sender)
{
Password="";
Result=false;
pass1->Text="";
pass2->Text="";
Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm5::okClick(TObject *Sender)
{
if(pass1->Text!=pass2->Text)
{
	MessageBox(this->Handle, AnsiString("Passwords in both fields must be the same.").c_str(),AnsiString("Warning").c_str(), MB_ICONWARNING);
	return;
}
if(pass1->Text.Length()<4)
{
 MessageBox(this->Handle, AnsiString("Password must contain at least 4 characters.").c_str(),AnsiString("Warning").c_str(), MB_ICONWARNING);
 return;
}
Password = pass1->Text;
Result=true;
Close();
}
//---------------------------------------------------------------------------
