#include<Windows.h>
#include<Richedit.h>
#include"resource.h"

//CHAR Buffer[8 * 1024 * 1024];
CONST CHAR g_sz_CLASS_NAME[] = "My Text Editor";

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT CALLBACK DlgProcAbout(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL LoadTextFileToEdit(HWND hEdit, LPCSTR lpszFileName);
BOOL SaveTextFileFromEdit(HWND hEdit, LPCSTR lpszFileName);
void LoadFile(HWND hEdit, LPCSTR lpszFileName);
BOOL SavingThisFile(HWND hwnd, BOOL onSave);
VOID SelectFont(HWND hwnd);
HFONT g_hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
COLORREF g_RGB_Text = RGB(0, 0, 0);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса окна
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	//стиль окна
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = 0;
	//внешний вид окна
	wc.hIcon = (HICON)LoadImage(hInstance, "Icons\\text_editor_icon.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hIconSm = (HICON)LoadImage(hInstance, "Icons\\text_editor_icon.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	//привязка названия окна, основной процедуры CALLBACK и основного меню
	wc.hInstance = hInstance;
	wc.lpszClassName = g_sz_CLASS_NAME;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	//2) Создание окна
	HWND hwnd = CreateWindowEx
	(
		WS_EX_ACCEPTFILES,//дает доступ к обработке события дропа файлов в окно приложения
		g_sz_CLASS_NAME,
		g_sz_CLASS_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
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

	//3) Запуск цикла сообщений
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
	static CHAR szFileName[MAX_PATH]{};
	static CHAR* szFileText = NULL;
	static BOOL onSave = TRUE;
	static HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	switch (uMsg)
	{
	case WM_CREATE:
	{
		LoadLibrary("riched20.dll");
		RECT rect;
		GetWindowRect(hwnd, &rect);
		HWND hEdit = CreateWindowEx
		(
			NULL, RICHEDIT_CLASS, "",
			WS_CHILD | WS_VISIBLE | ES_MULTILINE |WS_HSCROLL|WS_VSCROLL,
			0, 0,
			rect.right, rect.bottom,
			hwnd,
			(HMENU)IDC_EDIT,
			GetModuleHandle(NULL),
			NULL
		);
		g_hFont = CreateFont
			(
				32,11,
				0,0,FW_MEDIUM,
				0,0,0, //Italic, Underline, Strikeout
				RUSSIAN_CHARSET,
				OUT_TT_PRECIS,
				CLIP_CHARACTER_PRECIS,
				ANTIALIASED_QUALITY,
				FF_DONTCARE,
				"Tahoma"
			);
		SendMessage(hEdit, WM_SETFONT, (WPARAM)g_hFont,TRUE);
		//регистрируем горячие клавищи с добавлением Ctrl
		RegisterHotKey(hwnd, ID_FILE_OPEN + 10000, MOD_CONTROL, 'O');
		RegisterHotKey(hwnd, ID_FILE_SAVE + 10000, MOD_CONTROL, 'S');

		SetFocus(hEdit);
	}
	break;
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		SetWindowPos(GetDlgItem(hwnd, IDC_EDIT), NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_NOZORDER);
	}
	break;
	case WM_DROPFILES://перетаскивание и дроп файлов в окно приложения
	{
		// DragQueryFile() takes a LPWSTR for the name so we need a TCHAR string
		TCHAR szName[MAX_PATH];
		// Here we cast the wParam as a HDROP handle to pass into the next functions
		HDROP hDrop = (HDROP)wParam;
		// This functions has a couple functionalities.  If you pass in 0xFFFFFFFF in
		// the second parameter then it returns the count of how many filers were drag
		// and dropped.  Otherwise, the function fills in the szName string array with
		// the current file being queried.
		int count = DragQueryFile(hDrop, 0xFFFFFFFF, szName, MAX_PATH);
		// Here we go through all the files that were drag and dropped then display them
		for (int i = 0; i < count; i++)
		{
			// Grab the name of the file associated with index "i" in the list of files dropped.
			// Be sure you know that the name is attached to the FULL path of the file.
			DragQueryFile(hDrop, i, szName, MAX_PATH);
			LoadTextFileToEdit(GetDlgItem(hwnd, IDC_EDIT), (LPCSTR)szName);
		}
		// Finally, we destroy the HDROP handle so the extra memory
		// allocated by the application is released.
		DragFinish(hDrop);
	}
	break;
	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		SetBkMode(hdc, OPAQUE);
		SetBkColor(hdc, g_RGB_Text);
		HBRUSH hBrush = CreateSolidBrush(g_RGB_Text);
		SetTextColor(hdc, g_RGB_Text);
		return (LRESULT)hBrush;
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_NEW:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			onSave = SavingThisFile(hwnd, onSave);
			SendMessage(hEdit, EM_GETMODIFY, FALSE, 0);
			SendMessage(hEdit, WM_SETTEXT, WPARAM(0), LPARAM(""));
		}
		break;
		case ID_FILE_OPEN:
		{
			onSave=SavingThisFile(hwnd, onSave);
			//CHAR szFileName[MAX_PATH]{};
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = "txt";

			if (GetOpenFileName(&ofn))
			{
				HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
				//onSave=LoadTextFileToEdit(hEdit, szFileName);
				//DWORD dwTextLenght = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
				//LPSTR lpszFileContent = (LPSTR)GlobalAlloc(GPTR, dwTextLenght + 1);
				//SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)lpszFileContent);
				
				LoadTextFileToEdit(hEdit, szFileName);
				//LoadFile(hEdit, szFileName);
			}
		}
		break;
		case ID_FILE_SAVE:
		{
			if (strlen(szFileName))
			{
				onSave = SaveTextFileFromEdit(GetDlgItem(hwnd, IDC_EDIT), szFileName);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT), EM_GETMODIFY, FALSE, 0);
			}
			else SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVEAS,0);
		}
		break;
		case ID_FILE_SAVEAS:
		{
			//CHAR szFileName[MAX_PATH]{};
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = (LPCSTR)"*.txt";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.lpstrDefExt = "txt";
			if (GetSaveFileName(&ofn))
			{
				HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
				onSave=SaveTextFileFromEdit(hEdit, szFileName);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT), EM_GETMODIFY, FALSE, 0);
			}
		}
		break;
		case ID_FORMAT_FONT:SelectFont(hwnd); break;
		case ID_HELP_ABOUT:
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ABOUT), hwnd, DlgProcAbout, 0);
			break;
		}
	break;
	case WM_HOTKEY://Прописываем поведение на горячих клавишах с resourse.h
	{
		switch (wParam)
		{
		case ID_FILE_OPEN+10000: 
		case ID_FILE_SAVE+10000: 
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam-10000), 0);
			break;
		}
		break;
	}
	break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE:
	{
		if(SendMessage(GetDlgItem(hwnd,IDC_EDIT),EM_GETMODIFY,0,0))
		{
			onSave = FALSE;
			int message = MessageBox(NULL, "Вы хотите сохранить изменения в файл?", g_sz_CLASS_NAME, MB_YESNOCANCEL | MB_ICONQUESTION);
			switch (message)
			{
			case IDYES:
				{
				SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, 0);
				if (onSave)DestroyWindow(hwnd); break;
				}
			case IDNO:DestroyWindow(hwnd); break;
			case IDCANCEL:break;
			}
		}
		else DestroyWindow(hwnd);
	}
	break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}

