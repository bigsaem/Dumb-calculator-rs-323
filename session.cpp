/*----------------------------------------------------------------------------------------------------------
--SOURCE FILE : session.cpp - An application that configure and open a port to run the program and allow 
--communication.
--
--PROGRAM : Dumb terminal
--
--FUNCTIONS :
--BOOLEAN ConfigPort(HANDLE &hComm, DCB &dcb, COMMTIMEOUTS timeout, const char* portname)
--
--DATE: October 01, 2019
--
--REVISIONS : None
--
--DESIGNER : Keunsaem Lee
--
--PROGRAMMER : Keunsaem Lee
--
--NOTES :
--This file propts user to configure port settings and initializes it based on the settings.
-----------------------------------------------------------------------------------------------------------*/
#include "session.h"
#include "application.h"
/*-----------------------------------------------------------------------------------------------------------
-- FUNCTION: ConfigPort
--
-- DATE: Oct 01, 2019
--
-- REVISIONS: None
--
-- DESIGNER: Keunsaem Lee
--
-- PROGRAMMER: Keunsaem Lee
--
-- INTERFACE: BOOLEAN ConfigPort(HANDLE &hComm, DCB &dcb, COMMTIMEOUTS timeout, const char* portname)
--
-- RETURNS: BOOLEAN, 1 (to check if it successfully passes data to serial port)
--
-- NOTES:
-- This function prompts user to configure port through CommConfigDialog function and stores the configuration
-- data in DCB structure, and initialize the port with the information of it. It also sets timeout to prevent
-- it from taking the control of the port too long time.
-----------------------------------------------------------------------------------------------------------*/
BOOLEAN ConfigPort(HANDLE &hComm, DCB &dcb, COMMTIMEOUTS timeout, const char* portname) {
	cc.dwSize = sizeof(COMMCONFIG);
	cc.wVersion = 0x100;
	lpszCommName = portname;
	GetCommConfig(hComm, &cc, &cc.dwSize);

	if (hComm != NULL) 
		CloseHandle(hComm);
	
	if (!CommConfigDialog(lpszCommName, hwnd, &cc))
		MessageBox(NULL, "Error setting commconfig", "", MB_OK);
	
	if ((hComm = CreateFile(lpszCommName, GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL))
		== INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "Error opening COM port:", "", MB_OK);
	}
	dcb = cc.dcb;

	if (!SetCommState(hComm, &dcb))
	{
		MessageBox(NULL, "Error configuring COM port:", "", MB_OK);
	}

	//timeout
	timeout.ReadIntervalTimeout = 1;
	timeout.ReadTotalTimeoutConstant = 1;
	timeout.ReadTotalTimeoutMultiplier = 1;
	timeout.WriteTotalTimeoutConstant = 1;
	timeout.WriteTotalTimeoutMultiplier = 1;

	if (!SetCommTimeouts(hComm, &timeout)) {
		MessageBox(NULL, "Error timeout:", "", MB_OK);
	}
	for (int i = 100; i < 107; i++) {
		EnableMenuItem(hmenu, i, MF_ENABLED);
	}
	return 1;
}
