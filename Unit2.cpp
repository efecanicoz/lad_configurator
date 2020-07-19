//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit8.h"
#include "Unit2.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TFrame2 *Frame2;
//---------------------------------------------------------------------------
__fastcall TFrame2::TFrame2(TComponent* Owner, TFrame2 * Previous, TWinControl * Parrent_Control)
	: TFrame(Owner)
{
	if(Previous!=NULL)
	{
		this->Top = Previous->Top+Previous->Height;
		this->Left = Previous->Left;
		this->Parent = Previous->Parent;
		this->Prev = Previous;
		this->VarIndex = Previous->VarIndex+1;
		Previous->Next = this;
	}
	else
	{
		this->Top=0;
		this->Left=0;
		this->Parent = Parrent_Control;
		this->Prev = NULL;
		this->VarIndex = 0;
	}
	this->Next = NULL;
	this->Name = "Var"+UnicodeString(VarIndex);
	value->MinValue=0;
	value->MaxValue=100;
	value->Value=50;
	Lock=false;
	this->Align=alTop;
	BuildCondition();

}
//---------------------------------------------------------------------------
void TFrame2::SetCondition(UnicodeString &Condition_Str)
{
if(Condition_Str=="") return;
Lock=true;
UnicodeString temp;
Input->Text = Condition_Str.SubString(1,3);
Condition_Str = Condition_Str.SubString(4,Condition_Str.Length());
Condition->Text = Condition_Str.SubString(1,1);
Condition_Str = Condition_Str.SubString(2,Condition_Str.Length());
int pos = Condition_Str.Pos("B");
temp=Condition_Str.SubString(1,pos-1);
if(temp.Pos("%"))
	{
		temp=temp.SubString(2,temp.Length());
		rg->ItemIndex=0;
	}
else  rg->ItemIndex=1;
value->Value = temp.ToInt();
Condition_Str=Condition_Str.SubString(pos+1,Condition_Str.Length());
Var = Condition_Str.SubString(1,3);
Condition_Str=Condition_Str.SubString(4,Condition_Str.Length());
Lock=false;
BuildCondition();
}
//---------------------------------------------------------------------------
void TFrame2::BuildCondition()
{
	if(Input->Text.Pos("F"))
	{
		rg->ItemIndex=0;
		rg->DragMode=dmAutomatic;
		rg->Items[0].Text="%";
	}
	else
	{
		rg->DragMode=dmManual;
		rg->Items[0].Text="% of VCC\rmilliVolts";
	}
	if(rg->ItemIndex==0)
	{
		if(value->Value>100)
			value->Value=50;
		value->MinValue=0;
		value->MaxValue=100;
	}
	else
	{
/*		if(value->Value>32000)
			value->Value=32000;
		if(value->Value<8000)
			value->Value=8000;*/
		value->MinValue=8000;
		value->MaxValue=32000;
	}
	if(Lock)
		return;
	if(VarIndex<=9)
		Var = "X0"+UnicodeString(VarIndex);
	else
		Var = "X"+UnicodeString(VarIndex);

	Variable->Caption = Var;
	Condition_String = Input->Text+Condition->Text;

	if(this->rg->ItemIndex==0)
		Condition_String=Condition_String+"%";

	Condition_String=Condition_String+UnicodeString(value->Value);
	Condition_String = Condition_String+"B"+Var;
}
//---------------------------------------------------------------------------
void __fastcall TFrame2::boxKeyPress(TObject *Sender, System::WideChar &Key)
{
Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TFrame2::rgClick(TObject *Sender)
{
	if(rg->ItemIndex==0)
	{
		if(value->Value>100) value->Value=50;
		value->MinValue=0;
		value->MaxValue=100;
	}
	else
	{
/*		if(value->Value>32000)
			value->Value=32000;
		if(value->Value<8000)
			value->Value=8000; */
		value->MinValue=8000;
		value->MaxValue=32000;
	}
	Form1->Modified=true;
	BuildCondition();
}
//---------------------------------------------------------------------------
void __fastcall TFrame2::InputSelect(TObject *Sender)
{
BuildCondition();
Form1->Modified=true;
}
//---------------------------------------------------------------------------
void __fastcall TFrame2::ConditionSelect(TObject *Sender)
{
BuildCondition();
Form1->Modified=true;
}
//---------------------------------------------------------------------------
void __fastcall TFrame2::valueChange(TObject *Sender)
{
if(value->Text!="") BuildCondition();
Form1->Modified=true;
}
//---------------------------------------------------------------------------
void TFrame2::SetLock(bool Locked)
{
if(Locked)
	{
		Input->Enabled=false;
		Condition->Enabled=false;
		value->Enabled=false;
		rg->Enabled=false;
	}
else
	{
		Input->Enabled=true;
		Condition->Enabled=true;
		value->Enabled=true;
		rg->Enabled=true;
    }
}



void __fastcall TFrame2::Button1Click(TObject *Sender)
{
	;
}
//---------------------------------------------------------------------------

