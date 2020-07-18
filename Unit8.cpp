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
	selected_item = -1;
}

void __fastcall TForm8::Button1Click(TObject *Sender)
{
	UnicodeString str;
	if(ComboBox1->Text.SubString(0,1) != "X")
	{
        return;
    }

	str = ComboBox1->Text.SubString(2, ComboBox1->Text.Length());

	this->selected_item = str.ToInt();
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm8::Button2Click(TObject *Sender)
{
	this->selected_item = -1;
    Close();
}
//---------------------------------------------------------------------------
