#pragma once


// CUILightDeiceStatusCtrl

class CUILightDeiceStatusCtrl : public CStatic
{
	DECLARE_DYNAMIC(CUILightDeiceStatusCtrl)

public:
	CUILightDeiceStatusCtrl();
	virtual ~CUILightDeiceStatusCtrl();

protected:
	DECLARE_MESSAGE_MAP()

    afx_msg void OnPaint();

public:
    bool    getStatus();
    void    setStatus(bool on);

private:
    bool    light_on_;
    CImage  img_[2];
};


