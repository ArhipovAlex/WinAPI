#include<Windows.h>
#include"resource.h"

CONST CHAR g_sz_USERNAME_MESSAGE[] = "Введите имя пользователя";

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//HWND - окно (Handler to Window - обработчик окна
	//uMsg - сообщение, которое передается окну
	//wParam и lParam - параметры сообщения
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{// здесь создаются элементы интерфейса
		//секция отрабатывает при запуске окна один раз
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
		//Первичная подсказка в Edit_Login
		SendMessage(GetDlgItem(hwnd,IDC_EDIT_LOGIN), WM_SETTEXT, 0, (LPARAM)g_sz_USERNAME_MESSAGE);
	}
		break;
	case WM_COMMAND:
		// здесь обрабатываются команды нажатием кнопок, сочетанием клавиш и тд.
		switch (LOWORD(wParam))
		{
			//события объекта IDC_EDIT_LOGIN
		case IDC_EDIT_LOGIN:
		{
			//извлечем содержимое Edit и проверим на наличие подсказки, и либо ее уберем или если это не подсказка оставим
			CHAR sz_buffer[MAX_PATH] = {};
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			SendMessage(hEditLogin, WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);
			if (HIWORD(wParam)==EN_SETFOCUS && strcmp(sz_buffer,g_sz_USERNAME_MESSAGE)==0)
				SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)"");
			if (HIWORD(wParam) == EN_KILLFOCUS && strlen(sz_buffer) == 0)
				SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)g_sz_USERNAME_MESSAGE);

		}
			break;
		case IDC_BUTTON_COPY:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDOK: MessageBox(hwnd, "Была нажата кнопка ОК", "Info", MB_OK | MB_ICONINFORMATION); break;
		case IDCANCEL: EndDialog(hwnd, 0); break;
		}
		break;
	case WM_CLOSE:
		//отрабатывает при нажании кнопки Закрыть
		EndDialog(hwnd, 0);
	}
	return FALSE;
}