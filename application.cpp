/*----------------------------------------------------------------------------------------------------------
--SOURCE FILE : application.cpp - An application that configures the window and handle sent messages. It also
--has the responsibility to display information on the window. 
--
--PROGRAM : Dumb terminal
--
--FUNCTIONS :
--BOOLEAN ConfigWindow(WNDCLASSEX wcl, HINSTANCE hInst, HWND &hwnd, int nCmdShow)
--LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
--BOOLEAN DisplayChar(HWND hwnd, HDC hdc, char* readBuffer, unsigned* xCoor)
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
--This file configures the window to start the program and set the menubar for it.
--It also accepts window messages and process it complying with the type of it. 
--It displays typed characters on the window as well.
-----------------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include "application.h"
#include "main.h"
#include "session.h"
#include "physical.h"
/*-----------------------------------------------------------------------------------------------------------
-- FUNCTION: ConfigWindow
--
-- DATE: Oct 01, 2019
--
-- REVISIONS: None
--
-- DESIGNER: Keunsaem Lee
--
-- PROGRAMMER: Keunsaem Lee
--
-- INTERFACE: BOOLEAN ConfigWindow(WNDCLASSEX wcl, HINSTANCE hInst, HWND &hwnd, int nCmdShow)
--
-- RETURNS: BOOLEAN, 1 (to check if it successfully passes data to serial port)
--
-- NOTES:
-- This function configure windows with options such as size, icon, cursor and name of the window.
-- It also updates the client area of the window.
-----------------------------------------------------------------------------------------------------------*/
BOOLEAN ConfigWindow(WNDCLASSEX wcl, HINSTANCE hInst, HWND &hwnd, int nCmdShow) {

	char Name[] = "Dumb Terminal";
	Wcl.cbSize = sizeof(WNDCLASSEX);
	Wcl.style = CS_HREDRAW | CS_VREDRAW;
	Wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION); // large icon 
	Wcl.hIconSm = NULL; // use small version of large icon
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);  // cursor style

	Wcl.lpfnWndProc = WndProc;
	Wcl.hInstance = hInst;
	Wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //white background
	Wcl.lpszClassName = Name;
	Wcl.lpszMenuName = "MYMENU"; // The menu Class
	Wcl.cbClsExtra = 0;      // no extra memory needed
	Wcl.cbWndExtra = 0;

	if (!RegisterClassEx(&Wcl))
		return 0;

	hwnd = CreateWindow(Name, Name, WS_OVERLAPPEDWINDOW, 10, 10,
		600, 400, NULL, NULL, hInst, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return 1;
}

/*-----------------------------------------------------------------------------------------------------------
-- FUNCTION: WndProc
--
-- DATE: Oct 01, 2019
--
-- REVISIONS: None
--
-- DESIGNER: Keunsaem Lee
--
-- PROGRAMMER: Keunsaem Lee
--
-- INTERFACE: LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
--
-- RETURNS: LRESULT, 0
--
-- NOTES:
-- This function accepts window messages and handles it based on the type of it.
-- It allows user to interact with the menu and handle escape character to call command mode.
-- It passes displayable characters to writeChar function to send it through serial cable.
-----------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	DWORD writtenBytes;
	char op;
	PAINTSTRUCT ps;            // required by BeginPaint 

	BOOLEAN commandMode = false;

	switch (Message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_COM1:
			SuspendThread(readingThrd);
			commandMode = true;
			ConfigPort(hComm, dcb, timeout, "com1");
			commandMode = false;
			ResumeThread(readingThrd);
			break;

		case IDM_COM2:
			SuspendThread(readingThrd);
			commandMode = true;
			ConfigPort(hComm, dcb, timeout, "com2");
			commandMode = false;
			ResumeThread(readingThrd);
			break;
		case IDM_HELP:
			MessageBox(hwnd, "Set up a port in command mode and send data from connect mode.", "", MB_OK);
			break;
		case IDM_EXIT:
			if (hComm != NULL) {
				CloseHandle(hComm);
			}
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_CHAR:	// Process keystroke
		switch (wParam) {
		case 0x1B:
			if (!commandMode) {
				hmenu = GetMenu(hwnd);
				for (int i = 100; i < 107; i++) {
					if (i != 101 && i != 102 && i != 103) {
						EnableMenuItem(hmenu, i, MF_DISABLED);
					}
				}
				MessageBox(hwnd, "You entered COMMAND MODE, please select port and configure it", "", MB_OK);
			}
			break;

		default:
			sprintf_s(writeChar, "%c", (char)wParam);
			/*DisplayChar(hwnd, hdc, writeChar, &xCoor);*/
			WriteChar(hComm, writeChar, &writeByte);
			break;
		}
		break;

	case WM_PAINT:		// Process a repaint message
		hdc = BeginPaint(hwnd, &ps); // Acquire DC
		TextOut(hdc, 0, 0, readBuffer, strlen(readBuffer)); // output character
		EndPaint(hwnd, &ps); // Release DC
		break;

	case WM_DESTROY:	// Terminate program
		if (hComm != NULL) {
			CloseHandle(hComm);
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}
/*-----------------------------------------------------------------------------------------------------------
-- FUNCTION: DisplayChar
--
-- DATE: Oct 01, 2019
--
-- REVISIONS: None
--
-- DESIGNER: Keunsaem Lee
--
-- PROGRAMMER: Keunsaem Lee
--
-- INTERFACE: BOOLEAN DisplayChar(HWND hwnd, HDC hdc, char* readBuffer, unsigned* xCoor)
--
-- RETURNS: BOOLEAN, 1
--
-- NOTES:
-- This function accepts the window handle, device context handle, readbuffer and X coordinate to display
-- passed characters it moves the cursor wherer it displays the character by 10 to X axis way.
-----------------------------------------------------------------------------------------------------------*/
BOOLEAN DisplayChar(HWND hwnd, HDC hdc, char* readBuffer, unsigned* xCoor) {
	hdc = GetDC(hwnd);
	TextOut(hdc, 10 * (*xCoor), 0, readBuffer, strlen(readBuffer)); // output character
	(*xCoor)++;
	ReleaseDC(hwnd, hdc);
	return 0;
}

