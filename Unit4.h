//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Mask.hpp>
//---------------------------------------------------------------------------
class TPassInp : public TForm
{
__published:	// IDE-managed Components
	TMaskEdit *pass;
	TButton *Button1;
	TButton *Button2;
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall passKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TPassInp(TComponent* Owner);
	bool Result;
	UnicodeString Password;
};
//---------------------------------------------------------------------------
extern PACKAGE TPassInp *PassInp;
//---------------------------------------------------------------------------
#endif
