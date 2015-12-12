// Assynhron.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Assynhron.h"
#define ID_BTN_SEND 1
#define ID_BTN_POST 2
#define ID_BTN_POST_THREAD 3
#define ID_LISTBOX 4
#define MAX_LOADSTRING 100
#define WM_SEND (WM_USER+1)
#define WM_POST (WM_USER+2)
#define WM_POST_THREAD (WM_USER+3)
#include <iostream>
HANDLE g_hThread;
HWND g_hThreadWnd;
HWND hWnd;
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];	
// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
DWORD WINAPI GuiThreadProc(void * p);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK	ThreadWndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ASSYNHRON, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASSYNHRON));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int) msg.wParam;
}
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASSYNHRON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ASSYNHRON);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
DWORD WINAPI GuiThreadProc(void * p)
{
	WNDCLASSEX wce = { 0 }; //инициализируем структуру и забиваем её нулями
	wce.cbSize = sizeof(WNDCLASSEX);
	wce.lpfnWndProc = ThreadWndProc;
	wce.hInstance = hInst;
	wce.lpszClassName = "MyWndClass";
	auto retVal = RegisterClassEx(&wce);
	auto err = GetLastError();
	g_hThreadWnd = CreateWindow("MyWndClass", "", WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInst, NULL);
	err = GetLastError();
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_POST_THREAD)
		{
			Sleep(3000);
			SendMessage((HWND)GetDlgItem(hWnd, ID_LISTBOX), LB_ADDSTRING, 0, (LPARAM)"WM_POST_THREAD proceed");
			continue;
		}
		DispatchMessage(&msg);
	}
	return 0;
}
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, 0, 460, 350, NULL, NULL, hInstance, NULL);
   CreateWindow("BUTTON", "Send Message", WS_CHILD | WS_VISIBLE, 310, 25, 120, 25, hWnd, (HMENU)ID_BTN_SEND, hInst, NULL);
   CreateWindow("BUTTON", "Post Message", WS_CHILD | WS_VISIBLE, 310, 75, 120, 25, hWnd, (HMENU)ID_BTN_POST, hInst, NULL);
   CreateWindow("BUTTON", "Post ThreadMsg", WS_CHILD | WS_VISIBLE, 310, 125, 120, 25, hWnd, (HMENU)ID_BTN_POST_THREAD, hInst, NULL);
   CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL, 10, 20, 290, 270, hWnd, (HMENU)ID_LISTBOX, hInst, NULL);
   if (!hWnd)
   {
      return FALSE;
   }
   DWORD Id;
   g_hThread = CreateThread(NULL, 0, GuiThreadProc, NULL, 0, &Id);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK ThreadWndProc(HWND hWnd1, UINT message, WPARAM wParam, LPARAM lParam)// обработка полученных сообщений
{
	switch (message)
	{
	case WM_SEND:
		Sleep(3000);
		SendMessage((HWND)GetDlgItem(hWnd, ID_LISTBOX), LB_ADDSTRING, 0, (LPARAM)"WM_SEND proceed");
		break;
	case WM_POST: 
		Sleep(3000);
		SendMessage((HWND)GetDlgItem(hWnd, ID_LISTBOX), LB_ADDSTRING, 0, (LPARAM)"WM_POST proceed");
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)// отправка сообщений
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	DWORD mistake;
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)// post thred обрабатывается в threadwndproc!!!!
		{
		case ID_BTN_SEND:
			SendMessage(g_hThreadWnd, WM_SEND, 0 , 0);
			mistake = GetLastError();
			break;
		case ID_BTN_POST:
			PostMessage(g_hThreadWnd, WM_POST, 0, 0);
			break;
		case ID_BTN_POST_THREAD:
			PostThreadMessage(GetThreadId(g_hThread), WM_POST_THREAD, 0, 0);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


