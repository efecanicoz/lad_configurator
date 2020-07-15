//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "cspin.h"
#include <Vcl.ExtCtrls.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TFrame2 : public TFrame
{
__published:	// IDE-managed Components
	TLabel *Variable;
	TLabel *Label2;
	TComboBox *Input;
	TLabel *Inp;
	TLabel *Label3;
	TComboBox *Condition;
	TLabel *Label4;
	TRadioGroup *rg;
	TCSpinEdit *value;
	TButton *Button1;
	void __fastcall boxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall rgClick(TObject *Sender);
	void __fastcall InputSelect(TObject *Sender);
	void __fastcall ConditionSelect(TObject *Sender);
	void __fastcall valueChange(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
	void BuildCondition();
	bool Lock;
public:		// User declarations
	__fastcall TFrame2(TComponent* Owner, TFrame2 * Previous, TWinControl * Parrent_Control);
	TFrame2 * Prev;
	TFrame2 * Next;
	UnicodeString Condition_String;
	UnicodeString Var;
	int VarIndex;
	void SetCondition(UnicodeString &Condition_Str);
	void SetLock(bool Locked);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrame2 *Frame2;
//---------------------------------------------------------------------------
#endif
