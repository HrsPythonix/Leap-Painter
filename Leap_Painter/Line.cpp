// Line.cpp: implementation of the CLine class.
//
//////////////////////////////////////////////////////////////////////

#include "Line.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLine::CLine(HWND hWnd):CTool(hWnd)
{

}

CLine::~CLine()
{

}

void CLine::Draw()
{	
	if(m_state == MouseDown)
	{
		MoveToEx(m_hMemDC,m_begPT.x,m_begPT.y,NULL);
		LineTo(m_hMemDC,m_oldPT.x,m_oldPT.y);
		
		MoveToEx(m_hMemDC,m_begPT.x,m_begPT.y,NULL);
		LineTo(m_hMemDC,m_endPT.x,m_endPT.y);
	}
}







