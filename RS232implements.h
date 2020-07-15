//---------------------------------------------------------------------------

#ifndef RS232implementsH
#define RS232implementsH
#include <vcl.h>
#include <stdio.h>
#include <windows.h>
#include <TCHAR.H >
#include <conio.h>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <SyncObjs.hpp>
#include <winbase.h>
//---------------------------------------------------------------------------

class TComPort
{
	private:
	HANDLE porth;

	public:
	TComPort();
    ~TComPort();
	bool EnumPorts(TStringList *List);
	bool IsOpened();
	void ClosePort();
	bool OpenPort(UnicodeString PortName, 	//like COMx
					int CBR,				//BaudRate, ilke CBR_19200, CBR_115200, CBR_256000
					int Bytes,				//7,8
					int Parity,				//NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY
					int StopBits,			//ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS
					int RWTimeout			//100
				  );
	unsigned long PortWrite(AnsiString Message);
	AnsiString PortRead();
	bool ReadWrite(AnsiString Message, AnsiString &Answer);
};

















#endif

