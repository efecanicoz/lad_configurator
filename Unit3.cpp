//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button1Click(TObject *Sender)
{
Result=true;
Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button2Click(TObject *Sender)
{
Result=false;
Close();
}
//---------------------------------------------------------------------------
void TForm3::PrepareToContact()
{
  Width = 330;
  Height = 89;
  Button1->Left = 159;
  Button1->Top = 22;
  Button2->Top=Button1->Top;
  Button2->Left = 240;
  Label2->Visible=false;
  time->Visible=false;
  Label3->Visible=false;
  typegroup->Visible=false;
}
//----------------------------------------------------------------------------
void TForm3::PrepareToTimer()
{
  Width = 275;
  Height = 170;
  Button1->Left = 182;
  Button1->Top = 22;
  Button2->Left=Button1->Left;
  Button2->Top = 53;
  Label2->Visible=true;
  time->Visible=true;
  Label3->Visible=true;
  typegroup->Visible=true;
}
