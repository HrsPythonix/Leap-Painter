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
	enum STATE{MouseUp,MouseDown,MouseSecondDown};//�Զ���ö�ټ�¼���״̬
protected:
public:
	static HWND hToolbar;
	static int m_nWidth, m_nHeight;//���򴰿ڳ���
	static int m_hPenBody;//���ʿ��
	static int m_nColor;//������ɫ
	POINT m_startPT;
	//ֻ�ڻ�������ʱ�õ�����¼�ڶ��������(ͨ���������������������ж�����á������а�����������ʹ���θ����ӣ������׳�����)
	POINT m_begPT;//����ʱ�������
	POINT m_endPT;//�յ�
	POINT m_oldPT;//��ʱ�㣬���Խ������ͼ������
	STATE m_state;
	static HDC m_hMemDC;//ͨ���ڴ��豸�����ģ���������������˸����
	static bool m_bl;
	HWND m_hWnd;
	void LoadBMP(HINSTANCE hInst,char cA[]);
	void hPenBody(int nA);//�Զ�����Ļ����߿���
	void Color();	//�Զ�����Ļ�����ɫ����
	void OnPaint(PAINTSTRUCT & ps);//ʵ����ʾͼ��Ļ�������
	virtual void Draw();//���麯�����������������������Զ��廭ͼ����
	void OnMouseMove(WPARAM wParam,LPARAM lParam);//
	void OnLButtonUp(WPARAM wParam,LPARAM lParam);//
	void OnLButtonDown(WPARAM wParam,LPARAM lParam);//��ͼ��Ҫ�����ڴ����������н��У�ͨ����������ֲ�ͬ״̬ʱ�޸���ĸ�����ʵ�ֻ�ͼ
	CTool(HWND hWnd);
	virtual ~CTool();
};

#endif // !defined(AFX_TOOL_H__94669C7D_7D02_417B_8AE6_8B846431EE82__INCLUDED_)
