#pragma once
#define IDM_CONNECT			100
#define IDM_COMMAND			101
#define IDM_COM1			102
#define IDM_COM2			103
#define IDM_HELP			104
#define IDM_EXIT			105
#define IDM_ABOUT			106

__declspec(selectany) WNDCLASSEX Wcl;
__declspec(selectany) HWND hwnd;
__declspec(selectany) HMENU hmenu;
__declspec(selectany) HDC hdc;
__declspec(selectany) char readBuffer[10] = "";
__declspec(selectany) unsigned xCoor = 0;
__declspec(selectany) DWORD readBytes;
BOOLEAN ConfigWindow(WNDCLASSEX, HINSTANCE, HWND &, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOLEAN commandMode(COMMCONFIG, BOOLEAN*, const char*);
BOOLEAN DisplayChar(HWND, HDC, char*, unsigned*);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
