// Ellip.h: interface for the CEllip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELLIP_H__06090CF0_55C1_4033_8580_C089DE89B6A7__INCLUDED_)
#define AFX_ELLIP_H__06090CF0_55C1_4033_8580_C089DE89B6A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tool.h"

class CEllip : public CTool  
{
public:
	void Draw();
	CEllip(HWND hWnd);
	virtual ~CEllip();

};

#endif // !defined(AFX_ELLIP_H__06090CF0_55C1_4033_8580_C089DE89B6A7__INCLUDED_)
