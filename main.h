#pragma once
__declspec(selectany) MSG Msg;
__declspec(selectany) HANDLE readingThrd;
__declspec(selectany) DWORD threadid;
DWORD WINAPI ThreadFunc(LPVOID);