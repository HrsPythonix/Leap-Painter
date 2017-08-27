// Ellip.cpp: implementation of the CEllip class.
//
//////////////////////////////////////////////////////////////////////

#include "Ellip.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEllip::CEllip(HWND hWnd):CTool(hWnd)
{

}

CEllip::~CEllip()
{

}

void CEllip::Draw()
{
	if(m_state == MouseDown)
	{
		Ellipse(m_hMemDC,m_begPT.x,m_begPT.y,m_oldPT.x,m_oldPT.y);
		Ellipse(m_hMemDC,m_begPT.x,m_begPT.y,m_endPT.x,m_endPT.y);
	}
}
