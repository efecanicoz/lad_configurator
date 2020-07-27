//---------------------------------------------------------------------------

#pragma hdrstop

#include "RS232implements.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
bool TComPort::EnumPorts(TStringList *List)
{
    if(List==NULL) return false;
	List->Clear();
	TCHAR szDevices[65535 * 2];
	DWORD dwChars = QueryDosDevice(NULL, szDevices, (65535 * 2));
	if(dwChars)
		{
		 int i=0;
		 while(1)
		 {
		   // Получаем текущее имя устройства
		   TCHAR* pszCurrentDevice = &szDevices[i];
		   // Если похоже на "COMX" выводим на экран
		   int nLen = _tcslen(pszCurrentDevice);

		   if(nLen > 3 && _tcsnicmp(pszCurrentDevice, _T("COM"), 3) == 0)
				{
				List->Add(UnicodeString(pszCurrentDevice));
				}
		   // Переходим к следующему символу терминатору

		   while(szDevices[i] != _T('\0')) i++;

		   // Перескакиваем на следующую строку
		   i++;
		   // Список завершается двойным симмволом терминатором, так что если символ
		   // NULL, мы дошли до конца
		   if(szDevices[i] == _T('\0'))
		   {
				if(List->Text!="")
					return true;
				else
					return false;
		   }
		 }
        }
return false;
}
//---------------------------------------------------------------------------
bool TComPort::IsOpened()
{
	if(porth!=NULL && porth!=INVALID_HANDLE_VALUE) return true;
	return false;
}
//---------------------------------------------------------------------------
TComPort::TComPort()
{
porth=NULL;
}
//---------------------------------------------------------------------------
TComPort::~TComPort()
{
ClosePort();
}
//---------------------------------------------------------------------------
void TComPort::ClosePort()
{
if(!IsOpened()) return;
CloseHandle(porth);
Application->ProcessMessages();
porth=NULL;
}
//---------------------------------------------------------------------------
bool TComPort::OpenPort(UnicodeString PortName, //like COMx
					int CBR,				//BaudRate, ilke CBR_19200, CBR_115200, CBR_256000
					int Bytes,				//7,8
					int Parity,				//NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY
					int StopBits,			//ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS
					int RWTimeout			//100
				  )
{
ClosePort();

DCB ComDCB;
COMMTIMEOUTS ComTimes;
UnicodeString Msg = "\\\\.\\" + PortName.UpperCase();

porth = CreateFile(AnsiString(Msg).c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
if (porth == INVALID_HANDLE_VALUE)
  {
   ClosePort();
   return false;
  }
/*ZeroMemory(&ComDCB, sizeof(ComDCB));
ComDCB.DCBlength = sizeof(ComDCB);
if (!GetCommState(porth, &ComDCB))
  {
	ClosePort();
	return false;
  }
ComDCB.BaudRate = CBR;
ComDCB.ByteSize = Bytes;
ComDCB.Parity   = Parity;
ComDCB.StopBits = StopBits;
ComDCB.fBinary  = 1;
ComDCB.fParity  = Parity;//(ComDCB.Parity != NOPARITY) ? 1 : 0;
ComDCB.fOutxCtsFlow = 0;
ComDCB.fOutxCtsFlow = false;
ComDCB.fOutxDsrFlow = false;
ComDCB.fDtrControl = DTR_CONTROL_DISABLE;
ComDCB.fDsrSensitivity = false;
ComDCB.fNull = false;
ComDCB.fRtsControl = RTS_CONTROL_DISABLE;
ComDCB.fOutX = 0;
ComDCB.fInX = 0;
ComDCB.fAbortOnError = false;
if (!SetCommState(porth, &ComDCB))
  {
   ClosePort();
   return false;
  }  */
if (!GetCommTimeouts(porth, &ComTimes))
  {
   ClosePort();
   return false;
  }
ComTimes.ReadIntervalTimeout         = MAXDWORD;
ComTimes.ReadTotalTimeoutMultiplier  = 0;
ComTimes.ReadTotalTimeoutConstant    = RWTimeout;
ComTimes.WriteTotalTimeoutMultiplier = 0;
ComTimes.WriteTotalTimeoutConstant   = 0;//RWTimeout;
if (!SetCommTimeouts(porth, &ComTimes))
  {
   ClosePort();
   return false;
  }
SetupComm(porth, 1024, 1024);
PurgeComm(porth, PURGE_RXCLEAR | PURGE_TXCLEAR);
return true;
}
//---------------------------------------------------------------------------
unsigned long TComPort::PortWrite(AnsiString Message)
{
if(!IsOpened()) return 0;
unsigned long len=0;
WriteFile(porth, Message.c_str(), Message.Length(), &len, NULL);
return len;
}
//---------------------------------------------------------------------------
AnsiString TComPort::PortRead()
{
if(!IsOpened()) return "";
BYTE s[MAX_PATH];
unsigned long len;
ReadFile(porth, &s, MAX_PATH, &len, NULL);
if(len==0) return "";
s[len]=0;
AnsiString buf = AnsiString((char*)s);
return buf;
}
//---------------------------------------------------------------------------
bool TComPort::ReadWrite(AnsiString Message, AnsiString &Answer)
{
int len =PortWrite(Message);
if(len==0) return false;
Answer = PortRead();
if(Answer=="") return false;
return  true;
}
//---------------------------------------------------------------------------

