#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<stdio.h>
#include"resource.h"

CONST CHAR* g_sz_VALUES[] = { "This","is","my","first","Combo","Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

INT WINAPI Dialog(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), NULL, (DLGPROC)DlgProc2, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_SPEAKER));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

		HWND hCombo = GetDlgItem(hwnd, IDC_COMBO1);
		for (int i = 0; i < sizeof(g_sz_VALUES) / sizeof(g_sz_VALUES[0]); i++)
		{
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)g_sz_VALUES[i]);
		}
		SendMessage(hCombo, CB_SETCURSEL, 0, 0);
	}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hCombo = GetDlgItem(hwnd, IDC_COMBO1);
			int i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);//получили номер выбранного элемента в CB
			CHAR sz_buffer[MAX_PATH]{};
			CHAR sz_message[MAX_PATH]{};
			SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)sz_buffer);
			sprintf(sz_message, "Вы выбрали пункт %i со значением \"%s\".", i, sz_buffer);
			MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
		}
			break;
		case IDC_BUTTON_ADD:
		{
			HINSTANCE hInstance;
			DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), NULL, (DLGPROC)DlgProc2, 0);
		}
			break;
		case IDCANCEL:EndDialog(hwnd, 0);
		}
		break;
	case WM_CLOSE:EndDialog(hwnd, 0);
	}
	return FALSE;
}

BOOL CALLBACK DlgProc2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
		{
			HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(NULL));
			SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
		}
		break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{

			}
		break;
		case WM_CLOSE:EndDialog(hwnd, 0);
	}
	return FALSE;
}