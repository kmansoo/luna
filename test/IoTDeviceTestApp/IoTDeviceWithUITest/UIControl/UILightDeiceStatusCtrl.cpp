// Control/UILightDeiceStatusCtrl.cpp : implementation file
//

#include "stdafx.h"

#include "UILightDeiceStatusCtrl.h"


// CUILightDeiceStatusCtrl

IMPLEMENT_DYNAMIC(CUILightDeiceStatusCtrl, CStatic)

CUILightDeiceStatusCtrl::CUILightDeiceStatusCtrl()
{
    light_on_ = false;

    img_[0].Load(_T("./image/bulb_off.png"));
    img_[1].Load(_T("./image/bulb_on.png"));

}

CUILightDeiceStatusCtrl::~CUILightDeiceStatusCtrl()
{
}


BEGIN_MESSAGE_MAP(CUILightDeiceStatusCtrl, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()



// CUILightDeiceStatusCtrl message handlers

bool CUILightDeiceStatusCtrl::getStatus() {
    return light_on_;
}

void CUILightDeiceStatusCtrl::setStatus(bool on) {
    bool is_repaint = false;

    if (light_on_ != on)
        is_repaint = true;

    light_on_ = on;

    if (is_repaint)
        Invalidate();
}

void CUILightDeiceStatusCtrl::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CStatic::OnPaint() for painting messages

    CRect rect;
    GetClientRect(rect);

    dc.FillSolidRect(rect, RGB(0, 0, 0));
 
    try {
        img_[light_on_].Draw(dc, CPoint(rect.CenterPoint().x - img_[light_on_].GetWidth() / 2 , rect.CenterPoint().y - img_[light_on_].GetHeight() / 2));
    }
    catch (...) {

    }
}
