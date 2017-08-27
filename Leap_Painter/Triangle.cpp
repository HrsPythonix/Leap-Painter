// Triangle.cpp: implementation of the CTriangle class.
//
//////////////////////////////////////////////////////////////////////

#include "Triangle.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CTriangle::m_secbl;

CTriangle::CTriangle(HWND hWnd):CTool(hWnd)
{

}

CTriangle::~CTriangle()
{

}

void CTriangle::Draw()
{
	if(m_state == MouseDown)
	{
		m_secbl = false;
	}
	MoveToEx(m_hMemDC,m_begPT.x,m_begPT.y,NULL);
	LineTo(m_hMemDC,m_oldPT.x,m_oldPT.y);
	if(m_state == MouseSecondDown)
	{
		LineTo(m_hMemDC,m_startPT.x,m_startPT.y);
		if(m_secbl==false)
		{
			LineTo(m_hMemDC,m_begPT.x,m_begPT.y);
			m_secbl = true;
		}
	}	
	MoveToEx(m_hMemDC,m_begPT.x,m_begPT.y,NULL);
	LineTo(m_hMemDC,m_endPT.x,m_endPT.y);
	if(m_state == MouseSecondDown)
	{
		LineTo(m_hMemDC,m_startPT.x,m_startPT.y);
	}
}