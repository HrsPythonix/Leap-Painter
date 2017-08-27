// Tool.cpp: implementation of the CTool class.
//ͼ�λ���
//////////////////////////////////////////////////////////////////////
#include "Tool.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
HDC CTool::m_hMemDC;
bool CTool::m_bl;
int CTool::m_nColor;
int CTool::m_hPenBody = 1;//��ʼ�������߿�
int CTool::m_nWidth = GetSystemMetrics(SM_CXSCREEN);
int CTool::m_nHeight = GetSystemMetrics(SM_CYSCREEN);

CTool::CTool(HWND hWnd):m_hWnd(hWnd)
{
	m_state = MouseUp;//��ʼ�����״̬Ϊ������δ����
	m_begPT.x = 0;
	m_begPT.y = 0;//��ʼ���������Ϊ��Ļ���Ͻ�
	m_startPT = m_endPT = m_oldPT = m_begPT;//��ʼ����������ʱ����һ�Ͷ����������
	if(m_bl==false)
	{
		HDC hDC = GetDC(m_hWnd);
		m_hMemDC = CreateCompatibleDC(hDC);

		HBITMAP hBitmap = CreateCompatibleBitmap(hDC,m_nWidth,m_nHeight);
		SelectObject(m_hMemDC,hBitmap);
		PatBlt(m_hMemDC,0,0,m_nWidth,m_nHeight,WHITENESS);

		DeleteObject(hBitmap);
		ReleaseDC(m_hWnd,hDC);
		m_bl=true;
	}
}

CTool::~CTool()
{
	if(m_hMemDC!=NULL)
		delete m_hMemDC;
	m_hMemDC = NULL;
}

void CTool::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	m_begPT.x = LOWORD(lParam);
	m_begPT.y = HIWORD(lParam);
	m_oldPT = m_endPT = m_begPT;
	m_state = (m_state == MouseDown ? MouseSecondDown : MouseDown);
	if(m_state == MouseDown)
	{
		m_startPT = m_begPT;
	}
}

void CTool::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	m_state = (m_state == MouseDown ? MouseDown : MouseUp);
}

void CTool::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	if(m_state!=MouseUp)
	{
		m_endPT.x = LOWORD(lParam);
		m_endPT.y = HIWORD(lParam);
		HPEN hPen = CreatePen(0, m_hPenBody, m_nColor^GetBkColor(m_hMemDC));
		SelectObject(m_hMemDC,hPen);
		HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		SelectObject(m_hMemDC, hBrush);
		SetROP2(m_hMemDC, R2_XORPEN);
		Draw();
		DeleteObject(hPen);
		DeleteObject(hBrush);
		m_oldPT = m_endPT;
	}
}

void CTool::Draw()
{
}

void CTool::OnPaint(PAINTSTRUCT & ps)
{
    BeginPaint(m_hWnd, &ps);
	RECT rect;
	GetClientRect(m_hWnd,&rect);
	BitBlt(ps.hdc,0,0,rect.right,rect.bottom,m_hMemDC,0,0,SRCCOPY);
	EndPaint(m_hWnd,&ps);
}

void CTool::Color()
{
	COLORREF CustColor[16];   //������ɫ����
	CHOOSECOLOR cColor;   //������ɫ�ṹ��
	cColor.Flags = CC_FULLOPEN | CC_RGBINIT;   //ָ����ʾʲô���ĶԻ���
	cColor.hwndOwner = m_hWnd;    //�Ի���ӵ���ߴ��ھ��
	cColor.lpCustColors = (LPDWORD)CustColor;
	cColor.lStructSize = sizeof(CHOOSECOLOR);
	//����API��ʾ�Ի���
	if(ChooseColor(&cColor))     //�������ȷ��
	{
		m_nColor = cColor.rgbResult;
	}
}

void CTool::hPenBody(int nA)
{
	m_hPenBody = nA;
}

void CTool::LoadBMP(HINSTANCE hInst,char cA[])
{
	HDC hDC = GetDC(m_hWnd);
	m_hMemDC = CreateCompatibleDC(hDC);

	HBITMAP hBitmap=(HBITMAP)LoadImage(hInst,cA,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	SelectObject(m_hMemDC,hBitmap);
	BitBlt(hDC,0,0,m_nWidth,m_nHeight,m_hMemDC,0,0,SRCCOPY);
	
	DeleteObject(hBitmap);
	ReleaseDC(m_hWnd,hDC);
}



