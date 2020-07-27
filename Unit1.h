//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include "LADGraph.h"
#include <System.Actions.hpp>
#include <Vcl.ActnCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.PlatformDefaultStyleActnCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.StdStyleActnCtrls.hpp>
#include <Vcl.ActnMenus.hpp>
#include <Vcl.StdActns.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.CategoryButtons.hpp>
#include <Vcl.ValEdit.hpp>
#include <Data.DB.hpp>
#include <Vcl.DBGrids.hpp>
#include "LAD_ConfiguratorPCH1.h"
#include "Unit2.h"
#include "Unit3.h"
#include <Vcl.Dialogs.hpp>
#include "Unit4.h"
#include "Unit5.h"
#include "RS232implements.h"
#include "Unit6.h"
#include "Unit7.h"
//---------------------------------------------------------------------------
#define MAXIVAR		5
#define MAXIOVAR	15
#define MAXMVAR		15
#define MAXTVAR		10
#define MAXCVAR		15
#define MAXCOIL		2
#define MAXACTIONS	20
//---------------------------------------------------------------------------
#define ENTER_PROGRAMMING_MODE	"@P"
#define CLOSE_PROGRAMMING_MODE	"@E"
#define OK_ANSWER				"@O"
#define ERR_ANSWER				"@R"
#define BUSY_ANSWER				"@B"
#define REPEAT_ANSWER		   	"@T"
#define MAX_PROG_BUF			 10
#define MAXSHOTS				 50
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TImageList *ImageList1;
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TImageList *EnableActList;
	TPageControl *Tabs;
	TTreeView *tw;
	TImageList *DisableACTList;
	TTabSheet *IO;
	TTabSheet *Variables;
	TTabSheet *LAD;
	TStatusBar *StatusBar1;
	TMenuItem *New1;
	TActionToolBar *ActionToolBar1;
	TActionToolBar *ActionToolBar2;
	TMenuItem *OpenProject1;
	TMenuItem *SaveProject1;
	TMenuItem *SaveProjectAs1;
	TMenuItem *Exit1;
	TScrollBox *ScrollBox1;
	TActionManager *Actions;
	TAction *Open;
	TAction *New;
	TAction *Save;
	TAction *Save_as;
	TAction *Exit;
	TAction *NewAct;
	TAction *DeleteAct;
	TAction *AddNO;
	TAction *AddNC;
	TAction *AddT;
	TAction *AddDownNO;
	TAction *AddNCDown;
	TAction *AddTDown;
	TAction *AddSET;
	TAction *AddCoil;
	TAction *AddResetCoil;
	TValueListEditor *inputs;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox3;
	TAction *AddVariable;
	TActionToolBar *ActionToolBar3;
	TScrollBox *ScrollBox2;
	TGroupBox *GroupBox2;
	TValueListEditor *MVars;
	TGroupBox *GroupBox4;
	TValueListEditor *Timers;
	TGroupBox *GroupBox5;
	TValueListEditor *outputs;
	TAction *Delete;
	TAction *Comp;
	TSaveDialog *SaveDialog1;
	TOpenDialog *OpenDialog1;
	TAction *programm;
	TMenuItem *PLC1;
	TMenuItem *ProgrammtoPLC1;
	TMenuItem *Privilege1;
	TAction *Unlock;
	TMenuItem *Getprivileges1;
	TAction *changepass;
	TMenuItem *ChangePassword1;
	TTimer *progtimeout;
	TAction *Revision;
	TMenuItem *LAD1;
	TMenuItem *ChangeLADrevision1;
	TAction *Action1;
	TGroupBox *GroupBox6;
	TValueListEditor *counters;
	void __fastcall twClick(TObject *Sender);
	void __fastcall StringGrid1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall StringGrid1Click(TObject *Sender);
	void __fastcall ScrollBox1Resize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ExitExecute(TObject *Sender);
	void __fastcall NewActExecute(TObject *Sender);
	void __fastcall DeleteActExecute(TObject *Sender);
	void __fastcall inputsDblClick(TObject *Sender);
	void __fastcall ComboBox1Select(TObject *Sender);
	void __fastcall AddVariableExecute(TObject *Sender);
	void __fastcall NewExecute(TObject *Sender);
	void __fastcall AddNOExecute(TObject *Sender);
	void __fastcall AddNCExecute(TObject *Sender);
	void __fastcall AddTExecute(TObject *Sender);
	void __fastcall AddDownNOExecute(TObject *Sender);
	void __fastcall AddNCDownExecute(TObject *Sender);
	void __fastcall AddTDownExecute(TObject *Sender);
	void __fastcall AddCoilExecute(TObject *Sender);
	void __fastcall AddSETExecute(TObject *Sender);
	void __fastcall AddResetCoilExecute(TObject *Sender);
	void __fastcall DeleteExecute(TObject *Sender);
	void __fastcall CompExecute(TObject *Sender);
	void __fastcall Save_asExecute(TObject *Sender);
	void __fastcall SaveExecute(TObject *Sender);
	void __fastcall OpenExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall UnlockExecute(TObject *Sender);
	void __fastcall changepassExecute(TObject *Sender);
	void __fastcall MVarsKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall TimersKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall outputsKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall inputsKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall ComboBox1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall programmExecute(TObject *Sender);
	void __fastcall FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight, bool &Resize);
	void __fastcall progtimeoutTimer(TObject *Sender);
	void __fastcall RevisionExecute(TObject *Sender);
	void __fastcall DeleteVariableExecute(TObject *Sender);
	void __fastcall countersKeyPress(TObject *Sender, System::WideChar &Key);
private:	// User declarations
	TLadGraph * LadGraph;
	bool TimersUsed[MAXTVAR];
	bool CoilsUsed[MAXCOIL+MAXMVAR];
	bool Unlocked;
	UnicodeString StartWindowName;
	void BuildWindowAttributes(UnicodeString _postfix);
	void  GetFileVersion(LPCSTR filename,char * sVer, int slen);
	void SetModified(bool Mod);
	bool PosWrite;
	TFrame2 * frame2_array[MAXIOVAR];
    int frame2_count;
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	bool Modified;
	TComboBox *InputsBox;
	void PrepareContacts(TForm3 * frm);
	bool PrepareTimers(TForm3 * frm);
	bool TestDown();
	bool PrepareCoils(TForm3 * frm);
	void RefreshCiolsUsing();
	void RefreshTimersInGrid(TStringGrid * grid);
	void RefreshAllTimers();
	UnicodeString LastFilename;
	void SaveProject(UnicodeString FileName);
	unsigned long getCRC(char *pchBuf, int nBufLen);
	void OpenProject(UnicodeString FileName);
	void TestModified();
	void SetModified();
	UnicodeString GetPassHashString(UnicodeString Password);
	UnicodeString ReadHashFromReg();
	bool WritePassHashToReg(UnicodeString Hash);
	bool GetPrivileges();
	bool SendToPLC(TMemoryStream * stream);
	bool MyReadWrite(TComPort * port, AnsiString Message, AnsiString &Answer);
	AnsiString ConvertBufToHEXBuf(byte * buf, int len);

	TMemoryStream * ProgStream;
	TComPort * ComPort;
	bool StopRecv;
	void OnPortRecv(AnsiString Answer);
	int count;
	AnsiString LastMSG;
	TForm6 * progr;
	uint8_t ProgRevision;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
