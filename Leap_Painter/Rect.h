// Rect.h: interface for the CRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECT_H__5E85AE19_E705_4D2A_A118_3E1FFFCED5F6__INCLUDED_)
#define AFX_RECT_H__5E85AE19_E705_4D2A_A118_3E1FFFCED5F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tool.h"

class CRect : public CTool  
{
public:
	void Draw();
	CRect(HWND hWnd);
	virtual ~CRect();

};

#endif // !defined(AFX_RECT_H__5E85AE19_E705_4D2A_A118_3E1FFFCED5F6__INCLUDED_)
