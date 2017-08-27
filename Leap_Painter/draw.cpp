//main implementation
#include "Line.h"
#include "Tool.h"
#include "Rect.h"
#include "Ellip.h"
#include "Curve.h"
#include "Triangle.h"
#include <windows.h>
#include <conio.h>
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
#include <CommCtrl.h> 
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
static COLORREF CurrentColor = CTool::m_nColor;
static int CurWidth = CTool::m_hPenBody;
static vector<PAINTDATA> datas;
static int penStyle = PS_SOLID;
static PAINTDATA *pCurrentData = NULL;//指向当前PAINTDATA的指针  


//listener & listener
Leap_Listener listener;
Controller controller;

HINSTANCE hInst = NULL;//解决工具条显示位图问题,传递入口函数实例句柄到工具条实例
HWND hwnd;//传递主回调函数窗口句柄到退出对话框

long __stdcall WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
long CALLBACK DialogProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam);
long CALLBACK DialogProc1(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam);
void setOPN(OPENFILENAME * ofName,char strFile[]);

int __stdcall WinMain(HINSTANCE hInstance,
					  HINSTANCE hPrevInstance,
					  LPSTR lpCmdLine,int nCmdShow)
{
	hInst = hInstance;
	//1、定义窗口类
	WNDCLASSEX wcls;
	//2、填充窗口类
	wcls.cbClsExtra=0;  //额外的信息，窗口类扩展
	wcls.cbSize =sizeof(wcls); //窗口类的大小
	wcls.cbWndExtra =0;   //额外的信息，窗口实例扩展
	wcls.hInstance = hInstance;   //指定当前应用程序实例句柄,也就是程序当前的标识号。
	wcls.hbrBackground =(HBRUSH)(COLOR_WINDOW+1);  //设置窗口的背景颜色
	wcls.hCursor = LoadCursor(NULL,IDC_CROSS);//设置光标的样式
	wcls.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));  //大图标，EXE的图标
	wcls.hIconSm = LoadIcon(wcls.hInstance,MAKEINTRESOURCE(IDI_ICON1));  //小图标，设置窗口标题栏的图标
	wcls.lpfnWndProc = WndProc;   //指定窗口的处理函数为WndProc，WndProc将处理windows消息。
	wcls.lpszClassName = "Leap_Painter";   //窗口类对象的名称
	wcls.lpszMenuName = (char *)IDR_MENU;    //设置窗口的菜单，要菜单句柄。
	wcls.style = CS_HREDRAW|CS_VREDRAW;       //窗口类的样式风格
	//3、注册窗口类
	RegisterClassEx(&wcls);
	//4、实例化窗口（创建窗口）
	HWND hwnd;
	hwnd=CreateWindow("Leap_Painter"    //窗口类的名称
		,"Leap Painter"     //窗口的标题
		,WS_OVERLAPPEDWINDOW     //窗口的样式
		,CW_USEDEFAULT,0        //窗口在屏幕的X、Y轴坐标
		,800,800     //窗口的宽度和高度
		,NULL        //所从属的父窗口的窗口句柄
		,NULL        //菜单句柄
		,hInstance   //此窗口所属应用程序实例句柄
		,NULL);     //建立窗体时的一些附带信息
	//5、判断实例化是否成功
	if(hwnd==NULL)
	{
		MessageBox(NULL,"窗口创建失败了！","错误提示",MB_OK);
		return 0;
	}
	//6、显示窗口
	ShowWindow(hwnd,SW_SHOWNORMAL);
	if (MessageBox(hwnd, "是否对Leap Motion进行初始化(需要硬件支持)", "注意", MB_YESNO) == 7)flag = 0;
		while (flag)		//测试Leap
		listener.onFrame(controller);
	listener.onConnect(controller);//enable gestures

	pCurrentData = new PAINTDATA;
	pCurrentData->penStyle = penStyle;
	pCurrentData->ptBeginX = cur_pos[0];
	pCurrentData->ptBeginY = cur_pos[1];
	//7、重绘窗口
	UpdateWindow(hwnd);
	//8、建立消息循环
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	return 0;
}

