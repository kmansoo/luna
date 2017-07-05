#pragma once


// CUIColorEdit

class CUIColorEdit : public CEdit
{
	DECLARE_DYNAMIC(CUIColorEdit)

public:
	CUIColorEdit();
	virtual ~CUIColorEdit();

public:
	void SetTextColor(COLORREF rgb = 0xffffffff);
	void SetBackColor(COLORREF rgb = 0xffffffff);

public:
    enum DEFAULT_COLOR {
        DEF_COLOR_TEXT  =   RGB(0, 0, 0),
        DEF_COLOR_BACK  =   RGB(255, 255, 255),
    };

protected:
	DECLARE_MESSAGE_MAP()

protected:
	//text and text background colors
	COLORREF    _crText;
	COLORREF    _crBackGnd;

	//background brush
	CBrush      _brBackGnd;
public:
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
};


