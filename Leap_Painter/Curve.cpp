// Curve.cpp: implementation of the CCurve class.
//
//////////////////////////////////////////////////////////////////////

#include "Curve.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCurve::CCurve(HWND hWnd):CTool(hWnd)
{

}

CCurve::~CCurve()
{

}

void CCurve::Draw()
{
	if(m_state == MouseDown)
	{
		MoveToEx(m_hMemDC,m_begPT.x,m_begPT.y,NULL);
		LineTo(m_hMemDC,m_oldPT.x,m_oldPT.y);
		
		MoveToEx(m_hMemDC,m_begPT.x,m_begPT.y,NULL);
		LineTo(m_hMemDC,m_endPT.x,m_endPT.y);
		m_begPT = m_endPT;
	}
}