INT CALLBACK DlgProcAbout(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
		case IDOK:EndDialog(hwnd, 0);
		}
	case WM_CLOSE:EndDialog(hwnd, 0);
	}
	return FALSE;
}

BOOL LoadTextFileToEdit(HWND hEdit, LPCSTR lpszFileName)
{
	BOOL bSuccess = FALSE;
	//HANDLE hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != UINT_MAX)
		{
			LPSTR lpszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
			if (lpszFileText)
			{
				DWORD dwRead;
				if (ReadFile(hFile, lpszFileText, dwFileSize, &dwRead, NULL))
				{
					lpszFileText[dwFileSize] = 0;
					if (SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)lpszFileText))bSuccess = TRUE;
				}
				GlobalFree(hFile);
			}
			CloseHandle(hFile);
		}
	}
	return bSuccess;
}

BOOL SaveTextFileFromEdit(HWND hEdit, LPCSTR lpszFileName)
{
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//узнаем длину текста для считывания
	int saveLength = GetWindowTextLength(hEdit) + 1;
	//создаем буфер для сохранения текста из окна
	char* data = new char[saveLength];
	//заполняем буфер
	saveLength = GetWindowTextA(hEdit, data, saveLength);
	//записываем побитово в файл считанный буфер
	DWORD byteIterated;
	WriteFile(hFile, data, saveLength, &byteIterated, NULL);
	//закрываем файл
	CloseHandle(hFile);
	//удаляем буфер
	delete[] data;
	return TRUE;
}
//диалог на сохранение файла при редактировании Edit
BOOL SavingThisFile(HWND hwnd, BOOL onSave)
{
	if (SendMessage(GetDlgItem(hwnd, IDC_EDIT), EM_GETMODIFY, 0, 0))
	{
		onSave = FALSE;
		int message = MessageBox(NULL, "Вы хотите сохранить изменения в файл?", g_sz_CLASS_NAME, MB_YESNO | MB_ICONQUESTION);
		switch (message)
		{
		case IDYES:
		{
			SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, 0);
			break;
		}
		case IDNO:break;
		}
	}
	return onSave;
}

