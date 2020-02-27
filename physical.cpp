/*----------------------------------------------------------------------------------------------------------
--SOURCE FILE : physical.cpp - An application that reads and writes stream from and to the serial port
--
--
--PROGRAM : Dumb terminal
--
--FUNCTIONS :
--BOOLEAN WriteChar(HANDLE &hComm, char* WriteChar, LPDWORD writeByte)
--BOOLEAN ReadChar(LPVOID threadHandle)
--
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
--This file reads input stream from serial port and call display function to display it on the window.
--It also writes output stream to send characters to another computer through serial port.
-----------------------------------------------------------------------------------------------------------*/
#include "physical.h"
#include "application.h"
#include "session.h"
/*------------------------------------------------------------------------------------------------------------
-- FUNCTION: WriteChar
--
-- DATE: Oct 01, 2019
--
-- REVISIONS: None
--
-- DESIGNER: Keunsaem Lee
--
-- PROGRAMMER: Keunsaem Lee
--
-- INTERFACE: BOOLEAN WriteChar(HANDLE &hComm, char* WriteChar, LPDWORD writeByte)
--
-- RETURNS: BOOLEAN, 1 (to check if it successfully passes data to serial port)
--
-- NOTES:
-- This function is used to transmit character data to another computer through serial port. It aceepts
-- serial port handle and send a character from character buffer, it records the the number of sents bytes.
------------------------------------------------------------------------------------------------------------*/
BOOLEAN WriteChar(HANDLE &hComm, char* WriteChar, LPDWORD writeByte) {
	if (!WriteFile(hComm, WriteChar, 1, writeByte, NULL))
	{
		MessageBox(NULL, "Error writing ", "", MB_OK);
	}
	return 1;
}
/*------------------------------------------------------------------------------------------------------------
-- FUNCTION: ReadChar
--
-- DATE: Oct 01, 2019
--
-- REVISIONS: None
--
-- DESIGNER: Keunsaem Lee
--
-- PROGRAMMER: Keunsaem Lee
--
-- INTERFACE: BOOLEAN WriteChar(HANDLE &hComm, char* WriteChar, LPDWORD writeByte)
--
-- RETURNS: BOOLEAN, 1 (to check if it successfully passes data to serial port)
--
-- NOTES:
-- This function is used to read character data from another computer through serial port. It aceepts
-- serial port handle and store a passed character and call display function to show it on window.
-- It kept running on an extra thread to monitor inputstream from the cable.
------------------------------------------------------------------------------------------------------------*/
BOOLEAN ReadChar(LPVOID threadHandle) {
	for (;;) {
		if (ReadFile(hComm, readBuffer, 1, &readBytes, NULL))
		{
			if (readBytes > 0 && readBuffer != NULL) {
				DisplayChar((HWND)threadHandle, hdc, readBuffer, &xCoor);
			}
		}
	}
	return 1;
}