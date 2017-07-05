
// IoTDeviceWithUITestDlg.cpp : implementation file
//

#include "stdafx.h"

#include <algorithm>

#include "IoTDeviceWithUITest.h"
#include "IoTDeviceWithUITestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CIoTDeviceWithUITestDlg dialog



CIoTDeviceWithUITestDlg::CIoTDeviceWithUITestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IOTDEVICEWITHUITEST_DIALOG, pParent)
    , ccIoTDevice("LightDeviceInfo.json")
    , home_name_(_T(""))
    , room_name_(_T(""))
    , device_name_(_T(""))
    , register_status_(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIoTDeviceWithUITestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_IMAGE, light_image_ctrl_);
    DDX_Text(pDX, IDC_HOME_NAME, home_name_);
    DDX_Text(pDX, IDC_ROOM_NAME, room_name_);
    DDX_Text(pDX, IDC_DEVICE_NAME, device_name_);
    DDX_Text(pDX, IDC_REGISTER_STATUS, register_status_);
}

BEGIN_MESSAGE_MAP(CIoTDeviceWithUITestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDCANCEL, &CIoTDeviceWithUITestDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_REGISTER, &CIoTDeviceWithUITestDlg::OnBnClickedRegister)
    ON_BN_CLICKED(IDC_LIGHT_ON, &CIoTDeviceWithUITestDlg::OnBnClickedLightOn)
    ON_BN_CLICKED(IDC_LIGHT_OFF, &CIoTDeviceWithUITestDlg::OnBnClickedLightOff)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CIoTDeviceWithUITestDlg message handlers

BOOL CIoTDeviceWithUITestDlg::OnInitDialog()
{
    this->home_name_ = ccIoTDevice::get_info().get_device_specification(0).get_item_text("HomeName").c_str();
    this->room_name_ = ccIoTDevice::get_info().get_device_specification(0).get_item_text("RoomName").c_str();
    this->device_name_ = ccIoTDevice::get_info().get_device_specification(0).get_name().c_str();

	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    light_status_ = false;
    is_register_ = false;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIoTDeviceWithUITestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIoTDeviceWithUITestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIoTDeviceWithUITestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CIoTDeviceWithUITestDlg::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    // CDialogEx::OnCancel();
}


void CIoTDeviceWithUITestDlg::OnBnClickedRegister()
{
    is_register_ = !is_register_;

    if (is_register_) {
        GetDlgItem(IDC_REGISTER)->SetWindowText(_T("Stop"));
    
        register_status_ = _T("");
        UpdateData(FALSE);

        SetTimer(0x10, 100, NULL);

        ccIoTDevice::start();
    }
    else {
        KillTimer(0x10);
        setBlink(false);

        GetDlgItem(IDC_REGISTER)->SetWindowText(_T("Register"));

        ccIoTDevice::stop();

        register_status_ = _T("");
        UpdateData(FALSE);
    }
}

void CIoTDeviceWithUITestDlg::OnBnClickedLightOn()
{
    light_status_ = true;

    light_image_ctrl_.setStatus(light_status_);
}


void CIoTDeviceWithUITestDlg::OnBnClickedLightOff()
{
    light_status_ = false;

    light_image_ctrl_.setStatus(light_status_);
}

void CIoTDeviceWithUITestDlg::setBlink(bool on) {
    if (on)
        SetTimer(kTimerID_BlinkLight, 250, NULL);
    else {
        KillTimer(kTimerID_BlinkLight);

        light_image_ctrl_.setStatus(light_status_);
    }
}

void CIoTDeviceWithUITestDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    switch (nIDEvent) {
    case 0x10:
        if (ccIoTDevice::is_registered()) {
            KillTimer(0x10);

            register_status_ = _T("REGISTERED");
            UpdateData(FALSE);
            break;
        }
        break;

    case kTimerID_BlinkLight:
        light_image_ctrl_.setStatus(!light_image_ctrl_.getStatus());
        break;
    }

    CDialogEx::OnTimer(nIDEvent);
}

bool CIoTDeviceWithUITestDlg::set_device_command(ccIoTDeviceProtocol& oProtocol) {
    if (oProtocol.ext_info_.isNull())
        return false;

    if (ccIoTDevice::has_device(oProtocol.ext_info_["DeviceType"].asString()) == false)
        return false;

    std::string control = oProtocol.ext_info_["Control"].asString();

    std::transform(control.begin(), control.end(), control.begin(), ::tolower);

    if (control == "on")
        OnBnClickedLightOn();

    if (control == "off")
        OnBnClickedLightOff();

    if (control == "blink_start")
        setBlink(true);

    if (control == "blink_stop")
        setBlink(false);

    return true;
}

bool CIoTDeviceWithUITestDlg::get_device_status_command(ccIoTDeviceProtocol& oProtocol) {
    return false;
}