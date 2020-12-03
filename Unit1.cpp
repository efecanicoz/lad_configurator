//---------------------------------------------------------------------------

#include <vcl.h>
#include <Registry.hpp>
#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit8.h"
#include "Unit9.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	LadGraph = new TLadGraph(ImageList1);
	PosWrite=false;
	tw->FullExpand();
	Modified=false;
	InputsBox=NULL;
	MVars->DeleteRow(1);
	for(int i=0; i<MAXMVAR; i++)
	{
		if(i<=9)
			MVars->InsertRow("M0"+UnicodeString(i),"",true);
		else
			MVars->InsertRow("M"+UnicodeString(i),"",true);
	}

	Timers->DeleteRow(1);
	for(int i=0; i<MAXTVAR; i++)
	{
		if(i<=9)
			Timers->InsertRow("T0"+UnicodeString(i),"",true);
		else
			Timers->InsertRow("T"+UnicodeString(i),"",true);
	}

	counters->DeleteRow(1);
	for(int i=0; i<MAXCVAR; i++)
	{
		if(i<=9)
			counters->InsertRow("C0"+UnicodeString(i),"",true);
		else
			counters->InsertRow("C"+UnicodeString(i),"",true);
	}


	Tabs->ActivePageIndex=0;
	Unlocked=false;
	StartWindowName = Form1->Caption;
	BuildWindowAttributes("");
	progr = new TForm6(this);
	ProgRevision=1;
	frame2_count = 0;
}
//---------------------------------------------------------------------------
void  TForm1::GetFileVersion(LPCSTR filename,char * sVer, int slen)
{
	DWORD set_to_zero = 1;
    DWORD info_size = 0;
    info_size = GetFileVersionInfoSizeA(filename, &set_to_zero);
        if(info_size)
        {
            UINT *buffer = (UINT*) calloc(sizeof(UINT),info_size);
			if(GetFileVersionInfoA(filename, 0, info_size, buffer))
            {
                UINT value_size;
                PUINT version_buffer;
                if(VerQueryValueA(buffer, "\\", (LPVOID *)&version_buffer, &value_size))
                {
					VS_FIXEDFILEINFO *file_info = (VS_FIXEDFILEINFO *)version_buffer;
						sprintf_s(sVer,slen,"%d.%d.%d.%d%c",
						HIWORD(file_info->dwFileVersionMS),
						LOWORD(file_info->dwFileVersionMS),
						HIWORD(file_info->dwFileVersionLS),
						LOWORD(file_info->dwFileVersionLS),0);

                }
            }
            //DWORD err = GetLastError();
			free(buffer);
        }
}
//---------------------------------------------------------------------------
void TForm1::BuildWindowAttributes(UnicodeString _postfix)
{
UnicodeString Revision = UnicodeString((int)ProgRevision);
if(Revision.Length()<2) Revision="0"+Revision;
char str[50];
GetFileVersion(AnsiString(Application->ExeName).c_str(),str,50);
Form1->Caption = StartWindowName + " v."+UnicodeString(str)+_postfix;
StatusBar1->Panels->operator [](1)->Text="LAD diagramm revision: "+Revision;
}
//---------------------------------------------------------------------------
void TForm1::SetModified(bool Mod)
{
  if(Mod && !Modified)
	{
		Modified=true;
		BuildWindowAttributes("*");
		return;
	}
	else if(!Mod && Modified)
	{
		Modified=false;
		BuildWindowAttributes("");
		return;
    }
}
//---------------------------------------------------------------------------
bool TForm1::GetPrivileges()
{
	TPassInp * pin = new TPassInp(this);
	pin->ShowModal();
	UnicodeString pass = pin->Password;
	delete pin;
	if(pin->Result)
	{
	 UnicodeString MyHash;
	 UnicodeString YouHash;

	 MyHash = ReadHashFromReg();

	 if(MyHash=="") MyHash = GetPassHashString("1a2s3d4f");
	 YouHash = GetPassHashString(pass);
	 if(MyHash==YouHash) return true;
	}
return false;
}
//---------------------------------------------------------------------------
bool TForm1::WritePassHashToReg(UnicodeString Hash)
{
if(Hash=="") return false;
TRegistry *reg = new TRegistry;
try
{
reg->RootKey = HKEY_CURRENT_USER;
reg->OpenKey("Software\\SCSL", true);
reg->WriteString("PassHash",Hash);
}
catch(...)
{
	delete reg;
	return false;
}
delete reg;
return true;
}
//---------------------------------------------------------------------------
UnicodeString TForm1::ReadHashFromReg()
{
UnicodeString s;
TRegistry *reg = new TRegistry;
try
{
reg->RootKey = HKEY_CURRENT_USER;
reg->OpenKey("Software\\SCSL", false);
s=reg->ReadString("PassHash");
}
__finally
{
delete reg;
}
return s;
}
//---------------------------------------------------------------------------
UnicodeString TForm1::GetPassHashString(UnicodeString Password)
{
	AnsiString s = AnsiString(Password);
	unsigned long crc=this->getCRC(s.c_str(),s.Length());
	s=s.sprintf("0x%X",crc);
	return UnicodeString(s);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::StringGrid1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State)
{
	LadGraph->DrawCell(Sender,ACol,ARow,Rect);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::StringGrid1Click(TObject *Sender)
{
 LadGraph->SetCurrentGrid((TStringGrid*)Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::twClick(TObject *Sender)
{
if(!tw->Selected->HasChildren)
	{
	  if(tw->Selected->Text=="Inputs configuration")
		{
			Tabs->ActivePageIndex=0;
		}
	  else if(tw->Selected->Text=="Variables")
		{
			Tabs->ActivePageIndex=1;
		}
	  else if(tw->Selected->Text=="LAD Diagramm")
		{
			Tabs->ActivePageIndex=2;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ScrollBox1Resize(TObject *Sender)
{
LadGraph->OnGridsResize();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormShow(TObject *Sender)
{
TIniFile * ini = new TIniFile(ExtractFileDir(Application->ExeName)+"\\config.ini");
int state=0;
state = ini->ReadInteger("Position","WindowState",1);
if(state==1)
{
this->Top=ini->ReadInteger("Position","Top",this->Top);
this->Left=ini->ReadInteger("Position","Left",this->Left);
Form1->WindowState = wsMaximized;
}
else
{
Form1->WindowState = wsNormal;
this->Top=ini->ReadInteger("Position","Top",this->Top);
this->Left=ini->ReadInteger("Position","Left",this->Left);
this->Width = ini->ReadInteger("Position","Width",this->Width);
this->Height = ini->ReadInteger("Position","Height",this->Height);
}
delete ini;
NewExecute(Sender);
PosWrite=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ExitExecute(TObject *Sender)
{
Form1->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::NewActExecute(TObject *Sender)
{
	if(LadGraph->Networks>=MAXACTIONS)
	{
		MessageBox(this->Handle, AnsiString("Sorry, but the maximum number of Actions has been reached.").c_str(),AnsiString("Warning").c_str(), MB_ICONWARNING);
		return;
	}
	TStringGrid * grid = LadGraph->CreateNetwork(this,ScrollBox1);
	grid->OnDrawCell = (TDrawCellEvent)&StringGrid1DrawCell;
	grid->OnClick = StringGrid1Click;
	grid->Visible=true;
	grid->Invalidate();
	Application->ProcessMessages();
	SetModified(true);
}
//---------------------------------------------------------------------------
void TForm1::RefreshTimersInGrid(TStringGrid * grid)
{
if(grid==NULL) return;
int TimerIndex = 0;

for(int Row=0; Row<grid->RowCount; Row++)
{
	for(int Col=1; Col<grid->ColCount-1; Col++)
	{
		if(LadGraph->CellIsTimer(grid,Row,Col,TimerIndex))
		{
			TimersUsed[TimerIndex]=true;
		}
	}
}
}
//---------------------------------------------------------------------------
void TForm1::RefreshAllTimers()
{
	for(int i=0; i<MAXTVAR; i++)
		{
		  TimersUsed[i]=false;
		}
	if(LadGraph->nets==NULL)
	{
	return;
	}
	Nets * net = LadGraph->nets;
	while(1)
	{
	   if(net==NULL) return;
	   if(net->Grid!=NULL)
		{
		   RefreshTimersInGrid(net->Grid);
		}
		net=net->NextGrid;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteActExecute(TObject *Sender)
{
TStringGrid * grid = LadGraph->GetCurrentGrid();
if(grid==NULL) return;
int TimerIndex = 0;
bool TimersUsed2[MAXTVAR];
memcpy(&TimersUsed2,&TimersUsed, sizeof(bool)*MAXTVAR);
for(int Row=0; Row<grid->RowCount; Row++)
{
	for(int Col=1; Col<grid->ColCount-1; Col++)
	{
		if(LadGraph->CellIsTimer(grid,Row,Col,TimerIndex))
		{
			TimersUsed2[TimerIndex]=false;
		}
	}
}
if(LadGraph->DeleteCurrentAction(false))
{
memcpy(&TimersUsed,&TimersUsed2, sizeof(bool)*MAXTVAR);
SetModified(true);
}
RefreshCiolsUsing();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::inputsDblClick(TObject *Sender)
{
	int i;
	if(!this->Unlocked)
		return;

	if(inputs->Col>0 && inputs->Cells[0][inputs->Row].Pos("S"))
	{
		if(InputsBox==NULL)
		{
			InputsBox = new TComboBox(Form1);
			InputsBox->Parent= inputs->Parent;
		}

		InputsBox->Items->Clear();

		for(i = 1; i < 64; i++)
		{
			InputsBox->AddItem(UnicodeString(i), NULL);
		}

		InputsBox->ItemIndex = inputs->Cells[inputs->Col][inputs->Row].ToInt() - 1;

		InputsBox->Left=inputs->DefaultColWidth*inputs->Col+inputs->Left+3;
		InputsBox->Top =inputs->DefaultRowHeight*inputs->Row+inputs->Top+1+inputs->Row;
		InputsBox->Width = inputs->DefaultColWidth+1;
		InputsBox->Height = inputs->DefaultRowHeight+1;
		InputsBox->Visible=true;
		InputsBox->OnSelect = ComboBox1Select;
		InputsBox->OnKeyPress = ComboBox1KeyPress;
		InputsBox->SetFocus();
	}
	else if(inputs->Col>0 && !inputs->Cells[0][inputs->Row].Pos("F"))
	{
		if(InputsBox==NULL)
		{
			InputsBox = new TComboBox(Form1);
			InputsBox->Parent= inputs->Parent;
		}

		InputsBox->Items->Clear();
		InputsBox->AddItem("Normal",NULL);
		InputsBox->AddItem("Pull up to Vcc (9...32V)",NULL);

		if(inputs->Cells[inputs->Col][inputs->Row]=="Normal")
		{
			InputsBox->ItemIndex=0;
		}
		else
		{
			InputsBox->ItemIndex=1;
		}
		Application->ProcessMessages();

		InputsBox->Left=inputs->DefaultColWidth*inputs->Col+inputs->Left+3;
		InputsBox->Top =inputs->DefaultRowHeight*inputs->Row+inputs->Top+1+inputs->Row;
		InputsBox->Width = inputs->DefaultColWidth+1;
		InputsBox->Height = inputs->DefaultRowHeight+1;
		InputsBox->Visible=true;
		InputsBox->OnSelect = ComboBox1Select;
		InputsBox->OnKeyPress = ComboBox1KeyPress;
		InputsBox->SetFocus();
	}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::ComboBox1Select(TObject *Sender)
{
try
{
inputs->Cells[inputs->Col][inputs->Row]=InputsBox->Text;
InputsBox->Visible=false;
SetModified(true);
}
catch(...)
{
InputsBox=NULL;
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AddVariableExecute(TObject *Sender)
{
	TFrame2 * frame;
	if(frame2_count == 0)
	{
		frame = new TFrame2(this, NULL, ScrollBox2);
		frame2_array[0] = frame;
		frame2_count = 1;
	}
	else
	{
		if(frame2_count >= MAXIOVAR)
		{
			MessageBox(this->Handle, AnsiString("Sorry, but the maximum number of variables of this type has been reached.").c_str(),AnsiString("Warning").c_str(), MB_ICONWARNING);
			return;
		}
		frame = new TFrame2(this, frame2_array[frame2_count-1], ScrollBox2);
		frame2_array[frame2_count] = frame;
        frame2_count++;
	}
	frame->Input->Clear();
	for(int i=1; i<(inputs->RowCount -1); i++)
	{
		frame->Input->Items->Add(inputs->Cells[0][i]);
	}

	for(int i=1; i<(counters->RowCount); i++)
	{
		frame->Input->Items->Add(counters->Cells[0][i]);
	}
	frame->Input->ItemIndex=MAXCOIL;
	frame->SetLock(!Unlocked);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::NewExecute(TObject *Sender)
{
	int i;
	TestModified();
	if(frame2_count != 0)
	{
		for(i = 0; i < frame2_count; i++)
		{
			delete frame2_array[i];
		}
		frame2_count = 0;
	}
	inputs->Strings->Clear();
	for(int i=0; i<MAXCOIL; i++)
	{
		if(i<=9)
			inputs->InsertRow("F0"+UnicodeString(i),"Y0"+UnicodeString(i)+" Load Feedback (0...100%)",true);
		else
			inputs->InsertRow("F"+UnicodeString(i),"Y"+UnicodeString(i)+" Load Feedback (0...100%)",true);
	}
	for(int i=0; i<MAXIVAR; i++)
	{
		if(i<=9)
			inputs->InsertRow("I0"+UnicodeString(i),"Normal",true);
		else
			inputs->InsertRow("I"+UnicodeString(i),"Normal",true);
	}
	inputs->InsertRow("Startup LED blink count", "1", true);

	MVars->Strings->Clear();
	for(int i=0; i<MAXMVAR; i++)
	{
		if(i<=9)
			MVars->InsertRow("M0"+UnicodeString(i),"",true);
		else
			MVars->InsertRow("M"+UnicodeString(i),"",true);
	}
	Timers->Strings->Clear();
	for(int i=0; i<MAXTVAR; i++)
	{
		if(i<=9)
			Timers->InsertRow("T0"+UnicodeString(i),"",true);
		else
			Timers->InsertRow("T"+UnicodeString(i),"",true);
		TimersUsed[i]=false;
	}

	counters->Strings->Clear();
	for(int i=0; i<MAXCVAR; i++)
	{
		if(i<=9)
			counters->InsertRow("C0"+UnicodeString(i),"",true);
		else
			counters->InsertRow("C"+UnicodeString(i),"",true);
	}

	outputs->Strings->Clear();
	for(int i=0; i<MAXCOIL; i++)
	{
		if(i<=9)
			outputs->InsertRow("Y0"+UnicodeString(i),"Power output No. "+UnicodeString(i+1),true);
		else
			outputs->InsertRow("Y"+UnicodeString(i),"Power output No. "+UnicodeString(i+1),true);
	}
	for (int i=0; i < MAXCOIL+MAXMVAR; i++)
	{
		CoilsUsed[i]=0;
	}
	if(LadGraph->nets!=NULL)
	{
		while(1)
		{
		 if(LadGraph->nets->Grid!=NULL)
			LadGraph->SetCurrentGrid(LadGraph->nets->Grid);
		 else break;
		 LadGraph->DeleteCurrentAction(true);
		}
	}
	NewActExecute(Sender);
	Tabs->TabIndex=0;
	LastFilename="";
	OpenDialog1->FileName="";
	SaveDialog1->FileName="";
	SetModified(false);
	this->BuildWindowAttributes("");
}
//---------------------------------------------------------------------------
void TForm1::PrepareContacts(TForm3 * frm)
{
	int i;
	frm->Var->Items->Clear();
	frm->Var->Text="";
	frm->Result=false;

	for(i = 0; i < frame2_count; i++)
	{
		frm->Var->AddItem(frame2_array[i]->Var, NULL);
		frame2_array[i]->SetLock(!Unlocked);
	}

	for(int i=1; i<MVars->RowCount; i++)
	{
		frm->Var->AddItem(MVars->Cells[0][i] , NULL);
	}
	if(frm->Var->Items->Count>0)
		frm->Var->ItemIndex=0;
	frm->PrepareToContact();
}
//---------------------------------------------------------------------------
bool TForm1::PrepareTimers(TForm3 * frm)
{
frm->Var->Items->Clear();
frm->Var->Text="";
frm->Result=false;
bool flg=false;
for(int i=1; i<Timers->RowCount; i++)
{
	if(!TimersUsed[i-1])
		{
		  frm->Var->AddItem(Timers->Cells[0][i] , NULL);
		  flg=true;
		}
}
if(frm->Var->Items->Count>0)
frm->Var->ItemIndex=0;
frm->PrepareToTimer();
return flg;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AddNOExecute(TObject *Sender)
{
	if(LadGraph->GetCurrentGrid()==NULL)
		return;
	TForm3 * frm = new TForm3(this);
	PrepareContacts(frm);
	frm->ShowModal();
	if(frm->Result)
	{
		LadGraph->Add_NO(LadGraph->GetCurrentGrid(),frm->Var->Text);
		SetModified(true);
	}
	delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AddNCExecute(TObject *Sender)
{
if(LadGraph->GetCurrentGrid()==NULL) return;
TForm3 * frm = new TForm3(this);
PrepareContacts(frm);
frm->ShowModal();
if(frm->Result)
	{
		LadGraph->Add_NC(LadGraph->GetCurrentGrid(),frm->Var->Text);
		SetModified(true);
	}
delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AddTExecute(TObject *Sender)
{
if(LadGraph->GetCurrentGrid()==NULL) return;
TForm3 * frm = new TForm3(this);
if(!PrepareTimers(frm))
{
MessageBox(this->Handle, AnsiString("Sorry, but all timers have already been used.").c_str(),AnsiString("Warning").c_str(), MB_ICONWARNING);
delete frm;
return;
}
frm->ShowModal();
if(frm->Result)
	{
		if(frm->typegroup->ItemIndex==0)
			LadGraph->Add_TON(LadGraph->GetCurrentGrid(),frm->Var->Text, frm->time->Value);
		else
			LadGraph->Add_TOF(LadGraph->GetCurrentGrid(),frm->Var->Text, frm->time->Value);
		UnicodeString s = frm->Var->Text.SubString(2,frm->Var->Text.Length());
		TimersUsed[s.ToInt()]=true;
		SetModified(true);
	}
delete frm;
}
//---------------------------------------------------------------------------
bool TForm1::TestDown()
{
return  LadGraph->CurrentCellIsFBD();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AddDownNOExecute(TObject *Sender)
{
if(LadGraph->GetCurrentGrid()==NULL) return;
if(!TestDown()) return;
TForm3 * frm = new TForm3(this);
PrepareContacts(frm);
frm->ShowModal();
if(frm->Result)
	{
		LadGraph->Add_DOWN_NO(LadGraph->GetCurrentGrid(),frm->Var->Text);
		SetModified(true);
	}
delete frm;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AddNCDownExecute(TObject *Sender)
{
if(LadGraph->GetCurrentGrid()==NULL) return;
if(!TestDown()) return;
TForm3 * frm = new TForm3(this);
PrepareContacts(frm);
frm->ShowModal();
if(frm->Result)
	{
		LadGraph->Add_DOWN_NC(LadGraph->GetCurrentGrid(),frm->Var->Text);
		SetModified(true);
	}
delete frm;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AddTDownExecute(TObject *Sender)
{
if(LadGraph->GetCurrentGrid()==NULL) return;
if(!TestDown()) return;
TForm3 * frm = new TForm3(this);
if(!PrepareTimers(frm))
{
MessageBox(this->Handle, AnsiString("Sorry, but all timers have already been used.").c_str(),AnsiString("Warning").c_str(), MB_ICONWARNING);
delete frm;
return;
}
frm->ShowModal();
if(frm->Result)
	{
		if(frm->typegroup->ItemIndex==0)
			LadGraph->Add_DOWN_TON(LadGraph->GetCurrentGrid(),frm->Var->Text, frm->time->Value);
		else
			LadGraph->Add_DOWN_TOF(LadGraph->GetCurrentGrid(),frm->Var->Text, frm->time->Value);
		UnicodeString s = frm->Var->Text.SubString(2,frm->Var->Text.Length());
		TimersUsed[s.ToInt()]=true;
		SetModified(true);
	}
delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AddCoilExecute(TObject *Sender)
{
if(LadGraph->GetCurrentGrid()==NULL) return;
TForm3 * frm = new TForm3(this);
if(!PrepareCoils(frm))
{
MessageBox(this->Handle, AnsiString("Sorry, but all coils with direct write have already been used.").c_str(),AnsiString("Warning").c_str(), MB_ICONWARNING);
delete frm;
return;
}
frm->ShowModal();
if(frm->Result)
	{
	   LadGraph->Add_Coil(LadGraph->GetCurrentGrid(),frm->Var->Text);
	   UnicodeString s = frm->Var->Text.SubString(2,frm->Var->Text.Length());
	   if(frm->Var->Text.operator [](1)=='Y') CoilsUsed[s.ToInt()-1]=true;
	   else CoilsUsed[s.ToInt()+MAXCOIL]=true;
	   SetModified(true);
	}
delete frm;
}
//---------------------------------------------------------------------------
bool TForm1::PrepareCoils(TForm3 * frm)
{
frm->Var->Items->Clear();
frm->Var->Text="";
frm->Result=false;
bool flg=false;
RefreshCiolsUsing();
for(int i=1; i<outputs->RowCount; i++)
{
	if(!CoilsUsed[i-1])
		{
		  frm->Var->AddItem(outputs->Cells[0][i] , NULL);
		  flg=true;
		}
}
for(int i=1; i<MVars->RowCount; i++)
{
if(!CoilsUsed[i-1+MAXCOIL])
	{
	frm->Var->AddItem(MVars->Cells[0][i] , NULL);
	flg=true;
	}
}
if(frm->Var->Items->Count>0)
frm->Var->ItemIndex=0;
frm->PrepareToContact();
return flg;
}
//---------------------------------------------------------------------------
void TForm1::RefreshCiolsUsing()
{
	UnicodeString CoilName;
	Nets * net = LadGraph->nets;
	for(int i=0; i<MAXCOIL+MAXMVAR; i++)
	{
		CoilsUsed[i]=false;
    }
	while(1)
	{
	  if(net==NULL) return;
	  if(LadGraph->CoilTypeIsDirect(net,CoilName))
		{
			if(CoilName!="???")
			{
				if(CoilName.operator [](1)=='Y')
				{
					CoilName=CoilName.SubString(2,2);
					int idx = CoilName.ToInt();
					CoilsUsed[idx]=true;
				}
				else
					{
					CoilName=CoilName.SubString(2,2);
					int idx = CoilName.ToInt()+MAXCOIL;
                    CoilsUsed[idx]=true;
                    }
            }
		}
		net=net->NextGrid;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AddSETExecute(TObject *Sender)
{
if(LadGraph->GetCurrentGrid()==NULL) return;
TForm3 * frm = new TForm3(this);
if(!PrepareCoils(frm))
{
MessageBox(this->Handle, AnsiString("Sorry, but all coils have already been used.").c_str(),AnsiString("Warning").c_str(), MB_ICONWARNING);
delete frm;
return;
}
frm->ShowModal();
if(frm->Result)
	{
	   LadGraph->Add_SET_Coil(LadGraph->GetCurrentGrid(),frm->Var->Text);
	   SetModified(true);
	}
delete frm;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AddResetCoilExecute(TObject *Sender)
{
if(LadGraph->GetCurrentGrid()==NULL) return;
TForm3 * frm = new TForm3(this);
if(!PrepareCoils(frm))
{
MessageBox(this->Handle, AnsiString("Sorry, but all coils have already been used.").c_str(),AnsiString("Warning").c_str(), MB_ICONWARNING);
delete frm;
return;
}
frm->ShowModal();
if(frm->Result)
	{
	   LadGraph->Add_RESET_Coil(LadGraph->GetCurrentGrid(),frm->Var->Text);
	   SetModified(true);
	}
delete frm;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DeleteExecute(TObject *Sender)
{
TStringGrid * grid = LadGraph->GetCurrentGrid();
if(grid==NULL) return;
int TimerIndex = 0;
bool IsDel = false;
if(LadGraph->CellIsTimer(grid,grid->Row,grid->Col,TimerIndex))
{
	IsDel=true;
}
if(IsDel && LadGraph->DeleteFBD(grid))
{
	TimersUsed[TimerIndex]=false;
}
else LadGraph->DeleteFBD(grid);
SetModified(true);
RefreshCiolsUsing();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CompExecute(TObject *Sender)
{
	UnicodeString s = LadGraph->CompileLADProgramm();
	RefreshAllTimers();
	if(s!="")
		{
		 MessageBox(this->Handle, AnsiString("At first glance, everything is fine ...").c_str(),AnsiString("Congratulations!").c_str(), MB_ICONINFORMATION);
		}
	else
		{
			if(LadGraph->nets->Grid==NULL)
			{
			  MessageBox(this->Handle, AnsiString("Something went wrong...").c_str(),AnsiString("Error").c_str(), MB_ICONERROR);
			}
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Save_asExecute(TObject *Sender)
{
UnicodeString s = LadGraph->CompileLADProgramm();
if(s=="")
	{
	 Tabs->ActivePageIndex=2;
	 MessageBox(this->Handle, AnsiString("Something went wrong...\r\nOperation not completed").c_str(),AnsiString("Error").c_str(), MB_ICONERROR);
	 return;
	}
UnicodeString FileName;
if(SaveDialog1->Execute(this->Handle))
{
	UnicodeString ext = ExtractFileExt(SaveDialog1->FileName);
	if(ext!=".scsl" && ext!=".SCSL")
	{
	  FileName = SaveDialog1->FileName+".scsl";
	}
	else  FileName = SaveDialog1->FileName;
	LastFilename = FileName;
	SaveProject(FileName);
}
else return;
}
//---------------------------------------------------------------------------

void TForm1::SaveProject(UnicodeString FileName)
{
	int i;
	if(!Unlocked)
		return;
	TFileStream * stream = new TFileStream(FileName,fmOpenReadWrite|fmOpenWrite|fmCreate);
	if(stream==NULL)
	{
		MessageBox(this->Handle, AnsiString("Something went wrong...\r\nFile not opened").c_str(),AnsiString("Error").c_str(), MB_ICONERROR);
		return;
	}
	TMemoryStream *s = new TMemoryStream;
	inputs->Strings->SaveToStream(s);
	uint16_t size = s->Size;
	stream->Write(&size,sizeof(uint16_t));
	s->SaveToStream(stream);
	s->Clear();
	//сохраняем иксы
	for(i = 0; i < frame2_count; i++)
	{
		uint16_t len=0;
		AnsiString t = AnsiString(frame2_array[i]->Condition_String);
		len=t.Length();
		s->Write(&len,sizeof(uint16_t));
		s->Write(t.c_str(), len);
	}
	if(frame2_count != 0)
	{
		stream->Write(&frame2_count,sizeof(uint16_t));
		s->SaveToStream(stream);
		s->Clear();
	}
	else
	{
	size=0;
	stream->Write(&size,sizeof(uint16_t));
	}
	//сохраняем Мки
	MVars->Strings->SaveToStream(s);
	size = s->Size;
	stream->Write(&size,sizeof(uint16_t));
	s->SaveToStream(stream);
	s->Clear();

	//сохраняем Таймеры
	Timers->Strings->SaveToStream(s);
	size = s->Size;
	stream->Write(&size,sizeof(uint16_t));
	s->SaveToStream(stream);
	s->Clear();

	//сохраняем игрики
	outputs->Strings->SaveToStream(s);
	size = s->Size;
	stream->Write(&size,sizeof(uint16_t));
	s->SaveToStream(stream);
	s->Clear();

	//counters
	counters->Strings->SaveToStream(s);
	size = s->Size;
	stream->Write(&size,sizeof(uint16_t));
	s->SaveToStream(stream);
	s->Clear();


	//записываем ревизию
	stream->Write(&ProgRevision,sizeof(uint8_t));
	//сохраняем основную программу
	AnsiString prg =AnsiString(LadGraph->CompileLADProgramm());
	size = prg.Length();
	stream->Write(&size,sizeof(uint16_t));
	stream->Write(prg.c_str(),prg.Length());
	delete s;
	stream->Seek(0,soFromBeginning);
	char * buf = (char*)calloc(sizeof(char), stream->Size+1);
	stream->Read(buf,stream->Size);
	unsigned long CRC = getCRC(buf,stream->Size);
	stream->Seek(0,soFromEnd);
	stream->Write(&CRC, sizeof(unsigned long));
	free(buf);
	delete stream;
	SetModified(false);
}
//---------------------------------------------------------------------------
void TForm1::OpenProject(UnicodeString FileName)
{
TFileStream * stream = new TFileStream(FileName,fmOpenRead);
if(stream==NULL)
	{
	 MessageBox(this->Handle, AnsiString("Something went wrong...\r\nFile not opened").c_str(),AnsiString("Error").c_str(), MB_ICONERROR);
	 return;
	}
char * buf = (char*)calloc(sizeof(char), stream->Size+1);
unsigned long CRC1;
unsigned long CRC;
stream->Read(buf,stream->Size-sizeof(unsigned long));
stream->Read(&CRC1,sizeof(unsigned long));
CRC = getCRC(buf,stream->Size-sizeof(unsigned long));
if(CRC!=CRC1)
	{
	 MessageBox(this->Handle, AnsiString("The project file is corrupt. Checksum violated.\r\nOpening a project is not possible.").c_str(),AnsiString("Error").c_str(), MB_ICONERROR);
	 delete stream;
	 free(buf);
	 return;
	}
free(buf);
NewExecute(this);
uint16_t len=0;
uint16_t count=0;
stream->Seek(0,soFromBeginning);
stream->Read(&len,sizeof(uint16_t));
buf = (char*)calloc(sizeof(char), len+1);
stream->Read(buf,len);
TMemoryStream *s = new TMemoryStream;
s->Write(buf,len);
s->Seek(0,soFromBeginning);
inputs->Strings->LoadFromStream(s);
s->Clear();
free(buf);
//читаем иксы
stream->Read(&count,sizeof(uint16_t));  //количество иксов
if(count!=0)
{
	buf = (char*)calloc(sizeof(char), 256);
	for(int i=0; i<count; i++)
	{
		stream->Read(&len,sizeof(uint16_t));  //количество байтов в иксе
		stream->Read(buf,len);
		buf[len]=0;
		AddVariableExecute(this);
		UnicodeString str =  UnicodeString(buf);
		frame2_array[frame2_count-1]->SetCondition(str);
	}
	free(buf);
}
//читаем Мки
stream->Read(&len,sizeof(uint16_t));  //количество байтов в Мке
buf = (char*)calloc(sizeof(char), len+1);
stream->Read(buf,len);
s->Write(buf,len);
s->Seek(0,soFromBeginning);
MVars->Strings->LoadFromStream(s);
s->Clear();
free(buf);

//читаем Таймеры
stream->Read(&len,sizeof(uint16_t));  //количество байтов в таймерах
buf = (char*)calloc(sizeof(char), len+1);
stream->Read(buf,len);
s->Write(buf,len);
s->Seek(0,soFromBeginning);
Timers->Strings->LoadFromStream(s);
s->Clear();
free(buf);

//читаем игрики
stream->Read(&len,sizeof(uint16_t));  //количество байтов в игриках
buf = (char*)calloc(sizeof(char), len+1);
stream->Read(buf,len);
s->Write(buf,len);
s->Seek(0,soFromBeginning);
outputs->Strings->LoadFromStream(s);
s->Clear();
free(buf);

//timers
stream->Read(&len,sizeof(uint16_t));  //количество байтов в игриках
buf = (char*)calloc(sizeof(char), len+1);
stream->Read(buf,len);
s->Write(buf,len);
s->Seek(0,soFromBeginning);
counters->Strings->LoadFromStream(s);
s->Clear();
free(buf);

//читаем ревизию
stream->Read(&ProgRevision,sizeof(uint8_t));

//читаем основную программу
stream->Read(&len,sizeof(uint16_t));  //количество байтов в игриках
buf = (char*)calloc(sizeof(char), len+1);
stream->Read(buf,len);
LadGraph->BuildLADProgramm(UnicodeString(buf),this,ScrollBox1,(TDrawCellEvent)&StringGrid1DrawCell,StringGrid1Click);
free(buf);
delete s;
delete stream;
LastFilename =  FileName;
RefreshCiolsUsing();
RefreshAllTimers();
this->BuildWindowAttributes("");
}
//---------------------------------------------------------------------------
unsigned long TForm1::getCRC(char *pchBuf, int nBufLen)
{
int i;
int nIndex;
char ch;
unsigned long res;
unsigned long table [] = {
0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,

0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,

0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,

0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,

0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,

0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,

0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,

0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,

0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,

0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,

0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,

0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,

0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,

0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,

0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,

0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};
res = 0xFFFFFFFF;
for (i=0;i<nBufLen+4;i++)
{
nIndex = (res >> 24) & 0xFF;
if (i<nBufLen)
ch = pchBuf[i];
else
ch = 0x0;
res = (res << 8) | ch;
res ^= table[nIndex];
      }
return res;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SaveExecute(TObject *Sender)
{
if(LastFilename!="" && Modified)
{
UnicodeString s = LadGraph->CompileLADProgramm();
if(s=="")
	{
	 Tabs->ActivePageIndex=2;
	 MessageBox(this->Handle, AnsiString("Something went wrong...\r\nOperation not completed").c_str(),AnsiString("Error").c_str(), MB_ICONERROR);
	 return;
	}
  SaveProject(LastFilename);
}
else if(LastFilename=="" && Modified)
{
  Save_asExecute(Sender);
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OpenExecute(TObject *Sender)
{
TestModified();
if(OpenDialog1->Execute(this->Handle))
{
   OpenProject(OpenDialog1->FileName);
   SetModified(false);
   this->BuildWindowAttributes("");
}
}
//---------------------------------------------------------------------------
void TForm1::TestModified()
{
if(!Modified) return;
if(!Unlocked) return;
int res = MessageBox(this->Handle, AnsiString("Do you want to save changes to the program?").c_str(),AnsiString("Confirm please").c_str(), MB_YESNO|MB_ICONQUESTION);
if(res!= IDYES) return;
SaveExecute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
TestModified();
StopRecv=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UnlockExecute(TObject *Sender)
{
	int i;
	if(!GetPrivileges())
	{
		MessageBox(this->Handle, AnsiString("Invalid password. Try again.").c_str(),AnsiString("Error").c_str(), MB_ICONERROR);
		return;
	}
	for(int i=0; i<Actions->ActionCount; i++)
	{
		if(Actions->Actions[i]->Name != "Unlock")
			Actions->Actions[i]->Enabled=true;
		else
			Actions->Actions[i]->Enabled=false;
	}
	Unlocked=true;

	for(i = 0; i < frame2_count; i++)
	{
		frame2_array[i]->SetLock(!Unlocked);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::changepassExecute(TObject *Sender)
{
TForm5 * frm = new TForm5(this);
frm->ShowModal();
if(!frm->Result) return;
UnicodeString s = GetPassHashString(frm->Password);
if(!WritePassHashToReg(s))
	{
	 MessageBox(this->Handle, AnsiString("Oh! Something went wrong.\r\nThe password could not be changed for technical reasons.\r\nPerhaps the required access rights are missing.").c_str(),AnsiString("Error").c_str(), MB_ICONERROR);
	 return;
	}
MessageBox(this->Handle, AnsiString("Password changed successfully. Please remember it. There is no easy way to reset your password.").c_str(),AnsiString("Done").c_str(), MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MVarsKeyPress(TObject *Sender, System::WideChar &Key)
{
if(!this->Unlocked)
{
Key=NULL;
}
else
{
SetModified(true);
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TimersKeyPress(TObject *Sender, System::WideChar &Key)
{
if(!this->Unlocked)
{
Key=NULL;
}
else
{
SetModified(true);
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::outputsKeyPress(TObject *Sender, System::WideChar &Key)
{
if(!this->Unlocked)
{
Key=NULL;
}
else
{
SetModified(true);
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::inputsKeyPress(TObject *Sender, System::WideChar &Key)
{
if(!this->Unlocked)
{
Key=NULL;
}
else
{
SetModified(true);
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox1KeyPress(TObject *Sender, System::WideChar &Key)
{
	if(Key==27)   InputsBox->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::programmExecute(TObject *Sender)
{
//формат
//REV_PUPCOUNT_Xs_PRGLEN_PRG_
//REV_ - 8 бит - ревизия программы.
//PUP - 16 бит. PullUpMask для входов.
//COUNT - количество описанных иксов
//_Xs_ - строка с описанием иксов в формате, _Xs_ может быть равен нулю! Тогда прогамма что-то делает, но на входы не реагирует.
	//XLen_Xstr
	//где XLen - длина описателя  Xstr, Xstr - описатель икса
	//пример одной записи Xstr
	//I01>%30BX01 - читать как if(I01>(VCC*0.3)) X01=true;
	//						   else X01=false;
	//еще пример
	//I01<24BX02 - читать так:
	//if(I01<24) X02=true;
	//else X02=false;
//
//PRGLEN - длина LAD программы
//_PRG_ - сама LAD программа.
//В контроллер в виде прошивки не передаются переменные типа M, I, Y, T так как они должны быть определены в прошивке контроллера
//в том же и в полном количестве, в каком они определны в программе.
//то есть, см. дефайны:
//#define MAXIVAR		5
//#define MAXIOVAR		10
//#define MAXMVAR		15
//#define MAXTVAR		10
//#define MAXCOIL		2
//#define MAXACTIONS	20
	int i;
	int blink_count;
	AnsiString t;
	int16_t size, temp;
	UnicodeString ladprg = LadGraph->CompileLADProgramm();
	if(ladprg=="")
	{
		Tabs->ActivePageIndex=2;
		MessageBox(this->Handle, AnsiString("Something went wrong...\r\nOperation not completed").c_str(),AnsiString("Error").c_str(), MB_ICONERROR);
		return;
	}
	TMemoryStream *prog = new TMemoryStream;
	TMemoryStream *s = new TMemoryStream;
	temp=0;
	//записываем ревизию.
	//prog->Write(&ProgRevision,1);
	//делаем PullUpMask
	//всего в маске 16 бит. Бит0 = вход 0;
	//Бит=0 -> нормельный режим, иначе включить PullUp
	for(i = 0; i < inputs->RowCount-1; i++)
	{
		if(inputs->Cells[0][i+1].Pos("S"))
		{
			t = "L" + inputs->Cells[1][i+1] + "\n" ;
		}
		else if(inputs->Cells[1][i+1]!="Normal")
		{
			temp=temp| (uint16_t)(1<<i);
		}
	}
	prog->Write(t.c_str(), t.Length());
	prog->Write(&temp,sizeof(uint16_t));
	//записываем иксы
	temp=0;  //в нем будет количество иксов
	for(i = 0; i < frame2_count; i++)
	{
		t = AnsiString(frame2_array[i]->Condition_String);
		size = t.Length();
		s->Write(&size,sizeof(uint16_t));
		s->Write(t.c_str(), size);
	}

	if(frame2_count > 0)
	{
		prog->Write(&frame2_count,sizeof(uint16_t));
		s->SaveToStream(prog);
		s->Clear();
	}
	else
	{
		prog->Write(&temp,sizeof(uint16_t));
	}

	//counters

	for(i = 1; i <= MAXCVAR; i++)
	{
		t = AnsiString(counters->Cells[0][i]) + "|";
		if(counters->Cells[1][i] != "")
		{
			t = t + AnsiString(counters->Cells[1][i]) + "|";
		}
		else
		{
            t = t + "0|";
        }

		size = t.Length();
		s->Write(&size,sizeof(uint16_t));
		s->Write(t.c_str(), size);
	}

	temp = MAXCVAR;
	if(frame2_count > 0)
	{
		prog->Write(&temp,sizeof(uint16_t));
		s->SaveToStream(prog);
		s->Clear();
	}
	else
	{
		prog->Write(&temp,sizeof(uint16_t));
	}

	t=AnsiString(ladprg);
	size = t.Length();
	prog->Write(&size,sizeof(uint16_t));
	prog->Write(t.c_str(),size);
	delete s;
	TIniFile * ini = new TIniFile(ExtractFileDir(Application->ExeName)+"\\config.ini");
	if(ini->ReadInteger("options","SaveCompilled",0)>0)
		try
		{
		prog->SaveToFile(ExtractFileDir(Application->ExeName)+"\\Compilled.prg");
		}
		catch(...) {}
	delete ini;
	prog->Seek(0,soFromBeginning);
	SendToPLC(prog);
}
//---------------------------------------------------------------------------
DWORD WINAPI RecvStream(LPVOID c)
{
TForm1 * form = (TForm1*)c;
AnsiString Message;
while(1)
	{
		if(form->StopRecv)
		{
			if(form->ComPort!=NULL) form->ComPort->ClosePort();
			delete form->ComPort;
			form->ComPort=NULL;
			return 0;
		}
		Message = form->ComPort->PortRead();
		if(Message!="")
			{
			   if(form->ComPort!=NULL) form->OnPortRecv(Message);
			}
		Message="";
		if(form->ComPort==NULL) return 0;
		Sleep(50);
	}
}
//---------------------------------------------------------------------------
bool TForm1::MyReadWrite(TComPort * port, AnsiString Message, AnsiString &Answer)
{
int shots;	//количество неудачных попыток
for(shots = 0; shots<MAXSHOTS;shots++)
//while(1)
{
	if(port->ReadWrite(Message,Answer))
		{
		  if(Answer==OK_ANSWER)
		  return true;
		  else if(Answer==BUSY_ANSWER)
		  {
			shots=0;
			Sleep(20);
          }
		  else Sleep(20);
		}
	Application->ProcessMessages();
}
return false;
}
//---------------------------------------------------------------------------
AnsiString TForm1::ConvertBufToHEXBuf(byte * buf, int len)
{
	AnsiString s;
	for(int i=0; i<len; i++)
	{
		s=s+AnsiString().sprintf("%02X",(int)buf[i]);
	}
	return s;
}
//---------------------------------------------------------------------------
void TForm1::OnPortRecv(AnsiString Answer)
{
	int len;
	AnsiString temp;
	progtimeout->Enabled=false;
	if(Answer==OK_ANSWER)
	{
		if(LastMSG==ENTER_PROGRAMMING_MODE)
		{
		temp=ConvertBufToHEXBuf(&ProgRevision,1);
		LastMSG="!"+temp+"#";
		len=LastMSG.Length();
		len=ComPort->PortWrite(LastMSG);
		return;
		}
		else if(LastMSG==CLOSE_PROGRAMMING_MODE)
		{
		   StopRecv=true;
		   progr->Visible=false;
		   MessageBox(this->Handle, AnsiString("Programming completed successfully.").c_str(),AnsiString("Congratulations").c_str(), MB_ICONINFORMATION);
		   delete ProgStream;
		   ProgStream=NULL;
		   return;
		}
		else if(LastMSG.Pos("$"))
		{
			count++;
		}
	}
	else if(Answer==BUSY_ANSWER || Answer==REPEAT_ANSWER)
	{
	  ComPort->PortWrite(LastMSG);
	  progtimeout->Enabled=true;
	  return;
	}
	else //if(Answer==ERR_ANSWER)
	{
		   StopRecv=true;
		   progr->Visible=false;
		   MessageBox(this->Handle, AnsiString("Unknown error while programming the PLC. Operation aborted.").c_str(),AnsiString("Error").c_str(), MB_ICONERROR);
		   delete ProgStream;
		   ProgStream=NULL;
		   return;
	}
	BYTE buf[MAX_PROG_BUF+1];
	len = ProgStream->Read(buf, MAX_PROG_BUF);
	if(len==0)
	{
	//выводим из режима программирования
	LastMSG = CLOSE_PROGRAMMING_MODE;
	ComPort->PortWrite(LastMSG);
	progtimeout->Enabled=true;
	return;
	}
	progr->progress->Position=progr->progress->Position+len;
	buf[len]=0;
	temp=ConvertBufToHEXBuf(buf,len);
	LastMSG="$"+AnsiString().sprintf("%03d",count)+AnsiString().sprintf("%03d",temp.Length())+":"+temp+"#";
	len=LastMSG.Length();
	len=ComPort->PortWrite(LastMSG);
	progtimeout->Enabled=true;
}
//---------------------------------------------------------------------------
bool TForm1::SendToPLC(TMemoryStream * stream)
{
TComPort * port = new TComPort();
TStringList *ports = new TStringList;
port->EnumPortsA(ports);
if(ports->Text=="")
	{
MessageBox(this->Handle, AnsiString("No connection to the PLC detected. Operation aborted.").c_str(),AnsiString("Error").c_str(), MB_ICONERROR);
delete port;
return false;
	}
bool isFinded=false;
AnsiString Answer, Msg;
for(int i=1; i<ports->Count; i++)
{
	if(port->OpenPort(ports->operator [](i),CBR_115200,8,NOPARITY,ONESTOPBIT,1000))
	{
	  Msg = "V";
	  Answer = "";
	  if(port->ReadWrite(Msg,Answer))
		{
		 Answer=Answer.UpperCase();
		 if(Answer.Pos("V"))
			{
			  isFinded=true;
			  StatusBar1->Panels->operator [](0)->Text="PLC Port: "+ports->operator [](i);
			  break;
			}
		 else port->ClosePort();
		}
	}
}
delete ports;
if(!isFinded)
{
MessageBox(this->Handle, AnsiString("No connection to the PLC detected. Operation aborted.").c_str(),AnsiString("Error").c_str(), MB_ICONERROR);
port->ClosePort();
delete port;
return false;
}
ComPort = port;
StopRecv=false;
ProgStream = stream;
count=0;
CreateThread(NULL,0,RecvStream, (void*)this,0,NULL);
Msg = ENTER_PROGRAMMING_MODE;
LastMSG=Msg;
progr->progress->Position=0;
progr->progress->Max=stream->Size;
progr->Show();
Application->ProcessMessages();
port->PortWrite(Msg);
return true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight,
		  bool &Resize)
{
if(!PosWrite) return;
TIniFile * ini = new TIniFile(ExtractFileDir(Application->ExeName)+"\\config.ini");
if(this->WindowState==wsNormal)
{
ini->WriteInteger("Position","Top",this->Top);
ini->WriteInteger("Position","Left",this->Left);
ini->WriteInteger("Position","Width",NewWidth);
ini->WriteInteger("Position","Height",NewHeight);
ini->WriteInteger("Position","WindowState",0);
}
else if(this->WindowState==wsMaximized)
{
ini->WriteInteger("Position","Top",this->Top);
ini->WriteInteger("Position","Left",this->Left);
ini->WriteInteger("Position","WindowState",1);
}
delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::progtimeoutTimer(TObject *Sender)
{
	   progtimeout->Enabled=false;
	   StopRecv=true;
	   progr->Visible=false;
	   MessageBox(this->Handle, AnsiString("PLC is not responding. Operation aborted.").c_str(),AnsiString("Error").c_str(), MB_ICONERROR);
	   if(ProgStream!=NULL) delete ProgStream;
	   ProgStream=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RevisionExecute(TObject *Sender)
{
TForm7 * form=new TForm7(this);
form->rev->Value = ProgRevision;
form->ShowModal();
if(form->Result)
{
if(ProgRevision!=(uint8_t)form->rev->Value)
	{
	ProgRevision=(uint8_t)form->rev->Value;
	this->SetModified(true);
	this->BuildWindowAttributes("");
	}
}
delete form;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DeleteVariableExecute(TObject *Sender)
{
	UnicodeString temp_name, curr_name, prev_var, var;
	TFrame2 *frame_to_del;
	int top, left, prev_top, prev_left;
	int index_to_del;
	int i;
	if(this->frame2_count == 0)
	{
		return;
	}

	TForm8 * form = new TForm8(this);
	for(i = 0; i < this->frame2_count; i++)
	{
		form->ComboBox1->AddItem(frame2_array[i]->Var, NULL);
	}
	form->ShowModal();
	index_to_del = form->selected_item;

	if(index_to_del == -1)
	{
        return;
    }

	frame_to_del = this->frame2_array[index_to_del];

	for(i = index_to_del; i < (frame2_count-1); i++)
	{
		frame2_array[i] = frame2_array[i+1];
	}
	frame2_array[i] = NULL;

	temp_name = frame_to_del->Name;
	frame_to_del->Name = "";
	prev_top = frame_to_del->Top;
	prev_left = frame_to_del->Left;
	prev_var = frame_to_del->Var;


	for(i = index_to_del ; i < (frame2_count -1); i++)
	{
		top = frame2_array[i]->Top;
		left = frame2_array[i]->Left;
		var = frame2_array[i]->Var;
		curr_name = frame2_array[i]->Name;

		frame2_array[i]->Top = prev_top;
		frame2_array[i]->Left = prev_left;
		frame2_array[i]->Var = prev_var;
		frame2_array[i]->Name = temp_name;
        frame2_array[i]->VarIndex = i;
		frame2_array[i]->BuildCondition();

		temp_name = curr_name;
		prev_top = top;
		prev_left = left;
        prev_var = var;
	}

	delete frame_to_del;
	frame2_count--;

    LadGraph->UpdateConditionNames(index_to_del);

	return;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::countersKeyPress(TObject *Sender, System::WideChar &Key)
{
	if(!this->Unlocked)
	{
		Key=NULL;
	}
	else
	{
		SetModified(true);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Action2Execute(TObject *Sender)
{
	//increment    c
	TForm9 * frm;
	int i;

	if(LadGraph->GetCurrentGrid()==NULL)
		return;

	frm = new TForm9(this);
	frm->label_reset->Visible = false;
	frm->input_reset->Visible = false;
	frm->label_reset->Caption = "Period(ms) :";


	frm->counterDDL->Clear();
	for(i = 1; i <= MAXCVAR; i++)
	{
		frm->counterDDL->AddItem(counters->Cells[0][i], NULL);
	}
	frm->Caption = frm->Caption + "increment";
	frm->ShowModal();

	if("" != frm->result)
	{
		LadGraph->Add_CINC(LadGraph->GetCurrentGrid(),frm->result, frm->reset_val);
		SetModified(true);
	}

	/*
	if(frm->Result)
	{
		LadGraph->Add_SET_Coil(LadGraph->GetCurrentGrid(),frm->Var->Text);
		SetModified(true);
	} */
	delete frm;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Action3Execute(TObject *Sender)
{
	//decrement     c
		TForm9 * frm;
	int i;

	if(LadGraph->GetCurrentGrid()==NULL)
		return;

	frm = new TForm9(this);
	frm->label_reset->Visible = false;
	frm->input_reset->Visible = false;
	frm->label_reset->Caption = "Period(ms) :";

	frm->counterDDL->Clear();
	for(i = 1; i <= MAXCVAR; i++)
	{
		frm->counterDDL->AddItem(counters->Cells[0][i], NULL);
	}
	frm->Caption = frm->Caption + "decrement";
	frm->ShowModal();

	if("" != frm->result)
	{
		LadGraph->Add_CDEC(LadGraph->GetCurrentGrid(),frm->result, frm->reset_val);
		SetModified(true);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Action4Execute(TObject *Sender)
{
	//reset
		TForm9 * frm;
	int i;

	if(LadGraph->GetCurrentGrid()==NULL)
		return;

	frm = new TForm9(this);
	frm->chkWithTimer->Visible = false;

	frm->counterDDL->Clear();
	for(i = 1; i <= MAXCVAR; i++)
	{
		frm->counterDDL->AddItem(counters->Cells[0][i], NULL);
	}
	frm->Caption = frm->Caption + "reset";
	frm->ShowModal();

	if("" != frm->result)
	{
		LadGraph->Add_CRES(LadGraph->GetCurrentGrid(),frm->result, frm->reset_val);
		SetModified(true);
	}
}
//---------------------------------------------------------------------------

