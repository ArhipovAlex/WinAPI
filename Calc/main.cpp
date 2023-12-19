#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<stdio.h>
#include"resource.h"

CONST CHAR g_sz_CLASSNAME[] = "MyCacl";
CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;
CONST INT g_i_INTERVAL = 5;
CONST INT g_i_BUTTON_SIZE = 50;
CONST INT g_i_BUTTON_DOUBLE_SIZE = g_i_BUTTON_SIZE * 2 + g_i_INTERVAL;
CONST INT g_i_DISPLAY_WIDTH = (g_i_BUTTON_SIZE + g_i_INTERVAL)*5;
CONST INT g_i_DISPLAY_HEIGHT = 22;
CONST INT g_i_BUTTON_START_X = g_i_START_X;
CONST INT g_i_BUTTON_START_Y = g_i_START_Y+g_i_DISPLAY_HEIGHT + g_i_INTERVAL;

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
		WS_OVERLAPPEDWINDOW,			//Window styles
		CW_USEDEFAULT, CW_USEDEFAULT,	//Position
		CW_USEDEFAULT, CW_USEDEFAULT,	//Size
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
	switch (uMsg)
	{
	case WM_CREATE:
	{
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
		for (int i = 6; i >= 0; i-=3)
		{
			for(int j = 0; j < 3; j++)
			{ 
				CHAR sz_digit[2] = "";
				sz_digit[0] = i + j + 49;
				CreateWindowEx
				(
					NULL, "Button", sz_digit,
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					g_i_BUTTON_START_X+(g_i_BUTTON_SIZE+g_i_INTERVAL)*j,//X Position
					g_i_BUTTON_START_Y+(g_i_BUTTON_SIZE+g_i_INTERVAL)*(3-i/3-1),//Y Position
					g_i_BUTTON_SIZE,g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_0+i+j),
					GetModuleHandle(NULL),
					NULL
				);
			}
		}
		CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BUTTON_START_X,
			g_i_BUTTON_START_Y + (g_i_INTERVAL + g_i_BUTTON_SIZE) * 3,
			g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);
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
	}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_0:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			CHAR sz_buffer1[MAX_PATH]{};
			CHAR sz_buffer2[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, 0, (LPARAM)sz_buffer1);
			sprintf(sz_buffer2, "%s%s", sz_buffer1, "0");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer2);
		}
		break;
		case IDC_BUTTON_1:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			CHAR sz_buffer1[MAX_PATH]{};
			CHAR sz_buffer2[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, 0, (LPARAM)sz_buffer1);
			sprintf(sz_buffer2, "%s%s", sz_buffer1, "1");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer2);
		}
		break;
		case IDC_BUTTON_2:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			CHAR sz_buffer1[MAX_PATH]{};
			CHAR sz_buffer2[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, 0, (LPARAM)sz_buffer1);
			sprintf(sz_buffer2, "%s%s", sz_buffer1, "2");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer2);
		}
		break;
		case IDC_BUTTON_3:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			CHAR sz_buffer1[MAX_PATH]{};
			CHAR sz_buffer2[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, 0, (LPARAM)sz_buffer1);
			sprintf(sz_buffer2, "%s%s", sz_buffer1, "3");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer2);
		}
		break;
		case IDC_BUTTON_4:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			CHAR sz_buffer1[MAX_PATH]{};
			CHAR sz_buffer2[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, 0, (LPARAM)sz_buffer1);
			sprintf(sz_buffer2, "%s%s", sz_buffer1, "4");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer2);
		}
		break;
		case IDC_BUTTON_5:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			CHAR sz_buffer1[MAX_PATH]{};
			CHAR sz_buffer2[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, 0, (LPARAM)sz_buffer1);
			sprintf(sz_buffer2, "%s%s", sz_buffer1, "5");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer2);
		}
		break;
		case IDC_BUTTON_6:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			CHAR sz_buffer1[MAX_PATH]{};
			CHAR sz_buffer2[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, 0, (LPARAM)sz_buffer1);
			sprintf(sz_buffer2, "%s%s", sz_buffer1, "6");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer2);
		}
		break;
		case IDC_BUTTON_7:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			CHAR sz_buffer1[MAX_PATH]{};
			CHAR sz_buffer2[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, 0, (LPARAM)sz_buffer1);
			sprintf(sz_buffer2, "%s%s", sz_buffer1, "7");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer2);
		}
		break;
		case IDC_BUTTON_8:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			CHAR sz_buffer1[MAX_PATH]{};
			CHAR sz_buffer2[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, 0, (LPARAM)sz_buffer1);
			sprintf(sz_buffer2, "%s%s", sz_buffer1, "8");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer2);
		}
		break;
		case IDC_BUTTON_9:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			CHAR sz_buffer1[MAX_PATH]{};
			CHAR sz_buffer2[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, 0, (LPARAM)sz_buffer1);
			sprintf(sz_buffer2, "%s%s", sz_buffer1, "9");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer2);
		}
		break;
		case IDC_BUTTON_POINT:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			CHAR sz_buffer1[MAX_PATH]{};
			CHAR sz_buffer2[MAX_PATH]{};
			SendMessage(hEdit, WM_GETTEXT, 0, (LPARAM)sz_buffer1);
			sprintf(sz_buffer2, "%s%s", sz_buffer1, ".");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer2);
		}
		break;
		}
		break;
	case WM_DESTROY:PostQuitMessage(0); break;
	case WM_CLOSE: DestroyWindow(hwnd);
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}