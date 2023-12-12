#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<cstdio>
#include"resource.h"

//g - Global
CONST CHAR* G_SZ_ARR_PROPERTIES[] =
{
	"����� ���������� �� ��������",
	"������ �������� ��������",
	"������ ����� ���"

	"�� ��������� �������� �������",
	"�� �������� �������"
};

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMgs, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMgs, WPARAM wParam, LPARAM lParam)
{
	switch (uMgs)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

	}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			CONST INT SIZE = 1024;
			CHAR sz_fullname[MAX_PATH]{};
			CHAR sz_description[SIZE]{};
			HWND hEditFullName = GetDlgItem(hwnd, IDC_EDIT_FULLNAME);
			HWND hEditDescription = GetDlgItem(hwnd, IDC_EDIT_DESCRIPTION);
			//HWND boxes[5]{};
			SendMessage(hEditFullName, WM_GETTEXT, MAX_PATH, (LPARAM)sz_fullname);
			strcpy(sz_description, sz_fullname);
			for (int i = 0; i < 5; i++)
			{
				HWND hBox = GetDlgItem(hwnd, IDC_CHECK_ATTENTION + i);
				if (BST_CHECKED == SendMessage(hBox, BM_GETCHECK, 0, 0))
					sprintf(sz_description, "%s, %s", sz_description, G_SZ_ARR_PROPERTIES[0]);

			}
			//if (BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_CHECK_ATTENTION), BM_GETCHECK, 0, 0))
			//	sprintf(sz_description, "%s, %s", sz_description, G_SZ_ARR_ADVANTAGES[0]);
			SendMessage(hEditDescription, WM_SETTEXT, 0, (LPARAM)sz_description);
		}
		break;
		case IDCANCEL:EndDialog(hwnd, 0); break;
		}
		break;
	case WM_CLOSE:EndDialog(hwnd, 0);
	}
	return FALSE;
}