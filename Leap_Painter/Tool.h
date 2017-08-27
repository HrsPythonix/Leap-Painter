// Tool.h: interface for the CTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOL_H__94669C7D_7D02_417B_8AE6_8B846431EE82__INCLUDED_)
#define AFX_TOOL_H__94669C7D_7D02_417B_8AE6_8B846431EE82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include<windows.h>
#include "resource.h"
#include <commctrl.h>
#include <stdio.h>
class CTool  
{
public:
	enum STATE{MouseUp,MouseDown,MouseSecondDown};//自定义枚举记录鼠标状态
protected:
public:
	static HWND hToolbar;
	static int m_nWidth, m_nHeight;//程序窗口长宽
	static int m_hPenBody;//画笔宽度
	static int m_nColor;//画笔颜色
	POINT m_startPT;
	//只在画三角形时用到，记录第二顶点起点(通常此属性在三角形子类中定义更好。本例中按常规作法将使传参更复杂，更容易出问题)
	POINT m_begPT;//作画时画笔起点
	POINT m_endPT;//终点
	POINT m_oldPT;//临时点，用以解决残留图象问题
	STATE m_state;
	static HDC m_hMemDC;//通过内存设备上下文（句柄）解决画面闪烁问题
	static bool m_bl;
	HWND m_hWnd;
	void LoadBMP(HINSTANCE hInst,char cA[]);
	void hPenBody(int nA);//自定义更改画笔线宽函数
	void Color();	//自定义更改画笔颜色函数
	void OnPaint(PAINTSTRUCT & ps);//实现显示图象的基本功能
	virtual void Draw();//此虚函数包含下面三个函数，是自定义画图函数
	void OnMouseMove(WPARAM wParam,LPARAM lParam);//
	void OnLButtonUp(WPARAM wParam,LPARAM lParam);//
	void OnLButtonDown(WPARAM wParam,LPARAM lParam);//画图主要工作在此三个函数中进行，通过在鼠标三种不同状态时修改类的各属性实现画图
	CTool(HWND hWnd);
	virtual ~CTool();
};

#endif // !defined(AFX_TOOL_H__94669C7D_7D02_417B_8AE6_8B846431EE82__INCLUDED_)
