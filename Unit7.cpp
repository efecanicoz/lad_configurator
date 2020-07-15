//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit7.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TForm7 *Form7;
//---------------------------------------------------------------------------
__fastcall TForm7::TForm7(TComponent* Owner)
	: TForm(Owner)
{
Result=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm7::revKeyPress(TObject *Sender, System::WideChar &Key)
{
if(Key==13) this->Button1->Click();
}
//---------------------------------------------------------------------------
void __fastcall TForm7::Button1Click(TObject *Sender)
{
Result=true;
Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm7::Button2Click(TObject *Sender)
{
Result=false;
Close();
}
//---------------------------------------------------------------------------
