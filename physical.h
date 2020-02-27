#pragma once
#include <Windows.h>
__declspec(selectany) char writeChar[10]="a";
__declspec(selectany) DWORD writeByte = 0;
BOOLEAN WriteChar(HANDLE &, char*, LPDWORD);
BOOLEAN ReadChar(LPVOID);