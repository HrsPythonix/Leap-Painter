// Line.h: interface for the CLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE_H__050E7288_D76A_4299_BEAE_67FB913D008B__INCLUDED_)
#define AFX_LINE_H__050E7288_D76A_4299_BEAE_67FB913D008B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tool.h"

class CLine : public CTool  
{
public:
	void Draw();
	CLine(HWND hWnd);
	virtual ~CLine();
};

#endif // !defined(AFX_LINE_H__050E7288_D76A_4299_BEAE_67FB913D008B__INCLUDED_)
