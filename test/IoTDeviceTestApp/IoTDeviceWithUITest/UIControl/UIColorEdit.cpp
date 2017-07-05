// UIColorEdit.cpp : implementation file
//

#include "stdafx.h"

#include "UIColorEdit.h"


// CUIColorEdit

IMPLEMENT_DYNAMIC(CUIColorEdit, CEdit)

CUIColorEdit::CUIColorEdit()
{
	//  default text color
	_crText     = DEF_COLOR_TEXT;
    _crBackGnd  = DEF_COLOR_BACK;
}

CUIColorEdit::~CUIColorEdit()
{
	//delete brush
	if (_brBackGnd.GetSafeHandle())
       _brBackGnd.DeleteObject();
}


BEGIN_MESSAGE_MAP(CUIColorEdit, CEdit)
    ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// CUIColorEdit message handlers
HBRUSH CUIColorEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	//set text color
	pDC->SetTextColor(_crText);

	//set the text's background color
	pDC->SetBkColor(_crBackGnd);

	//return the brush used for background this sets control background
	return _brBackGnd;
}


void CUIColorEdit::SetBackColor(COLORREF rgb)
{
	//set background color ref (used for text's background)

    if (rgb == 0xffffffff)
        _crBackGnd = DEF_COLOR_BACK;
    else
	    _crBackGnd = rgb;
	
	//free brush
	if (_brBackGnd.GetSafeHandle())
       _brBackGnd.DeleteObject();

	//set brush to new color
	_brBackGnd.CreateSolidBrush(rgb);
	
	//redraw
	Invalidate(TRUE);
}

void CUIColorEdit::SetTextColor(COLORREF rgb)
{
	//set text color ref
    if (rgb == 0xffffffff)
        _crText = DEF_COLOR_TEXT;
    else
        _crText = rgb;

	//redraw
	Invalidate(TRUE);
}
