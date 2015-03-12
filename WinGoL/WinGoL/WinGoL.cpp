// WinGoL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WinGoL.h"
#include "SDL.h"
#include <iostream>
#include "GoL_Renderer.h"
#include "Cell_Map.h"
#include "L16_FParser.h"
#include "GameTimer.h"

#include <shobjidl.h>
#include <windows.h>
#include <string>
#include <stdlib.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

CLIENTCREATESTRUCT MDIClientCreateStruct;

//SDL and GOL objects
GoL_Renderer renderer;
Cell_Map cell_Map;
GameTimer gol_Timer;
HWND hWnd;
bool simPause = true;

std::string openFilePath();


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg = { 0 };
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINGOL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	//Innitiate SDL
	SDL_InitSubSystem(SDL_INIT_VIDEO);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	std::string patternPath;

	renderer.Init(hWnd);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINGOL));

	patternPath = openFilePath();

	L16_Parser::Load_Pattern(cell_Map, patternPath);

	while (WM_QUIT != msg.message){

		// Main message loop:
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

		}
		renderer.Render_Life(cell_Map.Get_Cellmap());

		if (!simPause){
		
			cell_Map.Next_Gen();
		
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINGOL));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WINGOL);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   //tool_Wnd = CreateWindowEx(WS_EX_TOOLWINDOW, TEXT("Home"),
	  // TEXT("The Home page"),
	  // WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
	  // CW_USEDEFAULT, CW_USEDEFAULT, 700, 400,
	  // NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		





		CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("PAUSE"),
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON |WS_TABSTOP,
			20, 10, 60, 30, hWnd, (HMENU)IDC_PAUSE, GetModuleHandle(NULL), NULL);

		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDC_PAUSE:
			if (!simPause){
				SetDlgItemText(hWnd, IDC_PAUSE, TEXT("START"));
			}
			else{
				SetDlgItemText(hWnd, IDC_PAUSE, TEXT("PAUSE"));
			}
			simPause = !simPause;
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

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

std::string openFilePath(){

	std::string pathRes;
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						/*std::wstringstream ss;
						ss << pszFilePath;
						std::cout << ss.str();*/

						//char buffer[32];
						//int ret;

						//printf("wchar_t string: %ls \n", pszFilePath);

						//ret = wcstombs_s(buffer, pszFilePath, sizeof(buffer));
						//if (ret == 32) buffer[31] = '\0';
						////if (ret) printf("multibyte string: %s \n", buffer);
						//std::string newRes(buffer);

						//pathRes = newRes;

						//std::wstringstream(pszFilePath) >> pathRes;

						size_t outputSize = wcslen(pszFilePath) + 1;
						char* outputString = new char[outputSize];
						size_t charsConverted = 0;
						const wchar_t * inputW = pszFilePath;
						wcstombs_s(&charsConverted, outputString, outputSize, inputW, wcslen(pszFilePath));

						std::string newPath(outputString);
						pathRes = newPath;

						/*MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
						CoTaskMemFree(pszFilePath);*/
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	return pathRes;
}