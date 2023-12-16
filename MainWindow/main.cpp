#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<wingdi.h>
#include<stdio.h>
#include"resource.h"

CONST CHAR g_sz_WINDOW_CLASS[] = "My first Window";

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса окна:
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	//cb - Count Bytes отвечают за размеры окна
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = 0;
	//внешний вид окна
	//wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	//wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hIcon = (HICON)LoadImage(hInstance, "icon2.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);//на панели задач
	wc.hIconSm = (HICON)LoadImage(hInstance, "icon1.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);//в строке заголовка окна
	wc.hCursor = (HCURSOR)LoadImage(hInstance, "Flame.ani", IMAGE_CURSOR, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	//
	wc.hInstance = hInstance;
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.lpszClassName = g_sz_WINDOW_CLASS;
	//регистрируем
	if (RegisterClassEx(&wc) == NULL)
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
	}
	//HDC hDCScreen = GetDC(NULL);
	//int Horres = GetDeviceCaps(hDCScreen, HORZRES);
	//int Vertres = GetDeviceCaps(hDCScreen, VERTRES);

	//2) Создание окна:
	INT screen_width = GetSystemMetrics(SM_CXSCREEN);
	INT screen_height = GetSystemMetrics(SM_CYSCREEN);
	INT window_width = screen_width * 3/4;
	INT window_height = screen_height * 3 / 4;
	INT window_start_x = screen_width / 8;
	INT window_start_y = screen_height / 8;
	//CHAR TitleWindow[] = "";
	//sprintf(TitleWindow, "Size: %ix%i Location: %i;%i", window_width, window_height, window_start_x, window_start_y);
	HWND hwnd = CreateWindowExA
	(
		NULL,//ExStyles
		g_sz_WINDOW_CLASS,//Class name
		"TitleWindow",//Заголовок окна
		WS_OVERLAPPEDWINDOW,//Стиль для главного окна:
							//наличие строки заголовка, кнопки управления 
							//и масштабирование размеров
		//CW_USEDEFAULT, CW_USEDEFAULT,//положение окна на экране
		window_start_x,window_start_y,
		//CW_USEDEFAULT, CW_USEDEFAULT,//размер окна
		window_width, window_height,		
		NULL,	//родительское окно (у главного нет)
		NULL,	//для главного окна задает меню,
				//для дочернего окна ID-ресурса дочернего окна (IDC_BUTTON_COPY, IDC_EDIT...)
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//3) Запуск цикла сообщений:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_SIZE:
	case WM_MOVE:
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		CONST INT SIZE = 256;
		CHAR sz_title[SIZE]{};
		sprintf(sz_title, "%s Position:%ix%i, Size: %ix%i", g_sz_WINDOW_CLASS, 
			rect.left, rect.top,
			rect.right-rect.left,rect.bottom-rect.top);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_title);
	}	
		break;
	case WM_PAINT:
		break;
	case WM_COMMAND:
		break;
	case WM_DESTROY:PostQuitMessage(0); break;
	case WM_CLOSE:  DestroyWindow(hwnd); break;
	default:		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}