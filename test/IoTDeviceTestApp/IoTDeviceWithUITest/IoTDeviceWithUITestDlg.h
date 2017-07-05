
// IoTDeviceWithUITestDlg.h : header file
//

#pragma once

#include "afxwin.h"
#include "UIControl/UILightDeiceStatusCtrl.h"

#include "ccIoTDevice/ccIoTDevice.h"

// CIoTDeviceWithUITestDlg dialog
class CIoTDeviceWithUITestDlg : 
    public CDialogEx,
    public ccIoTDevice
{
// Construction
public:
	CIoTDeviceWithUITestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IOTDEVICEWITHUITEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedRegister();
    afx_msg void OnBnClickedLightOn();
    afx_msg void OnBnClickedLightOff();

protected:
    virtual bool    set_device_command(ccIoTDeviceProtocol& oProtocol);
    virtual bool    get_device_status_command(ccIoTDeviceProtocol& oProtocol);

public:
    void    setBlink(bool on);

protected:
    enum TimerID {
        kTimerID_BlinkLight = 0x100,

    };
    
    CUILightDeiceStatusCtrl light_image_ctrl_;

    bool light_status_;
    bool is_register_;

public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    CString home_name_;
    CString room_name_;
    CString device_name_;
    CString register_status_;
};
