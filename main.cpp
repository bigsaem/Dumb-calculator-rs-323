/*-----------------------------------------------------------------------------------------------------------
-- SOURCE FILE: main.c - An application that runs the serial port program and manage threads.
-- 
--
-- PROGRAM: Dumb terminal
--
-- FUNCTIONS:
-- int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance,
-- DWORD WINAPI ThreadFunc(LPVOID threadHandle)
--
--
-- DATE: October 01, 2019
--
-- REVISIONS: None
--
-- DESIGNER: Keunsaem Lee
--
-- PROGRAMMER: Keunsaem Lee
--
-- NOTES:
-- This file runs the dumb terminal program. It monitors messages passed to windows and dispatch it 
-- to WndProc function. It also manages threads for reading intput stream from serial cable.
-----------------------------------------------------------------------------------------------------------*/

#include <windows.h>
#include <stdio.h>
#include "main.h"
#include "physical.h"
#include "session.h"
#include "application.h"

/*-----------------------------------------------------------------------------------------------------------
-- FUNCTION: WinMain
--
-- DATE: Oct 01, 2019
--
-- REVISIONS: None
--
-- DESIGNER: Keunsaem Lee
--
-- PROGRAMMER: Keunsaem Lee
--
-- INTERFACE: int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspszCmdParam, int nCmdShow)
--
-- RETURNS: Msg.wParam;
--
-- NOTES:
-- This function is used to initialize the program and and necessary functions for setting up the application.
-- It also keeps monitoring messages and dispatch messages to Wnd
------------------------------------------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance,
	LPSTR lspszCmdParam, int nCmdShow)
{
	ConfigWindow(Wcl, hInst, hwnd, nCmdShow);
	ConfigPort(hComm, dcb, timeout, "com1");

	readingThrd = CreateThread(NULL, 0, ThreadFunc, hwnd, 0, &threadid);

	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

/*--------------------------------------------------------------------------------------------------
-- FUNCTION: ThreadFunc
--
-- DATE: Oct 01, 2019
--
-- REVISIONS: None
--
-- DESIGNER: Keunsaem Lee
--
-- PROGRAMMER: Keunsaem Lee
--
-- INTERFACE: DWORD WINAPI ThreadFunc(LPVOID threadHandle)
--
-- RETURNS: DWORD 0;
--
-- NOTES:
-- This function is used to run an additional thread to monitor input stream from serial port.
-----------------------------------------------------------------------------------------------------*/
DWORD WINAPI ThreadFunc(LPVOID threadHandle)
{
	ReadChar(threadHandle);
	return 0;
}

