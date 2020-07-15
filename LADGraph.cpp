//---------------------------------------------------------------------------

#pragma hdrstop

#include "LADGraph.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
void TLadGraph::InvalidateGrids()
{
Nets * n = nets;
if(n==NULL) return;
while(1)
{
	 if(n->Grid!=NULL)
	 {
		n->Grid->Invalidate();
	 }
	 n=n->NextGrid;
	 if(n==NULL) break;
}
}
//---------------------------------------------------------------------------
void TLadGraph::ClearAllActions()
{
Nets * n = nets;
if(n==NULL) return;
while(1)
{
	 if(n==NULL) break;
	 if(n->Grid!=NULL)
	 {
			  CurrentGrid=n->Grid;
			  DeleteCurrentAction(true);
			  n = nets;
			  continue;
	 }
	 if(n==NULL) break;
	 else n=n->NextGrid;
}
CurrentGrid=NULL;
}
//---------------------------------------------------------------------------
void TLadGraph::BuildLADProgramm(UnicodeString Prog, TWinControl* AOwner, TWinControl * Parrent, TDrawCellEvent DrawEv, TNotifyEvent OnClick)
{
if(Prog=="") return;
ClearAllActions();
UnicodeString prg = Prog;
UnicodeString s;
TStringGrid *ss;
int pos;
while(1)
{
	pos = prg.Pos(ACT_DELIM);
	if(pos!=0)
	{
		s=prg.SubString(1,pos-1);
		prg=prg.SubString(pos+1,prg.Length());
	}
	else
	{
		s=prg;
		prg="";
	}
	if(s=="") break;
	ss=this->CreateNetwork(AOwner,Parrent);
	ss->OnDrawCell = DrawEv;
	ss->OnClick = OnClick;
	this->DecompileBuffer(ss,s);
	s="";
	if(prg=="") break;
}
this->AlignCols();
}
//---------------------------------------------------------------------------
UnicodeString TLadGraph::CompileLADProgramm()
{
UnicodeString prog;
UnicodeString s;
Nets * n = nets;
if(n==NULL) return "";
while(1)
{
	 if(n->Grid!=NULL)
	 {
		s = CompileGraph(n->Grid);
		if(s==NULL)
			{
			  CurrentGrid=n->Grid;
			  DeleteCurrentAction(true);
			  n = nets;
			  continue;
			}
		else if(s.Pos("?"))
			{
             CurrentGrid = n->Grid;
				if(n->Grid!=NULL)
				{
				n->Grid->Row=0;
				n->Grid->Col=1;
				n->Grid->Invalidate();
				}
			 MessageBox(n->Grid->Handle,
						AnsiString("LAD diagram should not contain undefined variables, such as '?..'").c_str(),
						AnsiString("Compilation error").c_str(),
						MB_ICONSTOP);
			 return "";
			}
		else if(UnicodeString(s.operator [](1))==UnicodeString(SET) ||
				UnicodeString(s.operator [](1))==UnicodeString(RES) ||
				UnicodeString(s.operator [](1))==UnicodeString(MOV))
		{
			CurrentGrid=n->Grid;
			DeleteCurrentAction(true);
			n = nets;
			continue;
		}
		else
		{
		  prog=prog+UnicodeString(ACT_DELIM)+s;
        }
	 }
	 n=n->NextGrid;
	 if(n==NULL) break;
}
if(prog.Length()>1)
{
	if(UnicodeString(prog.operator [](1))== UnicodeString(ACT_DELIM))
	{
		prog=prog.SubString(2,prog.Length());
	}
}
return prog;
}
//---------------------------------------------------------------------------
bool TLadGraph::DeleteCurrentAction(bool Silent)
{
if(CurrentGrid==NULL) return false;
if(nets==NULL) return false;
Nets * n = nets;
Nets * del=NULL;
Nets * prev;
Nets * next;
bool found;
while(1)
{
	 if(n->Grid!=NULL)
	 {
	   if(n->Grid==CurrentGrid)
		{
			found=true;
			break;
		}
	 }
	 n=n->NextGrid;
	 if(n==NULL) break;
}
if(!found) return false;
del=n;
if(!Silent)
{
	int res = MessageBox(del->Grid->Handle, AnsiString(AnsiString("Are you sure you want to delete Action ")+AnsiString(del->idx)).c_str(),AnsiString("Are you sure?").c_str(), MB_YESNO|MB_ICONQUESTION);
	if(res!= IDYES) return false;
}
if(del==nets) nets=del->NextGrid;
prev=n->PrevGrid;
next = n->NextGrid;
if(prev!=NULL) prev->NextGrid = next;
if(next!=NULL) next->PrevGrid = prev;
delete del->Grid;
delete del;
Networks--;
n = nets;
int idx=1;
if(n==NULL)
{
	nets = new Nets;
	nets->PrevGrid = NULL;
	nets->NextGrid = NULL;
	nets->Grid = NULL;
	nets->idx = 0;
	CurrentGrid=NULL;
	Networks=0;
return true;
}
if(nets->Grid!=NULL)
{
nets->Grid->Row=0;
nets->Grid->Col=1;
CurrentGrid = nets->Grid;
nets->Grid->Invalidate();
}
else CurrentGrid=NULL;
while(1)
{
	 if(n->Grid!=NULL)
	 {
		n->idx=idx;
		idx++;
		n->Grid->Cells[0][0]= "Action "+UnicodeString(n->idx);
		n->Grid->Invalidate();
	 }
	 n=n->NextGrid;
	 if(n==NULL) break;
}
return true;
}
//---------------------------------------------------------------------------
void TLadGraph::AlignCols_Minus()
{
 int maxcol = 0;
if(nets==NULL) return;
Nets * n = nets;
while(1)
{
	 if(n->Grid!=NULL)
	 {
		 CompressThisRows(n->Grid);
		 CompressThisCols(n->Grid);
		 if(n->Grid->ColCount>maxcol) maxcol = n->Grid->ColCount;
	 }
	 n=n->NextGrid;
	 if(n==NULL) break;
}
if(maxcol==0) return;
n = nets;
while(1)
{
	 if(n->Grid!=NULL)
	 {
		 if(n->Grid->ColCount<maxcol)
		 {
			 for(int i=n->Grid->ColCount; i<maxcol; i++)
			 {
			   InsertCol(n->Grid, n->Grid->ColCount-1);
			 }
		 }
	 }
	 n=n->NextGrid;
	 if(n==NULL) break;
}
}
//---------------------------------------------------------------------------
void TLadGraph::CompressThisCols(TStringGrid *Grid)
{
if(Grid==NULL) return;
int Col = 1;
int Row = 0;
bool flg=false;
CellParam param;
for(Col; Col<Grid->ColCount-1; Col++)
{
	for(Row; Row<Grid->RowCount; Row++)
	{
	param = GetCellParam(Grid->Cells[Col][Row]);
	if(param.Param!=LINE && param.Param!="")
		{
			flg=true;
			break;
		}
	}
	if(!flg)
		{
		this->DeleteCol(Grid,Col);
		}
		else
		{
			if(isCoil(param) && Col<Grid->ColCount-1)
				{
				for(Col=Col+1; Col<Grid->ColCount; Col++)
					{
						this->DeleteCol(Grid,Col);
					}
				break;
				}
		}
	flg=false;
}
if(Grid->ColCount<4)
			{
				this->InsertCol(Grid,Grid->ColCount-1);
			}
}
//---------------------------------------------------------------------------
void TLadGraph::CompressThisGrid(TStringGrid *Grid)
{
if(Grid==NULL) return;
CompressThisRows(Grid);
CompressThisCols(Grid);
/*while(1)
{
if(Grid->ColCount<4)
	{
	  this->InsertCol(Grid,Grid->ColCount-1);
	}
else break;
} */
AlignCols();
}
//---------------------------------------------------------------------------
void TLadGraph::CompressThisRows(TStringGrid *Grid)
{
	bool flg = false;
	CellParam param;
	for(int Row=0; Row<Grid->RowCount; Row++)
	{
		flg=false;
		for(int Col=1; Col<Grid->ColCount;Col++)
		{
		 param = GetCellParam(Grid->Cells[Col][Row]);
		 if(param.Param!=LINE_DOW	&&
			param.Param!="")
			{
				flg=true;
				break;
			}
		}
		if(!flg)
		{
			this->DeleteRow(Grid,Row);
			Row=0;
		}
	}
}
//---------------------------------------------------------------------------
void TLadGraph::AlignCols()
{
 int maxcol = 0;
if(nets==NULL) return;
Nets * n = nets;
while(1)
{
	 if(n->Grid!=NULL)
	 {
		 if(n->Grid->ColCount>maxcol) maxcol = n->Grid->ColCount;
	 }
	 n=n->NextGrid;
	 if(n==NULL) break;
}
if(maxcol==0) return;
n = nets;
while(1)
{
	 if(n->Grid!=NULL)
	 {
		 if(n->Grid->ColCount<maxcol)
		 {
			 for(int i=n->Grid->ColCount; i<maxcol; i++)
			 {
			   InsertCol(n->Grid, n->Grid->ColCount-1);
			 }
		 }
	 }
	 n=n->NextGrid;
	 if(n==NULL) break;
}
}
//---------------------------------------------------------------------------
void TLadGraph::CanGridsResize(TStringGrid *Grid)
{
	 if(Grid->Width<(Grid->ColCount*Grid->DefaultColWidth+Grid->ColCount*2))
	 {
		 Grid->Height = (Grid->RowCount * Grid->DefaultRowHeight)+21;
	 }
	 else
	 {
		 Grid->Height = (Grid->RowCount * Grid->DefaultRowHeight)+2;
	 }
}
//---------------------------------------------------------------------------
void TLadGraph::OnGridsResize()
{
	 if(nets==NULL) return;
	 Nets * n = nets;
	 while(1)
	 {
	 if(n->Grid!=NULL) CanGridsResize(n->Grid);
	 n=n->NextGrid;
	 if(n==NULL) break;
	 }
}
//---------------------------------------------------------------------------
void TLadGraph::DeleteCol(TStringGrid *Grid, int Col)
{
	TMyGrid * gr = ((TMyGrid*)Grid);
	for(int i=1; i<Grid->RowCount; i++) Grid->Cells[Col][i]="";
	gr->DeleteColumn(Col);
	CanGridsResize(Grid);
}
//---------------------------------------------------------------------------
void TLadGraph::DeleteRow(TStringGrid *Grid, int Row)
{
	TMyGrid * gr = ((TMyGrid*)Grid);
	for(int i=1; i<Grid->ColCount; i++) Grid->Cells[i][Row]="";
	gr->DeleteRow(Row);
	CanGridsResize(Grid);
}
//---------------------------------------------------------------------------
void TLadGraph::InsertRow(TStringGrid *Grid, int CurRow)
{
  CellParam param;
  if(CurRow<Grid->RowCount-1)
  {
  Grid->RowCount++;
  TMyGrid * gr = ((TMyGrid*)Grid);
  for(int i = Grid->RowCount-1; i>CurRow+1; i--)
  {
   gr->MoveRow(i-1, i);
  }
  //gr->Rows[CurRow+1]->Clear();
  for(int i=1; i<Grid->ColCount; i++) Grid->Cells[i][CurRow+1]="";
  for(int i=1; i<=Grid->ColCount-1; i++)
  {
	param = GetCellParam(Grid->Cells[i][CurRow]);
	if(param.Param==LINE_DF	||
			param.Param==LINE_UD	||
			param.Param==LINE_X		||
			param.Param==LINE_UDF	||
			param.Param==LINE_DOW)
		{
			param.Param=LINE_DOW;
			param.Value = "";
			Grid->Cells[i][CurRow+1] = this->SetCellParam(&param);
		}
  }
  }
  else
  {
  Grid->RowCount++;
  for(int i=1; i<Grid->ColCount; i++) Grid->Cells[i][CurRow+1]="";
  }
  CanGridsResize(Grid);
}
//---------------------------------------------------------------------------
void TLadGraph::InsertCol(TStringGrid *Grid, int CurCol)
{
	TMyGrid * gr = ((TMyGrid*)Grid);
	gr->ColCount++;
	for(int i = Grid->ColCount-1; i>CurCol; i--)
	{
		gr->MoveColumn(i-1,i);
	}
	CellParam param;
	param.Param=LINE;
	param.Value="";
	CellParam temp;
	for(int i=0; i<Grid->RowCount; i++)
	{
	  temp = GetCellParam(Grid->Cells[CurCol-1][i]);
	  if((temp.Param!=LINE_U && temp.Param!=LINE_UD && temp.Param!=LINE_DOW && temp.Param!="")|| (CurCol==1 && (temp.Param!="" || i==0)))
		{
		  Grid->Cells[CurCol][i] = this->SetCellParam(&param);
		}
	  else Grid->Cells[CurCol][i] = "";
	}
	CanGridsResize(Grid);
}
//------------------------------------------------------------------------------
bool TLadGraph::AddContactOrT(TStringGrid *Grid, CellParam *param)
{
   int Row = Grid->Row;
   int Col = Grid->Col;
   if(Col==0) return false;
   CellParam temp;
   temp = GetCellParam(Grid->Cells[Col][Row]);
   if(this->isOr(temp)) return false;
   if(temp.Param=="") return false;
   if(this->isCoil(temp)) //попытка добавить туда, где COIL
   {
		  this->InsertCol(Grid,Col);
		  Grid->Cells[Col][Row]=SetCellParam(param);
		  Grid->Row=Row;
		  Grid->Col=Col;
		  CompressThisGrid(Grid);
		  return true;
   }
   else
   {
	  if(this->isFBD(temp))
	  {
	  this->InsertCol(Grid,Col+1);
	  Col++;
	  Grid->Col=Col;
	  }
	  else
	  {
      for(int i=Col-1; i>=1; i--)
		{
			CellParam temp;
			temp = GetCellParam(Grid->Cells[i][Row]);
			if(temp.Param==LINE)
				{
				Col=i;
				Grid->Col=Col;
				}
			else break;
		}
	  }
	  Grid->Cells[Col][Row]=SetCellParam(param);
	  CompressThisGrid(Grid);
	  return true;
   }
}
//---------------------------------------------------------------------------
bool TLadGraph::Add_NO(TStringGrid *Grid, UnicodeString Parameter)
{
   if(Grid==NULL) return false;
   CellParam param;
   param.Param = NO;
   param.Value = Parameter;
   return AddContactOrT(Grid, &param);
}
//---------------------------------------------------------------------------
bool TLadGraph::Add_NC(TStringGrid *Grid, UnicodeString Parameter )
{
   if(Grid==NULL) return false;
   CellParam param;
   param.Param = NC;
   param.Value = Parameter;
   return AddContactOrT(Grid, &param);
}
//---------------------------------------------------------------------------
bool TLadGraph::AddRelays(TStringGrid *Grid, CellParam *param)
{
   int Row = 0;
   int Col = Grid->ColCount-1;
   Grid->Cells[Col][Row]=SetCellParam(param);
   CompressThisGrid(Grid);
   return true;
}
//---------------------------------------------------------------------------
void TLadGraph::DrawCell(TObject *Sender, int ACol, int ARow, TRect Rect)
{
	 TStringGrid * Grid = (TStringGrid*) Sender;
	 if(ACol==0) return;
	 if(Grid==NULL) return;
	 CellParam param = GetCellParam(Grid->Cells[ACol][ARow]);
	 CanGridsResize(Grid);
	 DrawCelll(Grid,&param, Rect,ACol,ARow);
}
//---------------------------------------------------------------------------
void TLadGraph::MyTextOut(TStringGrid *Grid, int x, int y, UnicodeString Text)
{
int h = Grid->Canvas->TextHeight(Text);
int w = Grid->Canvas->TextWidth(Text);
Graphics::TBitmap * bmp = new  Graphics::TBitmap();
bmp->Height = h+1;
bmp->Width = w+1;
bmp->Canvas->TextOutA(0,0,Text);
Grid->Canvas->Draw(x,y,bmp);
delete bmp;
}
//---------------------------------------------------------------------------
void TLadGraph::SetCurrentGrid(TStringGrid *Grid)
{
if(CurrentGrid!=NULL)
{
	CurrentGrid->Invalidate();
}
CurrentGrid = Grid;
}
//---------------------------------------------------------------------------
TStringGrid * TLadGraph::GetCurrentGrid()
{
 return CurrentGrid;
}
//---------------------------------------------------------------------------
void TLadGraph::DrawCelll(TStringGrid *Grid ,CellParam * param, TRect Rect, int ACol, int ARow)
{
	Graphics::TBitmap * bmp = new  Graphics::TBitmap();
	bmp->Width = Grid->DefaultColWidth;
	bmp->Height = Grid->DefaultRowHeight;
	if(param->Param!="")
	{
	try
	{
	Data->GetBitmap(param->Param.ToInt(),bmp);
	}
	catch(...)
	{}
	Grid->Canvas->Draw(Rect.Left-MINUSX,Rect.Top,bmp);
	delete bmp;
	}
	else
	{
	Grid->Canvas->Draw(Rect.Left-MINUSX,Rect.Top,bmp);
    }
	if(ACol==1)
		{
		 TPoint p;
		 p.X = Rect.Left-MINUSX;
		 p.Y = Rect.Top;
		 Grid->Canvas->PenPos = p;
		 Grid->Canvas->Pen->Color = clBlack;
		 Grid->Canvas->LineTo(Rect.Left-MINUSX,Rect.Bottom);
		 Grid->Canvas->LineTo(Rect.Left+1-MINUSX,Rect.Bottom);
		 Grid->Canvas->LineTo(Rect.Left+1-MINUSX,Rect.Top);
		 Grid->Canvas->LineTo(Rect.Left+2-MINUSX,Rect.Top);
		 Grid->Canvas->LineTo(Rect.Left+2-MINUSX,Rect.Bottom);
		}
		else if(ACol==(Grid->ColCount-1))
		{
		 TPoint p;
		 p.X = Rect.Right;
		 p.Y = Rect.Top;
		 Grid->Canvas->PenPos = p;
		 Grid->Canvas->Pen->Color = clBlack;
		 Grid->Canvas->LineTo(Rect.Right,Rect.Bottom);
		 Grid->Canvas->LineTo(Rect.Right-1,Rect.Bottom);
		 Grid->Canvas->LineTo(Rect.Right-1,Rect.Top);
		 Grid->Canvas->LineTo(Rect.Right-2,Rect.Top);
		 Grid->Canvas->LineTo(Rect.Right-2,Rect.Bottom);
		}
if(Grid==this->CurrentGrid && ACol==Grid->Col && ARow==Grid->Row)
{
	if(this->isFBD(*param) || this->isCoil(*param) || param->Param==LINE)
	{
	Grid->Canvas->Pen->Style=psDot;
	Grid->Canvas->Pen->Color=clRed;
	Grid->Canvas->Brush->Color=clWhite;
	Grid->Canvas->Pen->Width=1;
	Grid->Canvas->MoveTo(Rect.Left, Rect.Top);
	Grid->Canvas->LineTo(Rect.Left, Rect.Bottom-1);
	Grid->Canvas->LineTo(Rect.Right-1, Rect.Bottom-1);
	Grid->Canvas->LineTo(Rect.Right-1, Rect.Top);
	Grid->Canvas->LineTo(Rect.Left, Rect.Top);
	Grid->Canvas->Pen->Style=psSolid;
	Grid->Canvas->Pen->Color=clBlack;
    }
}
	if(param->Value=="") return;
	int h = Grid->Canvas->TextHeight(param->Value);
	int w = Grid->Canvas->TextWidth(param->Value);
	if(param->Param!=TOF && param->Param!=TON)
	{
	 MyTextOut(Grid,GetCenterH(Rect)-w/2-MINUSX+2,GetCenterV(Rect)-1.5*h,param->Value);
	}
	else
	{
	 MyTextOut(Grid,GetCenterH(Rect)+15,GetCenterV(Rect)-h-8,param->Value);
	 MyTextOut(Grid,Rect.Left+10,GetCenterV(Rect)-h+4,param->Value2+"s");
	}

}
//---------------------------------------------------------------------------
TStringGrid * TLadGraph::CreateNetwork(TWinControl* AOwner, TWinControl * Parrent)
{
if(nets==NULL)
	{
	nets = new Nets;
	nets->PrevGrid = NULL;
	nets->NextGrid = NULL;
	nets->Grid = NULL;
	nets->idx = 0;
	CurrentGrid=NULL;
	Networks=0;
    }
Nets * s = nets;
Nets * b = s->PrevGrid;
 while(1)
 {
 if(s==NULL)
	{
	  s=new Nets;
	  s->Grid = new TStringGrid(AOwner);
	  s->Grid->Visible=false;
	  s->idx = b->idx+1;
	  s->PrevGrid = b;
	  if(b!=NULL) b->NextGrid = s;
	  s->NextGrid = NULL;
	  break;
	}
	else if(s->Grid==NULL)
	{
	  s->Grid = new TStringGrid(AOwner);
	  s->Grid->Visible=false;
	  s->idx = 1;
	  s->PrevGrid = b;
	  if(b!=NULL) b->NextGrid = s;
	  s->NextGrid = NULL;
	  break;
	}
	else
	{
		b = s;
		s=s->NextGrid;
	}
 }
 s->Grid->Parent = Parrent;
 SetGridParams(s);
 Networks++;
 s->Grid->Row=0;
 s->Grid->Col = 1;
 this->CurrentGrid = s->Grid;
 this->InvalidateGrids();
return s->Grid;
}
//---------------------------------------------------------------------------
TLadGraph::TLadGraph(TImageList *list)
{
Data = list;
nets = new Nets;
nets->PrevGrid = NULL;
nets->NextGrid = NULL;
nets->Grid = NULL;
nets->idx = 0;
CurrentGrid=NULL;
Networks=0;
}
//---------------------------------------------------------------------------
int TLadGraph::GetCenterV(TRect Rect)
{
	return Rect.Top+(Rect.Bottom-Rect.Top)/2;
}
//---------------------------------------------------------------------------
int TLadGraph::GetCenterH(TRect Rect)
{
	return Rect.Left+(Rect.Right-Rect.Left)/2;
}
//---------------------------------------------------------------------------
CellParam TLadGraph::GetCellParam(UnicodeString CellText)
{
	UnicodeString temp;
	CellParam param;
	param.Param="";
	param.Value="";
	param.Value2="";
	if(CellText=="") return param;
	int pos =  CellText.Pos(DELIM);
	if(pos==0) return param;
	param.Param = CellText.SubString(1,pos-1);
	temp=CellText.SubString(pos+1,CellText.Length());
	pos =  temp.Pos(DELIM);
	if(pos==0)
	{
	param.Value=temp;
	return param;
	}
	param.Value = temp.SubString(1,pos-1);
	param.Value2=temp.SubString(pos+1,temp.Length());
	return param;
}
//---------------------------------------------------------------------------
UnicodeString TLadGraph::SetCellParam(CellParam * Param)
{
  UnicodeString Res;
  Res=Param->Param+UnicodeString(DELIM)+Param->Value;
  if(Param->Param==TOF || Param->Param==TON)
	{
		Res = Res+UnicodeString(DELIM)+Param->Value2;
    }
  return Res;
}
//---------------------------------------------------------------------------
void TLadGraph::SetGridParams(Nets * s)
{
  s->Grid->Visible=false;
 s->Grid->Height = 51;
 if(s->PrevGrid!=NULL)
 {
	if(s->PrevGrid->Grid!=NULL) s->Grid->Top = s->PrevGrid->Grid->Top + s->PrevGrid->Grid->Height;
	else s->Grid->Top=0;
 }
 else s->Grid->Top=0;
 s->Grid->Align = alTop;
 //s->Grid->Anchors << akLeft;
 //s->Grid->Anchors << akTop;
 s->Grid->BevelInner = bvNone;
 s->Grid->BevelOuter = bvNone;
 s->Grid->ColCount = 4;
 s->Grid->Ctl3D = false;
 s->Grid->DefaultColWidth = 98;
 s->Grid->DefaultRowHeight= 49;
 s->Grid->FixedColor = clGray;
 s->Grid->RowCount = 1;
  s->Grid->Width = (s->Grid->ColCount) * s->Grid->DefaultColWidth+s->Grid->ColCount*2;
 s->Grid->FixedRows = 0;
 s->Grid->Options.Clear();
 s->Grid->Options = s->Grid->Options<<goFixedVertLine;
 s->Grid->ParentCtl3D=false;
 s->Grid->TabOrder = s->idx;
 s->Grid->DrawingStyle = gdsClassic;
 s->Grid->Visible = true;
 s->Grid->Cells[0][0] = "Action "+UnicodeString(s->idx);
 CellParam param;
 param.Param = LINE;
 param.Value = "";
 s->Grid->Cells[1][0] = SetCellParam(&param);
 s->Grid->Cells[2][0] = SetCellParam(&param);
 param.Param = COIL;
 param.Value = "???";
 s->Grid->Cells[3][0] = SetCellParam(&param);
 s->Grid->ScrollBars = ssBoth;
 s->Grid->Visible=true;
 s->Grid->Tag = s->idx;
 AlignCols();
}
//---------------------------------------------------------------------------
bool TLadGraph::Add_Coil(TStringGrid *Grid, UnicodeString Parameter)
{
   if(Grid==NULL) return false;
   CellParam param;
   param.Param = COIL;
   param.Value = Parameter;
   return AddRelays(Grid, &param);
}
//---------------------------------------------------------------------------
bool TLadGraph::Add_SET_Coil(TStringGrid *Grid, UnicodeString Parameter)
{
   if(Grid==NULL) return false;
   CellParam param;
   param.Param = SE;
   param.Value = Parameter;
   return AddRelays(Grid, &param);
}
//---------------------------------------------------------------------------
bool TLadGraph::Add_RESET_Coil(TStringGrid *Grid, UnicodeString Parameter)
{
   if(Grid==NULL) return false;
   CellParam param;
   param.Param = RE;
   param.Value = Parameter;
   return AddRelays(Grid, &param);
}
//---------------------------------------------------------------------------
bool TLadGraph::Add_TON(TStringGrid *Grid, UnicodeString TimerName, int Sec)
{
   if(Grid==NULL) return false;
   CellParam param;
   param.Param = TON;
   param.Value = TimerName;
   param.Value2 = UnicodeString(Sec);
   return AddContactOrT(Grid, &param);
}
//---------------------------------------------------------------------------
bool TLadGraph::Add_TOF(TStringGrid *Grid, UnicodeString TimerName, int Sec)
{
   if(Grid==NULL) return false;
   CellParam param;
   param.Param = TOF;
   param.Value = TimerName;
   param.Value2 = UnicodeString(Sec);
   return AddContactOrT(Grid, &param);
}
//---------------------------------------------------------------------------
bool TLadGraph::AddDownContactOrT(TStringGrid *Grid, CellParam *param)
{
int Row = Grid->Row;
int Col = Grid->Col;
if(Col==0) return false;
CellParam temp;
temp = GetCellParam(Grid->Cells[Col][Row]);
if(temp.Param!=NC && temp.Param!=NO && temp.Param!=TON && temp.Param!=TOF) return false;
if(Row==Grid->RowCount-1 || Grid->Cells[Col][Row+1]!="")
{
this->InsertRow(Grid,Row);
}
Row++;
Grid->Cells[Col][Row]=SetCellParam(param);
Grid->Row=Row;
Grid->Col=Col;
temp = GetCellParam(Grid->Cells[Col+1][Row-1]);
if(temp.Param==NC || temp.Param==NO || temp.Param==RE || temp.Param==SE || temp.Param==TOF || temp.Param==TON || temp.Param==COIL)
{
this->InsertCol(Grid,Col+1);
temp.Param=LINE_U;
temp.Value = "";
Grid->Cells[Col+1][Row] = SetCellParam(&temp);
temp.Param=LINE_DF;
temp.Value = "";
Grid->Cells[Col+1][Row-1] = SetCellParam(&temp);
}
else
{
	 if(temp.Param==LINE) temp.Param=LINE_DF;
	 else if(temp.Param==LINE_U) temp.Param=LINE_UD;
	 else if(temp.Param==LINE_UF) temp.Param=LINE_X;
	 else if(temp.Param==LINE_FU) temp.Param=LINE_UDF;
	 Grid->Cells[Col+1][Row-1] = SetCellParam(&temp);
	 temp = GetCellParam(Grid->Cells[Col+1][Row]);
	 if(temp.Param=="")
		{
			temp.Param = LINE_U;
			Grid->Cells[Col+1][Row] = SetCellParam(&temp);
		}
	 else if(temp.Param==LINE_DOW)
		{
			temp.Param = LINE_UD;
			Grid->Cells[Col+1][Row] = SetCellParam(&temp);
		}
	 else if(temp.Param==LINE_UDF)
		{
			temp.Param = LINE_X;
			Grid->Cells[Col+1][Row] = SetCellParam(&temp);
		}
	 else if(temp.Param==LINE_FU)
		{
			temp.Param = LINE_UF;
			Grid->Cells[Col+1][Row] = SetCellParam(&temp);
		}
}
//обработка левого поля
if(Col==1)
{
CompressThisGrid(Grid);
return true;
}
temp = GetCellParam(Grid->Cells[Col-1][Row-1]);
if(temp.Param==NC || temp.Param==NO || temp.Param==TOF || temp.Param==TON)
{
	this->InsertCol(Grid,Col);
	Col++;
	Grid->Col=Col;
	temp.Param = LINE_DF;
	temp.Value = "";
	Grid->Cells[Col-1][Row-1] = SetCellParam(&temp);
	temp.Param = LINE_FU;
	temp.Value = "";
	Grid->Cells[Col-1][Row] = SetCellParam(&temp);
}
else
{
   	 if(temp.Param==LINE) temp.Param=LINE_DF;
	 else if(temp.Param==LINE_U) temp.Param=LINE_UD;
	 else if(temp.Param==LINE_UF) temp.Param=LINE_X;
	 else if(temp.Param==LINE_FU) temp.Param=LINE_UDF;
	 Grid->Cells[Col-1][Row-1] = SetCellParam(&temp);
	 temp = GetCellParam(Grid->Cells[Col-1][Row]);
	 if(temp.Param=="")
		{
			temp.Param = LINE_FU;
			Grid->Cells[Col-1][Row] = SetCellParam(&temp);
		}
	 if(temp.Param==LINE || temp.Param==LINE_U)
		{
			temp.Param = LINE_UF;
			Grid->Cells[Col-1][Row] = SetCellParam(&temp);
		}
	 if(temp.Param==LINE_UD)
		{
			temp.Param = LINE_X;
			Grid->Cells[Col-1][Row] = SetCellParam(&temp);
		}
	 if(temp.Param==LINE_DOW)
		{
			temp.Param = LINE_UDF;
			Grid->Cells[Col-1][Row] = SetCellParam(&temp);
		}
}
CompressThisGrid(Grid);
return true;
}
//---------------------------------------------------------------------------
bool TLadGraph::Add_DOWN_NC(TStringGrid *Grid, UnicodeString Parameter)
{
   if(Grid==NULL) return false;
   CellParam param;
   param.Param = NC;
   param.Value = Parameter;
   return AddDownContactOrT(Grid, &param);
}
//---------------------------------------------------------------------------
bool TLadGraph::Add_DOWN_NO(TStringGrid *Grid, UnicodeString Parameter )
{
   if(Grid==NULL) return false;
   CellParam param;
   param.Param = NO;
   param.Value = Parameter;
   return AddDownContactOrT(Grid, &param);
}
//---------------------------------------------------------------------------
bool TLadGraph::Add_DOWN_TON(TStringGrid *Grid, UnicodeString Parameter, int Sec)
{
   if(Grid==NULL) return false;
   CellParam param;
   param.Param = TON;
   param.Value = Parameter;
   param.Value2 = UnicodeString(Sec);
   return AddDownContactOrT(Grid, &param);
}
//---------------------------------------------------------------------------
bool TLadGraph::Add_DOWN_TOF(TStringGrid *Grid, UnicodeString Parameter, int Sec)
{
   if(Grid==NULL) return false;
   CellParam param;
   param.Param = TOF;
   param.Value = Parameter;
   param.Value2 = UnicodeString(Sec);
   return AddDownContactOrT(Grid, &param);
}
//---------------------------------------------------------------------------
UnicodeString TLadGraph::CompileCell(CellParam param)
{
	if(param.Param==COIL)
		{
		  return UnicodeString(MOV)+UnicodeString(param.Value);
		}
	if(param.Param==NC)
		{
		  return UnicodeString(NOT)+AnsiString(param.Value);
		}
	if(param.Param==NO)
		{
		  return UnicodeString(OT)+UnicodeString(param.Value);
		}
	if(param.Param==RE)
		{
		  return UnicodeString(RES)+UnicodeString(param.Value);
		}
	if(param.Param==SE)
		{
		  return UnicodeString(SET)+UnicodeString(param.Value);
		}
	if(param.Param==TOF)
		{
		  return UnicodeString(TF)+UnicodeString(param.Value)+UnicodeString(DELIM)+UnicodeString(param.Value2)+UnicodeString(DELIM);
		}
	if(param.Param==TON)
		{
		  return UnicodeString(TN)+UnicodeString(param.Value)+UnicodeString(DELIM)+UnicodeString(param.Value2)+UnicodeString(DELIM);
		}
return "";
}
//---------------------------------------------------------------------------
bool TLadGraph::isFBD(CellParam param)
{
  if(param.Param==NC || param.Param==NO || param.Param==TON || param.Param==TOF) return true;
  return false;
}
//---------------------------------------------------------------------------
bool TLadGraph::isCoil(CellParam param)
{
  if(param.Param==COIL || param.Param==RE && param.Param==SE) return true;
  return false;
}
//---------------------------------------------------------------------------
bool TLadGraph::isAnd(CellParam param)
{
  if(param.Param==LINE || isFBD(param)) return true;
  return false;
}
//---------------------------------------------------------------------------
bool TLadGraph::isOr(CellParam param)
{
  if(param.Param==LINE_U || param.Param==LINE_UF || param.Param==LINE_UD || param.Param==LINE_X ||
  param.Param==LINE_DF ||
  param.Param==LINE_FU ||
  param.Param==LINE_UDF ||
  param.Param==LINE_DOW) return true;
  return false;
}
//---------------------------------------------------------------------------
UnicodeString TLadGraph::CompileRowString(TStringGrid *Grid, int Arow, int StartCol, int &EndCol)
{
UnicodeString result;
CellParam temp;
int n;
for(n = StartCol; n<Grid->ColCount; n++)
{
	temp = GetCellParam(Grid->Cells[n][Arow]);
	if(isFBD(temp)) //если это функциональный блок
	{
		result = result + AND + CompileCell(temp);
	}
	else if(isOr(temp))	break;
	else if(isCoil(temp)) break;
}
if(result.Length()>=1)
{
if(result.operator [](1)==UnicodeString(AND).operator [](1))
{
	result=result.SubString(2,result.Length());
}
if(result.operator [](result.Length())==UnicodeString(AND).operator [](1))
{
	result=result.SubString(1,result.Length()-1);
}
}
//if(result!="") result = "("+result+")";
EndCol = n;
return result;
}
//---------------------------------------------------------------------------
UnicodeString TLadGraph::CompileGraph(TStringGrid *Grid)
{
TStringList *list = new TStringList();
UnicodeString s;
UnicodeString t;
UnicodeString res;
CellParam temp;
int Col=1;
int Row=0;
int Ecol=0;
int tmp=0;
for(int i=0; i<Grid->RowCount; i++)
{
	list->Add("");
}
while(Col<Grid->ColCount-1)
{
	for(int Arow = 0; Arow<Grid->RowCount; Arow++)
	{
	 s=CompileRowString(Grid,Arow,Col,tmp);
	 if(Ecol<tmp && Arow==0) Ecol=tmp;
	 if(Ecol<Grid->ColCount-1 && s!="")
		t=t+s+OR;
	 else if(s!="") t=t+AND+s;
	}
	if(t!="")
	{
	 if(t.operator [](t.Length())==UnicodeString(OR).operator [](1))
		{
			t=t.SubString(1,t.Length()-1);
		}
	 if(t.operator [](1)==UnicodeString(AND).operator [](1))
		{
			t=t.SubString(2,t.Length());
		}
	  res=res+AND+"("+t+")";
	  t="";
	}
	if(Ecol<Grid->ColCount) Col = Ecol+1;
	else break;
}
	 if(res!="")
	 {
	 if(res.operator [](1)==UnicodeString(AND).operator [](1))
		{
			res=res.SubString(2,res.Length());
		}
	 }
Col = Grid->ColCount-1;
Row=0;
temp = GetCellParam(Grid->Cells[Col][Row]);
res=res+CompileCell(temp);
//***************
//оптимизация
res=ReplaceData(res,"))"+UnicodeString(AND)+"((",")"+UnicodeString(AND)+"(");
res=ReplaceData(res,")"+UnicodeString(OR)+"(",UnicodeString(OR));
res=ReplaceData(res,"))",")");
res=ReplaceData(res,"((","(");
//***************
#ifdef debug
ShowMessage(res);
#endif
return res;
}
//---------------------------------------------------------------------------
UnicodeString TLadGraph::ReplaceData(UnicodeString Data, UnicodeString From, UnicodeString To)
{
UnicodeString s = Data;
int pos=0;
while(1)
{
pos=s.Pos(From);
if(pos==0) break;
else
	{
		s=s.SubString(1,pos-1)+To+s.SubString(pos+From.Length(),s.Length()+10);
    }
}
return s;
}
//---------------------------------------------------------------------------
CellParam TLadGraph::DeCompileCell(UnicodeString &s)
{
  CellParam Data;
  Data.Param="";
  Data.Value="";
  if(UnicodeString(s.operator [](1))==UnicodeString(NOT))
  {
   Data.Param=NC;
   Data.Value=s.SubString(2,3);
   s=s.SubString(5,s.Length());
   return Data;
  }
  else if(UnicodeString(s.operator [](1))==UnicodeString(OT))
  {
   Data.Param=NO;
   Data.Value=s.SubString(2,3);
   s=s.SubString(5,s.Length());
   return Data;
  }
  else if(UnicodeString(s.operator [](1))==UnicodeString(TF))
  {
   Data.Param=TOF;
   int pos = s.Pos(UnicodeString(DELIM));
   Data.Value=s.SubString(2,pos-2);
   s=s.SubString(pos+1,s.Length());
   pos = s.Pos(UnicodeString(DELIM));
   Data.Value2=s.SubString(1,pos-1);
   s=s.SubString(pos+1,s.Length());
   return Data;
  }
  else if(UnicodeString(s.operator [](1))==UnicodeString(TN))
  {
   Data.Param=TON;
   int pos = s.Pos(UnicodeString(DELIM));
   Data.Value=s.SubString(2,pos-2);
   s=s.SubString(pos+1,s.Length());
   pos = s.Pos(UnicodeString(DELIM));
   Data.Value2=s.SubString(1,pos-1);
   s=s.SubString(pos+1,s.Length());
   return Data;
  }
  else if(UnicodeString(s.operator [](1))==UnicodeString(MOV))
  {
   Data.Param=COIL;
   Data.Value=s.SubString(2,3);
   s=s.SubString(5,s.Length());
   return Data;
  }
  else if(UnicodeString(s.operator [](1))==UnicodeString(SET))
  {
   Data.Param=SE;
   Data.Value=s.SubString(2,3);
   s=s.SubString(5,s.Length());
   return Data;
  }
  else if(UnicodeString(s.operator [](1))==UnicodeString(RES))
  {
   Data.Param=RE;
   Data.Value=s.SubString(2,3);
   s=s.SubString(5,s.Length());
   return Data;
  }
   return Data;
}
//---------------------------------------------------------------------------
UnicodeString TLadGraph::GetCurrentCell(TStringGrid *Grid)
{
  return Grid->Cells[Grid->Col][Grid->Row];
}
//---------------------------------------------------------------------------
void TLadGraph::AddUp(TStringGrid *Grid, CellParam param)
{
	if(param.Param==NC) Add_NC(Grid, param.Value);
	else if(param.Param==NO) Add_NO(Grid, param.Value);
	else if(param.Param==TON || param.Param==TOF)
		{
         this->AddContactOrT(Grid, &param);
        }
	else if(param.Param==COIL) Add_Coil(Grid, param.Value);
	else if(param.Param==SE) Add_SET_Coil(Grid, param.Value);
	else if(param.Param==RE) Add_RESET_Coil(Grid, param.Value);
}
//---------------------------------------------------------------------------
void TLadGraph::AddDown(TStringGrid *Grid, CellParam param)
{
	if(param.Param==NC) Add_DOWN_NC(Grid, param.Value);
	else if(param.Param==NO) Add_DOWN_NO(Grid, param.Value);
	else if(param.Param==TON || param.Param==TOF)
	{
        this->AddDownContactOrT(Grid, &param);
    }
}
//---------------------------------------------------------------------------
void TLadGraph::DecompileBuffer2(TStringGrid *Grid, UnicodeString Buffer)
{
if(Grid==NULL) return;
if(Buffer=="") return;
CellParam param;
TDrawCellEvent DrawEvent = Grid->OnDrawCell;
//очищаем схему
int EndCol=Grid->Col;
int EndRow = Grid->Row;

for(int i=1; i<Grid->ColCount; i++) Grid->Cols[i]->Clear();
//****************************
Grid->RowCount = 1;
Grid->ColCount = 4;
Grid->DefaultColWidth = 98;
Grid->DefaultRowHeight= 49;
Grid->Height = 51;
param.Param = LINE;
param.Value = "";
Grid->Cells[1][0] = SetCellParam(&param);
Grid->Cells[2][0] = SetCellParam(&param);
param.Param = COIL;
param.Value = "???";
Grid->Cells[3][0] = SetCellParam(&param);
//**************************
Grid->Enabled=false;
//ищем действия в буфере
//*************************
UnicodeString s = Buffer;
Grid->Row = 0;
Grid->Col = 1;
int Level=0;
while(1)
{
if(s.Length()<4) break;
if(UnicodeString(s.operator [](1))=="(")
	{
	  s=s.SubString(2,s.Length());
	  Level++;
	}
else if(UnicodeString(s.operator [](1))==")")
	{
	  s=s.SubString(2,s.Length());
	  Level--;
	  if(Level==0)
		{
			Grid->Row = 0;
			this->InsertCol(Grid,Grid->ColCount-1);
			Grid->Col = Grid->ColCount-2;
		}
	}
else if(UnicodeString(s.operator [](1))==UnicodeString(OR))
	{
	  s=s.SubString(2,s.Length()+10);
	  param = DeCompileCell(s);
	  if(param.Param!="") AddDown(Grid,param);
	}
else if(UnicodeString(s.operator [](1))==UnicodeString(AND))
	{
	  s=s.SubString(2,s.Length()+10);
	  param = DeCompileCell(s);
	  if(param.Param!="") AddUp(Grid,param);
	}
else
	{
	  param = DeCompileCell(s);
	  if(param.Param!="") AddUp(Grid,param);
	  if(isCoil(param))
	  {
		break;
      }
	}
}
//*************************
Grid->Enabled=true;
Grid->OnDrawCell = DrawEvent;
if(Grid->ColCount<4) this->InsertCol(Grid,Grid->ColCount-1);
if(EndCol<Grid->ColCount-1) Grid->Col = EndCol;
else Grid->Col=1;
if(EndRow<Grid->RowCount-1) Grid->Row = EndRow;
else Grid->Row = 0;
Grid->FixedRows=0;
Grid->FixedCols=1;
}
//---------------------------------------------------------------------------
void TLadGraph::DecompileBuffer(TStringGrid *Grid, UnicodeString Buffer)
{
if(Grid==NULL) return;
if(Buffer=="") return;
TStringGrid *Grid1 = new TStringGrid(Grid->Owner);
Grid1->Visible=false;
Grid1->Parent =  Grid->Parent;
DecompileBuffer2(Grid1, Buffer);
TDrawCellEvent DrawEvent = Grid->OnDrawCell;
Grid->OnDrawCell=NULL;
Grid->ColCount = Grid1->ColCount;
Grid->RowCount = Grid1->RowCount;
for(int i=1; i<Grid1->ColCount; i++)
{
Grid->Cols[i]->Clear();
Grid->Cols[i]->Assign(Grid1->Cols[i]);
}
Grid->OnDrawCell = DrawEvent;
delete Grid1;
}
//---------------------------------------------------------------------------
bool TLadGraph::DeleteFBD(TStringGrid *Grid)
{
if(Grid==NULL) return false;
int Row = Grid->Row;
int Col = Grid->Col;
if(Col==0) return false;
if(Col==Grid->ColCount-1) return false;
CellParam temp;
temp = GetCellParam(Grid->Cells[Col][Row]);
if(temp.Param!=NC && temp.Param!=NO && temp.Param!=TON && temp.Param!=TOF) return false;

int res = MessageBox(Grid->Handle, AnsiString("Are you sure you want to delete this?").c_str(),AnsiString("Confirm please").c_str(), MB_YESNO|MB_ICONQUESTION);
if(res!= IDYES) return false;

temp.Param=LINE;
temp.Value="";
Grid->Cells[Col][Row]=SetCellParam(&temp);
UnicodeString Prog = CompileGraph(Grid);
DecompileBuffer(Grid, Prog);
AlignCols_Minus();
return true;
}
//---------------------------------------------------------------------------
bool TLadGraph::CurrentCellIsFBD()
{
CellParam temp;
TStringGrid * Grid = GetCurrentGrid();
if(Grid==NULL) return false;
temp = GetCellParam(Grid->Cells[Grid->Col][Grid->Row]);
if(temp.Param!=NC && temp.Param!=NO && temp.Param!=TON && temp.Param!=TOF) return false;
return true;
}
//---------------------------------------------------------------------------
bool TLadGraph::CoilTypeIsDirect(Nets *net, UnicodeString &CoilName)
{
CellParam temp;
if(net==NULL) return false;
if(net->Grid==NULL) return false;
temp = GetCellParam(net->Grid->Cells[net->Grid->ColCount-1][0]);
CoilName=temp.Value;
if(temp.Param==COIL) return true;
return false;
}
//---------------------------------------------------------------------------
bool TLadGraph::CellIsTimer(TStringGrid *Grid, int Arow, int Acol, int &TimerIndex)
{
CellParam temp;
if(Grid==NULL) return false;
temp = GetCellParam(Grid->Cells[Acol][Arow]);
if(temp.Param==TOF || temp.Param==TON)
{
   TimerIndex = temp.Value.SubString(2,2).ToInt();
   return true;
}
return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

