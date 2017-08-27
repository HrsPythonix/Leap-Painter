// GT_HelloWorldWin32.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c
#include <windows.h>
#include <Commdlg.h>
#include <windowsx.h>		//GET_X_LPARAM
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <tchar.h>
#include <wingdi.h>
#include <cmath>
#include"resource.h"
#include "Leap.h"
#include "Leap_Class.h"


#include <CommCtrl.h> //包含头文件  
// 导入静态库  
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "Comctl32.lib")  
using namespace std;


typedef struct tagData
{
	int ptBeginX, ptBeginY;//起点  
	int ptEndX, ptEndY;//终点  
	int penStyle;//画笔的线型
	int CurWidth;
	COLORREF CurrentColor;//画笔颜色
} PAINTDATA;
static COLORREF CurrentColor = RGB(0, 0, 0);
static int CurWidth = 1;
static vector<PAINTDATA> datas;
static int penStyle = PS_SOLID;
static PAINTDATA *pCurrentData = NULL;//指向当前PAINTDATA的指针  
void SetCurrentColor(COLORREF newColor) {
	CurrentColor = newColor;
}

void SetCurWidth(int newWidth) {
	CurWidth = newWidth;
}

//listener & listener
Leap_Listener listener;
Controller controller;


//current hinstance
HINSTANCE hInst;

// The main window class name.
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Leap_Demo");

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance; // Store instance handle in our global variable

					   // The parameters to CreateWindow explained:
					   // szWindowClass: the name of the application
					   // szTitle: the text that appears in the title bar
					   // WS_OVERLAPPEDWINDOW: the type of window to create
					   // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
					   // 500, 100: initial size (width, length)
					   // NULL: the parent of this window
					   // NULL: this application does not have a menu bar
					   // hInstance: the first parameter from WinMain
					   // NULL: not used in this application
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 800,
		NULL,
		LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1)),
		hInstance,
		NULL
		);



	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);
		return 1;
	}



	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd, nCmdShow);
	while (flag)
		listener.onFrame(controller);
	listener.onConnect(controller);

	pCurrentData = new PAINTDATA;
	pCurrentData->penStyle = penStyle;
	pCurrentData->ptBeginX = cur_pos[0];
	pCurrentData->ptBeginY = cur_pos[1];


	UpdateWindow(hWnd);


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{	
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			//将所有线条重新画一遍  
			vector<PAINTDATA>::const_iterator item;
			for (item = datas.begin(); item != datas.end(); item++)
			{
				HPEN pen = CreatePen(item->penStyle, CurWidth, CurrentColor);
				SelectObject(ps.hdc, pen);
				MoveToEx(ps.hdc, item->ptBeginX, item->ptBeginY, NULL);
				LineTo(ps.hdc, item->ptEndX, item->ptEndY);
				DeleteObject(pen);
			}
			EndPaint(hWnd, &ps);
		}
		return 0;
		case WM_DESTROY:
			controller.removeListener(listener);
			PostQuitMessage(0);
			return 0;
		case WM_COMMAND:
		{
			// 取出资源Id值  
			// 并判断用户选择了哪个菜单项  
			switch (LOWORD(wParam))
			{
			case ID_LEAP:
				MessageBox(hWnd, "详见 leapmotion.com", "关于", MB_OK);
				break;
			case ID_LEAPPAINTER:
				MessageBox(hWnd, "Powered by Pythonix", "关于", MB_OK);
				break;
				//修改画笔颜色
			case IDM_BLACK:
				SetCurrentColor(RGB(0, 0, 0));
				break;
			case IDM_RED:
				SetCurrentColor(RGB(255, 0, 0));
				break;
			case IDM_ORANGE:
				SetCurrentColor(RGB(250, 128, 114));
				break;
			case IDM_YELLOW:
				SetCurrentColor(RGB(255, 255, 0));
				break;
			case IDM_GREEN:
				SetCurrentColor(RGB(0, 255, 0));
				break;
			case IDM_LIGHT_BLUE:
				SetCurrentColor(RGB(0, 255, 255));
				break;
			case IDM_BLUE:
				SetCurrentColor(RGB(0, 0, 255));
				break;
			case IDM_PURPLE:
				SetCurrentColor(RGB(160, 32, 240));
				break;
				//修改画笔宽度 
			case IDM_WIDTH1:
				SetCurWidth(1);
				break;
			case IDM_WIDTH2:
				SetCurWidth(2);
				break;
			case IDM_WIDTH4:
				SetCurWidth(4);
				break;
			case IDM_WIDTH8:
				SetCurWidth(8);
				break;
			case IDM_OPEN:
				OPENFILENAME ofn;       // common dialog box structure
				char szFile[260];       // buffer for file name
				HWND hwnd;
				HANDLE hf;              // file handle

										// Initialize OPENFILENAME
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = szFile;
				// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
				// use the contents of szFile to initialize itself.
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				// Display the Open dialog box. 

				if (GetOpenFileName(&ofn) == TRUE)
					hf = CreateFile(ofn.lpstrFile,
						GENERIC_READ,
						0,
						(LPSECURITY_ATTRIBUTES)NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						(HANDLE)NULL);
				
			default:
				break;
			}
		}
		return 0;
		
		case WM_LBUTTONDOWN:
			while (true)
			{
				if (pCurrentData != NULL)
				{
					listener.onFrame(controller);
					//获取终点  
					if (switcher){
						switcher = 0;
						break;
				}
					pCurrentData->ptEndX = cur_pos[0];
					pCurrentData->ptEndY = cur_pos[1];
					//画出线条  
					HDC hdc = GetDC(hWnd);
					HPEN pen = CreatePen(pCurrentData->penStyle, CurWidth, CurrentColor);
					HPEN oldpen = (HPEN)SelectObject(hdc, pen);
					MoveToEx(hdc, pCurrentData->ptBeginX, pCurrentData->ptBeginY, NULL);
					LineTo(hdc, pCurrentData->ptEndX, pCurrentData->ptEndY);
					SelectObject(hdc, oldpen);
					DeleteObject(pen);
					ReleaseDC(hWnd, hdc);
					//把当前数据添加到vector中  
					datas.push_back(*pCurrentData);
					delete pCurrentData;
					pCurrentData = new PAINTDATA;
					pCurrentData->penStyle = penStyle;
					pCurrentData->ptBeginX = cur_pos[0];
					pCurrentData->ptBeginY = cur_pos[1];
					UpdateWindow(hWnd);	//THIS FUNCTION SEND WM_PAINT 
										// Main message loop:
				}

			}
			return 0;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
