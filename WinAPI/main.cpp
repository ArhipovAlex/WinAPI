#include<Windows.h>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	MessageBox(NULL, "HelloWorld!", "Info", MB_YESNOCANCEL| MB_ICONINFORMATION);
	//const char* - ANSI/ASCII string
	//LPCWSTR - Long Pointer to Wide String (строка в кодировке Unicode (wchar_t))

	return 0;
}