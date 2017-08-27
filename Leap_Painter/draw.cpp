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
	int ptBeginX, ptBeginY;//���  
	int ptEndX, ptEndY;//�յ�  
	int penStyle;//���ʵ�����
	int CurWidth;
	COLORREF CurrentColor;//������ɫ
} PAINTDATA;
static COLORREF CurrentColor = CTool::m_nColor;
static int CurWidth = CTool::m_hPenBody;
static vector<PAINTDATA> datas;
static int penStyle = PS_SOLID;
static PAINTDATA *pCurrentData = NULL;//ָ��ǰPAINTDATA��ָ��  


//listener & listener
Leap_Listener listener;
Controller controller;

HINSTANCE hInst = NULL;//�����������ʾλͼ����,������ں���ʵ�������������ʵ��
HWND hwnd;//�������ص��������ھ�����˳��Ի���

long __stdcall WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
long CALLBACK DialogProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam);
long CALLBACK DialogProc1(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam);
void setOPN(OPENFILENAME * ofName,char strFile[]);

int __stdcall WinMain(HINSTANCE hInstance,
					  HINSTANCE hPrevInstance,
					  LPSTR lpCmdLine,int nCmdShow)
{
	hInst = hInstance;
	//1�����崰����
	WNDCLASSEX wcls;
	//2����䴰����
	wcls.cbClsExtra=0;  //�������Ϣ����������չ
	wcls.cbSize =sizeof(wcls); //������Ĵ�С
	wcls.cbWndExtra =0;   //�������Ϣ������ʵ����չ
	wcls.hInstance = hInstance;   //ָ����ǰӦ�ó���ʵ�����,Ҳ���ǳ���ǰ�ı�ʶ�š�
	wcls.hbrBackground =(HBRUSH)(COLOR_WINDOW+1);  //���ô��ڵı�����ɫ
	wcls.hCursor = LoadCursor(NULL,IDC_CROSS);//���ù�����ʽ
	wcls.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));  //��ͼ�꣬EXE��ͼ��
	wcls.hIconSm = LoadIcon(wcls.hInstance,MAKEINTRESOURCE(IDI_ICON1));  //Сͼ�꣬���ô��ڱ�������ͼ��
	wcls.lpfnWndProc = WndProc;   //ָ�����ڵĴ�����ΪWndProc��WndProc������windows��Ϣ��
	wcls.lpszClassName = "Leap_Painter";   //��������������
	wcls.lpszMenuName = (char *)IDR_MENU;    //���ô��ڵĲ˵���Ҫ�˵������
	wcls.style = CS_HREDRAW|CS_VREDRAW;       //���������ʽ���
	//3��ע�ᴰ����
	RegisterClassEx(&wcls);
	//4��ʵ�������ڣ��������ڣ�
	HWND hwnd;
	hwnd=CreateWindow("Leap_Painter"    //�����������
		,"Leap Painter"     //���ڵı���
		,WS_OVERLAPPEDWINDOW     //���ڵ���ʽ
		,CW_USEDEFAULT,0        //��������Ļ��X��Y������
		,800,800     //���ڵĿ�Ⱥ͸߶�
		,NULL        //�������ĸ����ڵĴ��ھ��
		,NULL        //�˵����
		,hInstance   //�˴�������Ӧ�ó���ʵ�����
		,NULL);     //��������ʱ��һЩ������Ϣ
	//5���ж�ʵ�����Ƿ�ɹ�
	if(hwnd==NULL)
	{
		MessageBox(NULL,"���ڴ���ʧ���ˣ�","������ʾ",MB_OK);
		return 0;
	}
	//6����ʾ����
	ShowWindow(hwnd,SW_SHOWNORMAL);
	if (MessageBox(hwnd, "�Ƿ��Leap Motion���г�ʼ��(��ҪӲ��֧��)", "ע��", MB_YESNO) == 7)flag = 0;
		while (flag)		//����Leap
		listener.onFrame(controller);
	listener.onConnect(controller);//enable gestures

	pCurrentData = new PAINTDATA;
	pCurrentData->penStyle = penStyle;
	pCurrentData->ptBeginX = cur_pos[0];
	pCurrentData->ptBeginY = cur_pos[1];
	//7���ػ洰��
	UpdateWindow(hwnd);
	//8��������Ϣѭ��
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
	static CTool *pTool = new CLine(hWnd);//����ͼ�ι���һ������ָ�룬��ʼ��Ϊ����
	static SYSTEMTIME st;
	static HWND hState = NULL;
	hwnd = hWnd;
	vector<PAINTDATA>::const_iterator item;
	switch(msg)
	{
	case WM_PAINT:
		PAINTSTRUCT ps;
			//�������������»�һ��
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
					if (::PeekMessage(&message, hWnd ,NULL, NULL, PM_REMOVE))//Leap Mode�ĳ���
					{
						::TranslateMessage(&message);
						::DispatchMessage(&message);
					}
					if (pCurrentData != NULL)
					{
						CurWidth = CTool::m_hPenBody;
						CurrentColor = CTool::m_nColor;
						listener.onFrame(controller);
						//��ȡ�յ�  
						pCurrentData->ptEndX = cur_pos[0];
						pCurrentData->ptEndY = cur_pos[1];
						//��������  
						HDC hdc = GetDC(pTool->m_hWnd);
						HPEN pen = CreatePen(pCurrentData->penStyle, CurWidth, CurrentColor);
						HPEN oldpen = (HPEN)SelectObject(hdc, pen);
						MoveToEx(hdc, pCurrentData->ptBeginX, pCurrentData->ptBeginY, NULL);
						LineTo(hdc, pCurrentData->ptEndX, pCurrentData->ptEndY);
						SelectObject(hdc, oldpen);
						DeleteObject(pen);
						ReleaseDC(pTool->m_hWnd, hdc);
						//�ѵ�ǰ������ӵ�vector��  
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
				if (MessageBox(hWnd, "�Ƿ��˳�Leap Painter��", "��ʾ", MB_YESNO))PostMessage(hWnd, WM_DESTROY,NULL,NULL);
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
				//SAVE����δʵ��
				break;
			case IDM_LEAPMODE:
				MessageBox(hWnd, "Powered by Pythonix.\n(��ҪӲ��֧��)\n����leapģʽ��ʹ�õ����Ļ���ƻ�ESC����ֹ��ͼ\n�ر�Leapģʽ���ص����ģʽ", "����", MB_OK);
				break;
			case IDM_LEAP:
				MessageBox(hWnd, "��� www.leapmotion.com", "����", MB_OK);
				break;
			case IDM_LEAPPAINTER:
				MessageBox(hWnd, "Version 1.0 \nPowered by Robin,Ray and Pythonix\nCopyright (C) 2015 HHL. All rights reserved. ", "����", MB_OK);
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



//���ô򿪵��ļ����ṹ���Ա
void setOPN(OPENFILENAME *ofName,char strFile[])
{
	ofName->lStructSize = sizeof(OPENFILENAME);
	ofName->hwndOwner = NULL;    //�����
	ofName->hInstance = hInst;   //��ѡ��
	ofName->lpstrFilter = "ͼƬ�ļ�(*.BMP)\0*.BMP";
	ofName->lpstrFileTitle = NULL;  //�����
	ofName->lpstrCustomFilter = NULL;
	ofName->nMaxFile = MAX_PATH;    //�����
	ofName->lpstrFile = strFile;    //�����
	ofName->lpstrInitialDir = NULL;
	ofName->lpstrTitle = NULL;
	ofName->Flags = 0;      //�����
	ofName->lCustData = 0L;    //�����
	ofName->lpstrDefExt = "txt";   //�����
}