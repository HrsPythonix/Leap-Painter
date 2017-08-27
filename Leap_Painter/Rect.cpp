// Rect.cpp: implementation of the CRect class.
//
//////////////////////////////////////////////////////////////////////

#include "Rect.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRect::CRect(HWND hWnd):CTool(hWnd)
{

}

CRect::~CRect()
{

}

void CRect::Draw()
{	
	if(m_state == MouseDown)
	{
		Rectangle(m_hMemDC,m_begPT.x,m_begPT.y,m_oldPT.x,m_oldPT.y);
		Rectangle(m_hMemDC,m_begPT.x,m_begPT.y,m_endPT.x,m_endPT.y);
	}
}
