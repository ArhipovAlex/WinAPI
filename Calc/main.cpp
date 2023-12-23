#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<iostream>
#include<stdio.h>
#include"resource.h"

CONST CHAR g_sz_CLASSNAME[] = "MyCacl";
CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;
CONST INT g_i_INTERVAL = 5;
CONST INT g_i_BUTTON_SIZE = 50;
CONST INT g_i_BUTTON_DOUBLE_SIZE = g_i_BUTTON_SIZE * 2 + g_i_INTERVAL;
CONST INT g_i_DISPLAY_WIDTH = (g_i_BUTTON_SIZE + g_i_INTERVAL)*5-g_i_INTERVAL;
CONST INT g_i_DISPLAY_HEIGHT = 22;
CONST INT g_i_BUTTON_START_X = g_i_START_X;
CONST INT g_i_BUTTON_START_Y = g_i_START_Y+g_i_DISPLAY_HEIGHT + g_i_INTERVAL;
CONST INT g_i_WINDOW_WIDTH = g_i_DISPLAY_WIDTH + g_i_START_X * 2+16;
CONST INT g_i_WINDOW_HEIGHT = g_i_DISPLAY_HEIGHT + g_i_START_Y * 2 + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 4+42;

CONST CHAR* g_sz_arr_OPERATIONS[] = {"+","-","*","/"};
CONST CHAR* g_sz_arr_DIGITMAPS[] = { "button_1", "button_2", "button_3", "button_4", "button_5", "button_6", "button_7", "button_8", "button_9" };

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса Window:
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = 0;

	//wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hIcon = (HICON)LoadImage(hInstance, "calc.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hIconSm = (HICON)LoadImage(hInstance, "calc.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wc.hInstance = hInstance;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_sz_CLASSNAME;
	wc.lpfnWndProc = (WNDPROC)WndProc;

	if (RegisterClassEx(&wc) == NULL)
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	//2) Создать Window:
	HWND hwnd = CreateWindowEx
	(
		NULL,							//ExStyles
		g_sz_CLASSNAME,					//Class name
		g_sz_CLASSNAME,					//Window name
		WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX,//Window styles ^-XOR исключение
		CW_USEDEFAULT, CW_USEDEFAULT,	//Position
		//CW_USEDEFAULT, CW_USEDEFAULT,
		g_i_WINDOW_WIDTH, g_i_WINDOW_HEIGHT,//Size
		NULL,							//Parent window
		NULL,							//Menu
		hInstance,
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//3) Запустить цикл сообщений:
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
	CONST INT i_DISPLAY_BUFFER_SIZE = 256;
	static CHAR sz_display[i_DISPLAY_BUFFER_SIZE]{};
	static DOUBLE a=0,b=0;
	static INT operation;
	static BOOL input = FALSE;
	static BOOL operation_input = FALSE;
	static BOOL in_default_state = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HFONT hFont;

		LOGFONT LF = { -22, 0, 0, 0, FW_HEAVY, 0, 0, 0, RUSSIAN_CHARSET,
		   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, 0, "Microsoft Sans Serif" };
		hFont = CreateFontIndirect(&LF);

		HWND hEdit = CreateWindowEx
		(
			NULL, "Edit", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
			g_i_START_X, g_i_START_Y,
			g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGHT,
			hwnd,
			(HMENU)IDC_EDIT,
			GetModuleHandle(NULL),
			NULL
		);

		SendDlgItemMessage(hwnd, IDC_EDIT, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));

		for (int i = 6; i >= 0; i-=3)
		{
			for(int j = 0; j < 3; j++)
			{ 
				CHAR sz_digit[2] = "";
				sz_digit[0] = i + j + 49;
				HWND button1 = CreateWindowEx
				(
					NULL, "Button", sz_digit,
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
					g_i_BUTTON_START_X+(g_i_BUTTON_SIZE+g_i_INTERVAL)*j,//X Position
					g_i_BUTTON_START_Y+(g_i_BUTTON_SIZE+g_i_INTERVAL)*(3-i/3-1),//Y Position
					g_i_BUTTON_SIZE,g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_1+i+j),
					GetModuleHandle(NULL),
					NULL
				);
				HANDLE hImg = LoadImageW(NULL, (LPCWSTR)g_sz_arr_DIGITMAPS[i+j], IMAGE_BITMAP, g_i_BUTTON_SIZE, g_i_BUTTON_SIZE, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
				SendMessageW(button1, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImg);
			}
		}
		HWND button0=CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			g_i_BUTTON_START_X,
			g_i_BUTTON_START_Y + (g_i_INTERVAL + g_i_BUTTON_SIZE) * 3,
			g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);
		HANDLE hImg = LoadImageW(NULL, L"button_0.bmp", IMAGE_BITMAP, g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		SendMessageW(button0, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImg);
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BUTTON_START_X+g_i_BUTTON_DOUBLE_SIZE+g_i_INTERVAL,
			g_i_BUTTON_START_Y + (g_i_INTERVAL + g_i_BUTTON_SIZE) * 3,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_POINT,
			GetModuleHandle(NULL),
			NULL
		);
		//Operations
		for (int i = 0; i < 4; i++)
		{
			CreateWindowEx
			(
				NULL,
				"Button",
				g_sz_arr_OPERATIONS[3-i],
				WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
				g_i_BUTTON_START_X+(g_i_BUTTON_SIZE+g_i_INTERVAL)*3,
				g_i_BUTTON_START_Y+(g_i_BUTTON_SIZE+g_i_INTERVAL)*i,
				g_i_BUTTON_SIZE,
				g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_SLASH-i),
				GetModuleHandle(NULL),
				NULL
			);
		}
		CreateWindowEx
		(
			NULL,
			"Button",
			"<-",
			WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
			g_i_BUTTON_START_X+(g_i_BUTTON_SIZE+g_i_INTERVAL)*4,
			g_i_BUTTON_START_Y,
			g_i_BUTTON_SIZE,g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_BSP,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL,
			"Button",
			"C",
			WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
			g_i_BUTTON_START_X+(g_i_BUTTON_SIZE+g_i_INTERVAL)*4,
			g_i_BUTTON_START_Y+ g_i_BUTTON_SIZE + g_i_INTERVAL,
			g_i_BUTTON_SIZE,g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_CLEAR,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL,
			"Button",
			"=",
			WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
			g_i_BUTTON_START_X+(g_i_BUTTON_SIZE+g_i_INTERVAL)*4,
			g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL)*2,
			g_i_BUTTON_SIZE,g_i_BUTTON_DOUBLE_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_EQUAR,
			GetModuleHandle(NULL),
			NULL
		);
	}
		break;
	case WM_COMMAND:
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
		SendMessage(hEdit, WM_GETTEXT, i_DISPLAY_BUFFER_SIZE, (LPARAM)sz_display);
		CHAR sz_symbol[2]{};
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_POINT)
		{
			if (input==FALSE)SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");
			sz_symbol[0] = LOWORD(wParam) - IDC_BUTTON_0 + 48;
			if (LOWORD(wParam) == IDC_BUTTON_POINT)
			{
				if (strchr(sz_display, '.'))break;
				sz_symbol[0] = '.';
			}
			strcat(sz_display, sz_symbol);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_display);
			input = TRUE;
		}
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			SendMessage(hEdit, WM_GETTEXT, i_DISPLAY_BUFFER_SIZE, (LPARAM)sz_display);
			if (input)
			{
				b = atof(sz_display);
				input = FALSE;
			}
			if (in_default_state)
			{
				a = b;
				in_default_state = FALSE;
			}
			if(input && operation_input)SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAR), 0);
			input = FALSE;
			operation = LOWORD(wParam);
			operation_input = TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_EQUAR)
		{
			SendMessage(hEdit, WM_GETTEXT, i_DISPLAY_BUFFER_SIZE, (LPARAM)sz_display);
			if (input)b = atof(sz_display);
			input = FALSE;
			switch (operation)
			{
			case IDC_BUTTON_PLUS: a += b; break;
			case IDC_BUTTON_MINUS:a -= b; break;
			case IDC_BUTTON_ASTER:a *= b; break;
			case IDC_BUTTON_SLASH:a /= b; break;
			}
			operation_input = FALSE;
			sprintf(sz_display, "%g", a);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLEAR)
		{
			sz_display[0] = 0;
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_display);
			a = b = 0;
			operation = 0;
			input = FALSE;
			operation_input = FALSE;
			in_default_state = TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			sz_symbol[0] = sz_display[strlen(sz_display)-1];
			sz_display[strlen(sz_display) - 1] = 0;
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
	}
		break;
	case WM_DESTROY:PostQuitMessage(0); break;
	case WM_CLOSE: DestroyWindow(hwnd);
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}