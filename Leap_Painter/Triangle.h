// Triangle.h: interface for the CTriangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIANGLE_H__F6122704_BDFB_4659_892B_606A7B9731C4__INCLUDED_)
#define AFX_TRIANGLE_H__F6122704_BDFB_4659_892B_606A7B9731C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tool.h"

class CTriangle : public CTool  
{
public:
	void Draw();
	CTriangle(HWND hWnd);
	virtual ~CTriangle();

protected:
	static bool m_secbl;
};

#endif // !defined(AFX_TRIANGLE_H__F6122704_BDFB_4659_892B_606A7B9731C4__INCLUDED_)