long __stdcall WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	static CTool *pTool = new CLine(hWnd);//所有图形共用一个基类指针，初始化为画线
	static SYSTEMTIME st;
	static HWND hState = NULL;
	hwnd = hWnd;
	vector<PAINTDATA>::const_iterator item;
	switch(msg)
	{
	case WM_PAINT:
		PAINTSTRUCT ps;
			//将所有线条重新画一遍
			BeginPaint(pTool->m_hWnd, &ps);
			RECT rect;
			GetClientRect(pTool->m_hWnd, &rect);
			if (leapmode) {
				vector<PAINTDATA>::const_iterator item;
				for (item = datas.begin(); item != datas.end(); item++)
				{
					HPEN pen = CreatePen(item->penStyle, CurWidth, CurrentColor);
					SelectObject(ps.hdc, pen);
					MoveToEx(ps.hdc, item->ptBeginX, item->ptBeginY, NULL);
					LineTo(ps.hdc, item->ptEndX, item->ptEndY);
					DeleteObject(pen);
					EndPaint(pTool->m_hWnd, &ps);
					break;
				}
			}
		BitBlt(ps.hdc, 0, 0, rect.right, rect.bottom, pTool->m_hMemDC, 0, 0, SRCCOPY);
		//pTool->OnPaint(ps);
		break;
	case WM_LBUTTONUP:
		if(pTool != NULL)
		{
			pTool->OnLButtonUp(wParam,lParam);
		}		
		break;
	case WM_LBUTTONDOWN:
		if(pTool != NULL)
		{
			pTool->OnLButtonDown(wParam,lParam);
		}				
		break;
	case WM_MOUSEMOVE:
		if(pTool != NULL)
		{
			pTool->OnMouseMove(wParam,lParam);
			InvalidateRect(hWnd,0,false);

			char buffer[256];			
			int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);			
			SendMessage(hState,SB_SETTEXT,2,(LPARAM)buffer);
		}		
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			switcher = 0;
			leapmode = 0;
			break;
		}
		break;
	case WM_COMMAND:
		{
			OPENFILENAME ofName;
			char strFile[256];
			strFile[0]='\0';
			setOPN(&ofName,strFile);
			int nID;
			nID = LOWORD(wParam);
			switch(nID)
			{
			case IDM_LEAPOFF:
				leapmode = 0;
				switcher = 0;
				break;
			case IDM_LEAPON:
				leapmode = 1;
				if(leapmode)switcher = 1;
				else switcher = 0;
				MSG message;
				while (switcher)
				{
					if (::PeekMessage(&message, hWnd ,NULL, NULL, PM_REMOVE))//Leap Mode的出口
					{
						::TranslateMessage(&message);
						::DispatchMessage(&message);
					}
					if (pCurrentData != NULL)
					{
						CurWidth = CTool::m_hPenBody;
						CurrentColor = CTool::m_nColor;
						listener.onFrame(controller);
						//获取终点  
						pCurrentData->ptEndX = cur_pos[0];
						pCurrentData->ptEndY = cur_pos[1];
						//画出线条  
						HDC hdc = GetDC(pTool->m_hWnd);
						HPEN pen = CreatePen(pCurrentData->penStyle, CurWidth, CurrentColor);
						HPEN oldpen = (HPEN)SelectObject(hdc, pen);
						MoveToEx(hdc, pCurrentData->ptBeginX, pCurrentData->ptBeginY, NULL);
						LineTo(hdc, pCurrentData->ptEndX, pCurrentData->ptEndY);
						SelectObject(hdc, oldpen);
						DeleteObject(pen);
						ReleaseDC(pTool->m_hWnd, hdc);
						//把当前数据添加到vector中  
						datas.push_back(*pCurrentData);
						delete pCurrentData;
						pCurrentData = new PAINTDATA;
						pCurrentData->penStyle = penStyle;
						pCurrentData->ptBeginX = cur_pos[0];
						pCurrentData->ptBeginY = cur_pos[1];
						UpdateWindow(pTool->m_hWnd);	//THIS FUNCTION SEND WM_PAINT 
											// Main message loop:
					}

				}
				break;
			case IDM_DrawLine:
				pTool = new CLine(hWnd);
				break;
			case IDM_DrawCircle:
				pTool = new CEllip(hWnd);
				break;
			case IDM_DrawRect:
				pTool = new CRect(hWnd);
				break;
			case IDM_DrawCurve:
				pTool = new CCurve(hWnd);
				break;
			case IDM_DrawTriangle:
				pTool = new CTriangle(hWnd);
				break;
			case IDM_Exit:
				if (MessageBox(hWnd, "是否退出Leap Painter？", "提示", MB_YESNO))PostMessage(hWnd, WM_DESTROY,NULL,NULL);
				break;
			case IDM_Color:
				pTool->Color();
				break;
			case IDM_hPenBody1:
				pTool->hPenBody(1);
				break;
			case IDM_hPenBody2:
				pTool->hPenBody(2);
				break;
			case IDM_hPenBody3:
				pTool->hPenBody(3);
				break;
			case IDT_FileOpen:
				ofName.Flags = OFN_READONLY;
				GetOpenFileName(&ofName);
				pTool->LoadBMP(hInst,strFile);
				break;
			case IDT_FileSave:
				ofName.Flags = OFN_OVERWRITEPROMPT;
				GetSaveFileName(&ofName);
				//SAVE功能未实现
				break;
			case IDM_LEAPMODE:
				MessageBox(hWnd, "Powered by Pythonix.\n(需要硬件支持)\n开启leap模式后使用点击屏幕手势或ESC来终止画图\n关闭Leap模式来回到鼠标模式", "关于", MB_OK);
				break;
			case IDM_LEAP:
				MessageBox(hWnd, "详见 www.leapmotion.com", "关于", MB_OK);
				break;
			case IDM_LEAPPAINTER:
				MessageBox(hWnd, "Version 1.0 \nPowered by Robin,Ray and Pythonix\nCopyright (C) 2015 HHL. All rights reserved. ", "关于", MB_OK);
				break;
			}
		}
		break;
	case WM_DESTROY:
		controller.removeListener(listener);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
	return 0;
}



//设置打开的文件名结构体成员
void setOPN(OPENFILENAME *ofName,char strFile[])
{
	ofName->lStructSize = sizeof(OPENFILENAME);
	ofName->hwndOwner = NULL;    //必需的
	ofName->hInstance = hInst;   //可选的
	ofName->lpstrFilter = "图片文件(*.BMP)\0*.BMP";
	ofName->lpstrFileTitle = NULL;  //必需的
	ofName->lpstrCustomFilter = NULL;
	ofName->nMaxFile = MAX_PATH;    //必需的
	ofName->lpstrFile = strFile;    //必需的
	ofName->lpstrInitialDir = NULL;
	ofName->lpstrTitle = NULL;
	ofName->Flags = 0;      //必需的
	ofName->lCustData = 0L;    //必需的
	ofName->lpstrDefExt = "txt";   //必需的
}