VOID SelectFont(HWND hwnd)
{
	HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
	//создали структуры
	CHOOSEFONT cf;
	LOGFONT lf;
	//обнулили структуры
	ZeroMemory(&cf, sizeof(cf));
	ZeroMemory(&lf, sizeof(lf));

	GetObject(g_hFont, sizeof(LOGFONT), &lf);

	cf.lStructSize = sizeof(cf);
	cf.hwndOwner = hwnd;

	cf.Flags = CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
	cf.hInstance = GetModuleHandle(NULL);
	cf.lpLogFont = &lf;
	cf.rgbColors = g_RGB_Text;

	if (ChooseFont(&cf))
	{
		HFONT hf = CreateFontIndirect(&lf);
		if (hf)g_hFont = hf;
		else MessageBox(hwnd, "Font creation failed", "Error", MB_OK | MB_ICONERROR);
	}
	SendMessage(hEdit, WM_SETFONT, (WPARAM)g_hFont, TRUE);	
	g_RGB_Text = cf.rgbColors;

	HDC hdc = GetDC(hEdit);
	SendMessage(hwnd, WM_CTLCOLOREDIT, (WPARAM)hdc, (LPARAM)hEdit);
	ReleaseDC(hEdit, hdc);
}

void LoadFile(HWND hEdit, LPCSTR lpszFileName)
{
	BYTE BUFFERSIZE = 8 * 1024 * 1024;
	// объявление переменной для получения указателя на выделенную память
	LPVOID pBuffer;
	// объявление переменной для хэндла файла
	HANDLE hFile;
	// объявление переменной для получения количества прочитанных байт
	DWORD dwHaveRead;
	// объявление переменной для получения результата функции чтения файла
	BOOL blResult;
	// динамическое выделение памяти нужного объЁма
	pBuffer = VirtualAlloc(NULL, BUFFERSIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	// проверка, что память успешно выделена
	if (pBuffer != NULL)
	{
		// открытие заведомо существующего файла с доступом только на чтение,
		// общим доступом тоже только на чтение и с указанием для ОС, что файл
		// будет последовательно считываться (это необходимо для лучшего кэширования)
		hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

		// проверка, что файл успешно открыт
		if (hFile != INVALID_HANDLE_VALUE)
		{
			// цикл чтения файла кусками
			do
			{
				// чтение файла
				blResult = ReadFile(hFile, pBuffer, BUFFERSIZE, &dwHaveRead, NULL);
				//Тут должна быть дозапись в Edit

			} while (blResult && (dwHaveRead == BUFFERSIZE));

			// закрытие файла
			CloseHandle(hFile);
		}
		// освобождение выделенной памяти
		VirtualFree(pBuffer, 0, MEM_RELEASE);
	}
}