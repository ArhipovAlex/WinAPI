#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <winuser.h>
#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")
#define BUTTON 1

const char g_szClassName[] = "myWindowClass";
HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
using namespace Gdiplus;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;


    hInst = hInstance;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hbrBackground = CreateSolidBrush(RGB(0,0xc8,0xc8));
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); // 02
    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName,
        "Round Button",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 640, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    GdiplusShutdown(gdiplusToken);  // 03 
    return Msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndButton;
    LPDRAWITEMSTRUCT pdis;

    switch (msg)
    {
    case WM_CREATE:
    {
        //размеры кнопки желательно чтобы совпадали с размером вставляемой текстуры (173х173) иначе недорисует
        hwndButton = CreateWindow("button", "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            10, 10, 173, 173, hwnd, (HMENU)BUTTON, hInst, NULL);
        HRGN hrgn = CreateEllipticRgn(0, 0, 173, 173);
        SetWindowRgn(hwndButton, hrgn, TRUE);
    }
    break;
    case WM_DRAWITEM:
    {
        pdis = (LPDRAWITEMSTRUCT)lParam;
        Graphics graphics(pdis->hDC);
        graphics.SetSmoothingMode(SmoothingModeHighQuality);
        Pen      pen(Color(255, 0, 0, 255));
        //FillRect(pdis->hDC, &pdis->rcItem, (HBRUSH)GetStockObject(GRAY_BRUSH));
        FillRect(pdis->hDC, &pdis->rcItem, CreateSolidBrush(RGB(0, 0xc8, 0xc8)));
        if (pdis->CtlID == BUTTON)
        {
            RECT& rec = pdis->rcItem;
            RectF ellipseRect(rec.left, rec.top, rec.right - 1, rec.bottom - 1);
            Brush* brush = new SolidBrush(Color::Green);
            //для заполнения кнопки используется текстура, как подогнать по размерам кнопки текстуру???
            Image image(L"button_1.bmp");
            TextureBrush tBrush(&image);
            graphics.FillEllipse(&tBrush, ellipseRect);
            
        }
        
    }break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
    }
    break;
    case WM_COMMAND:
    {
        switch (wParam)
        {
        case BUTTON:
            MessageBox(NULL, "Button pressed", "", NULL);
            break;
        default:
            break;
        }
    }break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}