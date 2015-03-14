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
#include <windowsx.h>
#include <string>
#include <stdlib.h>
#include <WinUser.h>

#include "Settings.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

CLIENTCREATESTRUCT MDIClientCreateStruct;

std::string patternPath;

//SDL and GOL objects
GoL_Renderer renderer;
Cell_Map cell_Map;
GameTimer gol_Timer;
HWND hWnd, tWnd, sdl_Wnd, debgWnd;
bool simPause = false;

HIMAGELIST g_hImageList = NULL;

HWND CreateSimpleToolbar(HWND hWndParent, HINSTANCE hInstance);

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

	renderer.Init(sdl_Wnd);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINGOL));

	
	
	INITCOMMONCONTROLSEX icex;

	// Ensure that the common control DLL is loaded. 
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	


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
		if (GetAsyncKeyState(0x57)){ cameraY += 5 + (cameraZ + 1); };
		if (GetAsyncKeyState(0x53)){ cameraY -= 5 + (cameraZ + 1); };
		if (GetAsyncKeyState(0x41)){ cameraX += 5 + (cameraZ + 1); };
		if (GetAsyncKeyState(0x44)){ cameraX -= 5 + (cameraZ + 1); };

	}

	return (int) msg.wParam;
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

	RECT rects, toolRkt;
	int lheight;
	int lwidth;

	GetClientRect(hWnd, &rects);
	
	
	lwidth = rects.right;
	lheight = rects.bottom;
	sdlw_Width = lwidth;
	sdlw_Height = lheight;

	switch (message)
	{
	case WM_CREATE:
		
		tWnd = CreateSimpleToolbar(hWnd, hInst);

		GetClientRect(tWnd, &toolRkt);

		sdl_Wnd = CreateWindowEx(WS_EX_CLIENTEDGE,
			TEXT("sdlWnd"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			rects.left,
			rects.top+toolRkt.bottom,
			lwidth,
			lheight,
			hWnd,
			NULL,
			GetModuleHandle(NULL),
			NULL);
		if (NULL != sdl_Wnd)
		{
			ShowWindow(sdl_Wnd, SW_SHOW);
			UpdateWindow(sdl_Wnd);
		}

		debgWnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
			TEXT("debgWnd"),
			TEXT("Debug"),
			WS_BORDER | WS_SYSMENU | WS_DLGFRAME | WS_THICKFRAME,
			90,
			90,
			150,
			300,
			0,
			NULL,
			GetModuleHandle(NULL),
			NULL);
		
		if (NULL != debgWnd)
		{
			//ShowWindow(debgWnd, SW_SHOW);
			UpdateWindow(debgWnd);
		}

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
			simPause = !simPause;
			break;
		case IDM_OPEN:
			cell_Map.Clear_Map();
			patternPath = openFilePath();
			L16_Parser::Load_Pattern(cell_Map, patternPath);

			cameraX = sdlw_Width / 2;
			cameraY = sdlw_Height / 2;

			break;
		case IDM_PAUSE:
			simPause = !simPause;
			break;
		case IDM_DEBG:

			if (dbg_Toggle){
				
				/*if (NULL != sdl_Wnd)
				{
					ShowWindow(sdl_Wnd, SW_SHOW);
					UpdateWindow(sdl_Wnd);
				}*/
				ShowWindow(debgWnd, SW_SHOW);
				dbg_Toggle = !dbg_Toggle;
			}
			else{
				ShowWindow(debgWnd, SW_HIDE);
				dbg_Toggle = !dbg_Toggle;
			}

			break;
		case IDM_NEW:
			cell_Map.Clear_Map();
			break;
		
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_KEYDOWN:
		//E = 0x45
		if (wParam == 0x45){ simPause = !simPause; }

		//Q = 0x51
		if (wParam == 0x51){ dead_Colour_ON = !dead_Colour_ON; }

		break;

	case WM_MOUSEWHEEL:

		if (wParam > 0){
			cameraZ += (GET_WHEEL_DELTA_WPARAM(wParam)/120);
		}
		else{
			cameraZ -= (GET_WHEEL_DELTA_WPARAM(wParam) / 120);
		}

		if (cameraZ < 0){
			cameraZ = 0;
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

LRESULT CALLBACK SDLProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;


	//Mouse Coords
	int Mx = 0;
	int My = 0;

	switch (message)
	{
	case WM_CREATE:


		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:

		Mx = GET_X_LPARAM(lParam);
		My = GET_Y_LPARAM(lParam);

		Mx = Mx - cameraX;
		My = My - cameraY;

		Mx = Mx / (cameraZ + 1);
		My = My / (cameraZ + 1);

		cell_Map.Add_Cell(Mx,My);

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

LRESULT CALLBACK debgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	POINT p;
	GetCursorPos(&p);
	ScreenToClient(sdl_Wnd, &p);

	RECT rc;
	GetClientRect(debgWnd, &rc);

	std::wstring MousePosition_Wnd = TEXT("MousePosition_Wnd:: ") + std::to_wstring(cameraZ); // or wstring if you have unicode set
	std::wstring strOut = TEXT("Hello World!"); // or wstring if you have unicode set
	//std::wstring strOut = TEXT("Hello World!"); // or wstring if you have unicode set
	//std::wstring strOut = TEXT("Hello World!"); // or wstring if you have unicode set
	//std::wstring strOut = TEXT("Hello World!"); // or wstring if you have unicode set

	switch (message)
	{
	case WM_CREATE:

		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		
		

		hdc = BeginPaint(hWnd, &ps);
		
		

		SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 0, 0));

		
		DrawText(hdc, MousePosition_Wnd.c_str(), MousePosition_Wnd.length(), &rc, DT_SINGLELINE);
		
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

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	ATOM RetVAL = 0;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINGOL));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_WINGOL);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	// Register the Main Window class with the OS
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"),
			MB_ICONEXCLAMATION | MB_OK);
		RetVAL = 0;
	}

	wcex.lpfnWndProc = SDLProc;
	wcex.lpszClassName = TEXT("sdlWnd");
	wcex.hCursor = LoadCursor(NULL, IDC_CROSS);

	// Register the SDL Window class with the OS
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"),
			MB_ICONEXCLAMATION | MB_OK);
		RetVAL = 0;
	}

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;
	wcex.lpfnWndProc = debgProc;
	wcex.lpszClassName = TEXT("debgWnd");
	wcex.lpszMenuName = NULL;

	// Register the SDL Window class with the OS
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"),
			MB_ICONEXCLAMATION | MB_OK);
		RetVAL = 0;
	}

	return RetVAL = 0;
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
						
						size_t outputSize = wcslen(pszFilePath) + 1;
						char* outputString = new char[outputSize];
						size_t charsConverted = 0;
						const wchar_t * inputW = pszFilePath;
						wcstombs_s(&charsConverted, outputString, outputSize, inputW, wcslen(pszFilePath));

						std::string newPath(outputString);
						pathRes = newPath;

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


