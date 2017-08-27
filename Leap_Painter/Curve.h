// Curve.h: interface for the CCurve class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURVE_H__E70F6EB2_34D3_4E56_9891_565900F725F1__INCLUDED_)
#define AFX_CURVE_H__E70F6EB2_34D3_4E56_9891_565900F725F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tool.h"

class CCurve : public CTool  
{
public:
	void Draw();
	CCurve(HWND hWnd);
	virtual ~CCurve();

};

#endif // !defined(AFX_CURVE_H__E70F6EB2_34D3_4E56_9891_565900F725F1__INCLUDED_)
