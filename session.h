#pragma once
#include <Windows.h>

__declspec(selectany) HANDLE hComm;
__declspec(selectany) DCB dcb;
__declspec(selectany) COMMTIMEOUTS timeout;
__declspec(selectany) LPCSTR lpszCommName = "com1";
__declspec(selectany) COMMCONFIG	cc;
BOOLEAN ConfigPort(HANDLE &, DCB &, COMMTIMEOUTS, const char*);