HWND CreateSimpleToolbar(HWND hWndParent, HINSTANCE hInstance)
{
	
	HWND hWndToolbar;

	// Declare and initialize local constants.
	const int ImageListID = 0;
	const int numButtons = 5;
	const int bitmapSize = 16;

	const DWORD buttonStyles = BTNS_AUTOSIZE;

	// Create the toolbar.
	hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
		WS_CHILD | TBSTYLE_WRAPABLE | TBSTYLE_FLAT | TBSTYLE_LIST, 0, 0, 0, 0,
		hWndParent, NULL,hInstance, NULL);

	if (hWndToolbar == NULL)
		return NULL;

	// Create the image list.
	g_hImageList = ImageList_Create(bitmapSize, bitmapSize,   // Dimensions of individual bitmaps.
		ILC_COLOR16 | ILC_MASK,   // Ensures transparent background.
		numButtons, 0);

	// Set the image list.
	SendMessage(hWndToolbar, TB_SETIMAGELIST,
		(WPARAM)ImageListID,
		(LPARAM)g_hImageList);

	// Load the button images.
	SendMessage(hWndToolbar, TB_LOADIMAGES,
		(WPARAM)IDB_STD_SMALL_COLOR,
		(LPARAM)HINST_COMMCTRL);

	// Initialize button info.
	// IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.

	TBBUTTON tbButtons[numButtons] =
	{
		{ MAKELONG(STD_FILENEW, ImageListID), IDM_NEW, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"New" },
		{ MAKELONG(STD_FILEOPEN, ImageListID), IDM_OPEN, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Open" },
		{ MAKELONG(STD_FILESAVE, ImageListID), IDM_SAVE, 0, buttonStyles, { 0 }, 0, (INT_PTR)L"Save" },
		{ MAKELONG(STD_REPLACE, ImageListID), IDM_DEBG, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Debug" },
		{ MAKELONG(STD_OUTPUT_HANDLE, ImageListID), IDM_PAUSE, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)L"Pause" }
	};

	// Add buttons.
	SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);

	// Resize the toolbar, and then show it.
	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndToolbar, TRUE);

	return hWndToolbar;
}