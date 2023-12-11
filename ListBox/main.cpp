#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<stdio.h>
#include"resource.h"

CONST CHAR* g_sz_VALUES[] = { "This","is","my","first","List","Box" };

typedef struct
{
	TCHAR ListItem[FILENAME_MAX];
}List;
List list1[];
List list2[];

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMgs, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcAddItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

		HWND hList = GetDlgItem(hwnd, IDC_LIST1);
		for (int i = 0; i < sizeof(g_sz_VALUES) / sizeof(g_sz_VALUES[0]); i++)
		{
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)g_sz_VALUES[i]);
		}
	}
	break;
	case WM_COMMAND:
	{
		

		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			int i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			CHAR sz_buffer[FILENAME_MAX]{};
			CHAR sz_message[FILENAME_MAX]{};
			SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);
			sprintf(sz_message, "Вы выбрали пункт %i со значением \"%s\".", i, sz_buffer);
			MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case IDC_CHECK_SORT:
		{
			HWND hCheckSort = GetDlgItem(hwnd, IDC_CHECK_SORT);
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			if (SendMessage(hCheckSort, BST_CHECKED, 0, 0))
			{
				
			}
			else
				SendMessage(hList, LBS_SORT, 0, (LPARAM)FALSE);
		}
		break;
		case IDC_BUTTON_ADD_ITEM:
		{
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD_ITEM), hwnd, (DLGPROC)DlgProcAddItem, 0);
		}
		break;
		case IDC_BUTTON_DELETE_ITEM:
		{
			CHAR sz_buffer[FILENAME_MAX]{};
			CHAR sz_message[FILENAME_MAX]{};
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			int i = SendMessage(hList,LB_GETCURSEL,0,0);
			SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);
			sprintf(sz_message, "Вы действительно хотите удалить элемент №%i со значением \"%s\"?", i, sz_buffer);
			switch (MessageBox(hwnd,sz_message,"Вопрос",MB_YESNO|MB_ICONQUESTION))
			{
			case IDYES: SendMessage(hList,LB_DELETESTRING,i,0);
			case IDNO: break;
			}
		}
		break;
		case IDCANCEL:EndDialog(hwnd,0);
		}
	}
	break;
	case WM_CLOSE:EndDialog(hwnd, 0);
	}
	return FALSE;
}

BOOL CALLBACK DlgProcAddItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK: 
			{
			CHAR sz_buffer[FILENAME_MAX]{};
			HWND hEditAddItem = GetDlgItem(hwnd, IDC_EDIT_NEW_ITEM);
			SendMessage(hEditAddItem, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			HWND hParent = GetParent(hwnd);
			HWND hList = GetDlgItem(hParent, IDC_LIST1);
			if (SendMessage(hList, LB_FINDSTRING, -1, (LPARAM)sz_buffer) == LB_ERR)
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
			}
		case IDCANCEL:EndDialog(hwnd, 0); break;
		}
	}
	break;
	case WM_CLOSE:EndDialog(hwnd, 0);
	}
	return FALSE;
}