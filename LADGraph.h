//---------------------------------------------------------------------------

#ifndef LADGraphH
#define LADGraphH

#include <vcl.h>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
//#define debug

#define AND				"A"
#define OR				"O"
#define NOT				"N"
#define OT				"V"
#define SET				"S"
#define RES				"R"
#define MOV				"B"
#define TF				"C"
#define TN				"T"
//---------------------------------------------------------------------------
#define COIL			"0"
#define LINE			"1"
#define LINE_DF			"2"
#define LINE_U			"3"
#define LINE_UF			"4"
#define LINE_UD			"5"
#define LINE_X			"6"
#define NC				"7"
#define NO				"8"
#define RE				"9"
#define SE				"10"
#define TOF				"11"
#define TON			  	"12"
#define LINE_FU		  	"13"
#define LINE_UDF	  	"14"
#define LINE_DOW	  	"15"

#define DELIM			"|"
#define ACT_DELIM		"\r"
//---------------------------------------------------------------------------
#define MINUSX	0

typedef struct Nets
{
TStringGrid *Grid;
int idx;
Nets *PrevGrid;
Nets *NextGrid;
}Nets;

typedef struct CellParam
{
	UnicodeString Param;
	UnicodeString Value;
	UnicodeString Value2;
}CellParam;


class TMyGrid: public  TStringGrid
{
    public:
		using TStringGrid::DeleteRow;
		using TStringGrid::MoveRow;
		using TStringGrid::DeleteColumn;
		using TStringGrid::MoveColumn;
};


class TLadGraph
{
private:
	TImageList *Data;
	int GetCenterV(TRect Rect);
	int GetCenterH(TRect Rect);
	void DrawCelll(TStringGrid *Grid ,CellParam * param, TRect Rect, int ACol, int ARow);
	void SetGridParams(Nets * s);
	UnicodeString SetCellParam(CellParam * Param);
	void CanGridsResize(TStringGrid *Grid);
	bool AddContactOrT(TStringGrid *Grid, CellParam *param);
	bool AddRelays(TStringGrid *Grid, CellParam *param);
	bool AddDownContactOrT(TStringGrid *Grid, CellParam *param);
	UnicodeString CompileGraph(TStringGrid *Grid);
	CellParam GetCellParam(UnicodeString CellText);
	bool isFBD(CellParam param);
	bool isCoil(CellParam param);
	bool isAnd(CellParam param);
	bool isOr(CellParam param);
	UnicodeString ReplaceData(UnicodeString Data, UnicodeString From, UnicodeString To);
	UnicodeString CompileCell(CellParam param);
	void DecompileBuffer(TStringGrid *Grid, UnicodeString Buffer);
	void DecompileBuffer2(TStringGrid *Grid, UnicodeString Buffer);
	CellParam DeCompileCell(UnicodeString &s);
	UnicodeString GetCurrentCell(TStringGrid *Grid);
	void AddUp(TStringGrid *Grid, CellParam param);
	void AddDown(TStringGrid *Grid, CellParam param);
	UnicodeString CompileRowString(TStringGrid *Grid, int Arow, int StartCol, int & EndCol);
	void InsertCol(TStringGrid *Grid, int CurCol);
	void InsertRow(TStringGrid *Grid, int CurRow);
	void DeleteRow(TStringGrid *Grid, int Row);
	void DeleteCol(TStringGrid *Grid, int Col);
	void MyTextOut(TStringGrid *Grid, int x, int y, UnicodeString Text);
	TStringGrid * CurrentGrid;
	void CompressThisGrid(TStringGrid *Grid);
	void CompressThisRows(TStringGrid *Grid);
	void CompressThisCols(TStringGrid *Grid);
	void AlignCols();
	void AlignCols_Minus();
	void InvalidateGrids();
	void ClearAllActions();
public:
	Nets *nets;
	int Networks;
	TLadGraph(TImageList *list);
	void DrawCell(TObject *Sender, int ACol, int ARow, TRect Rect);
	TStringGrid * CreateNetwork(TWinControl* AOwner, TWinControl * Parrent);
	void OnGridsResize();
	void SetCurrentGrid(TStringGrid *Grid);
	TStringGrid * GetCurrentGrid();
	bool Add_NC(TStringGrid *Grid, UnicodeString Parameter);
	bool Add_NO(TStringGrid *Grid, UnicodeString Parameter);
	bool Add_TON(TStringGrid *Grid, UnicodeString TimerName, int Sec);
	bool Add_TOF(TStringGrid *Grid, UnicodeString TimerName, int Sec);
	bool Add_Coil(TStringGrid *Grid, UnicodeString Parameter);
	bool Add_SET_Coil(TStringGrid *Grid, UnicodeString Parameter);
	bool Add_RESET_Coil(TStringGrid *Grid, UnicodeString Parameter);
	bool Add_DOWN_NC(TStringGrid *Grid, UnicodeString Parameter);
	bool Add_DOWN_NO(TStringGrid *Grid, UnicodeString Parameter);
	bool Add_DOWN_TON(TStringGrid *Grid, UnicodeString Parameter, int Sec);
	bool Add_DOWN_TOF(TStringGrid *Grid, UnicodeString Parameter, int Sec);
	bool DeleteFBD(TStringGrid *Grid);
	bool DeleteCurrentAction(bool Silent);
	UnicodeString CompileLADProgramm();
	void BuildLADProgramm(UnicodeString Prog, TWinControl* AOwner, TWinControl * Parrent, TDrawCellEvent DrawEv, TNotifyEvent OnClick);
	bool CurrentCellIsFBD();
	bool CoilTypeIsDirect(Nets *net, UnicodeString &CoilName);
	bool CellIsTimer(TStringGrid *Grid, int Arow, int Acol, int &TimerIndex);
};





#endif
