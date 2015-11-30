
// ChattingWebApiTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChattingWebApiTest.h"
#include "ChattingWebApiTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "ccWin32API/ccWin32WebApiHelper.h"

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


// CChattingWebApiTestDlg dialog



CChattingWebApiTestDlg::CChattingWebApiTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHATTINGWEBAPITEST_DIALOG, pParent)
    , _strUserID(_T(""))
    , _strUserName(_T(""))
    , _strSessionName(_T(""))
    , _strJoinedSessionName(_T(""))
    , _strServerIP(_T(""))
    , _strServerPort(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChattingWebApiTestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_USER_ID, _strUserID);
    DDX_Text(pDX, IDC_USER_NAME, _strUserName);
    DDX_Text(pDX, IDC_SESSION_NAME, _strSessionName);
    DDX_Control(pDX, IDC_SESSION_LIST, _ctlSessionList);
    DDX_Text(pDX, IDC_JOINED_SESSION_NAME, _strJoinedSessionName);
    DDX_Control(pDX, IDC_SESSION_MEMBER_LIST, _ctlSessionMemberList);
    DDX_Control(pDX, IDC_MESSAGE_LIST, _ctlMessageList);
    DDX_Text(pDX, IDC_SERVER_IP, _strServerIP);
    DDX_Text(pDX, IDC_SERVER_PORT, _strServerPort);
}

BEGIN_MESSAGE_MAP(CChattingWebApiTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDCANCEL, &CChattingWebApiTestDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_MESSAGE_SEND, &CChattingWebApiTestDlg::OnBnClickedMessageSend)
    ON_BN_CLICKED(IDC_SESSION_LEAVE, &CChattingWebApiTestDlg::OnBnClickedSessionLeave)
    ON_BN_CLICKED(IDC_SESSION_MEMBER_UPDATE, &CChattingWebApiTestDlg::OnBnClickedSessionMemberUpdate)
    ON_BN_CLICKED(IDC_LOGIN, &CChattingWebApiTestDlg::OnBnClickedLogin)
    ON_BN_CLICKED(IDC_LOGOUT, &CChattingWebApiTestDlg::OnBnClickedLogout)
    ON_BN_CLICKED(IDC_SESSION_CREATE, &CChattingWebApiTestDlg::OnBnClickedSessionCreate)
    ON_BN_CLICKED(IDC_SESSION_LIST_UPDATE, &CChattingWebApiTestDlg::OnBnClickedSessionListUpdate)
    ON_BN_CLICKED(IDC_SESSION_JOIN, &CChattingWebApiTestDlg::OnBnClickedSessionJoin)
    ON_BN_CLICKED(IDC_SERVER_INFO_UPDATE, &CChattingWebApiTestDlg::OnBnClickedServerInfoUpdate)
END_MESSAGE_MAP()


// CChattingWebApiTestDlg message handlers

BOOL CChattingWebApiTestDlg::OnInitDialog()
{
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChattingWebApiTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChattingWebApiTestDlg::OnPaint()
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
HCURSOR CChattingWebApiTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChattingWebApiTestDlg::OnBnClickedCancel()
{

}

void CChattingWebApiTestDlg::OnBnClickedServerInfoUpdate()
{
    UpdateData();

    ccWin32WebApiHelper::getInstance()->SetConnectionInfo((LPCTSTR)_strServerIP, atoi(_strServerPort));
}

void CChattingWebApiTestDlg::OnBnClickedMessageSend()
{
    // TODO: Add your control notification handler code here
}


void CChattingWebApiTestDlg::OnBnClickedSessionLeave()
{
    // TODO: Add your control notification handler code here
}


void CChattingWebApiTestDlg::OnBnClickedSessionMemberUpdate()
{
    // TODO: Add your control notification handler code here
}


void CChattingWebApiTestDlg::OnBnClickedLogin()
{
    UpdateData();

    std::string strResponse;

    Json::Value oRequestRPC;
    Json::Value oUserInfo;
    Json::StyledWriter oWriter;

    oRequestRPC["jsonrpc"] = "2.0";
    oRequestRPC["id"] = 1;

    oUserInfo["ID"] = (LPCTSTR)_strUserID;
    oUserInfo["Name"] = (LPCTSTR)_strUserName;

    oRequestRPC["user_info"] = oUserInfo;

    ccWin32WebApiHelper::getInstance()->RequestAPI(ccWebServerRequest::HttpMethod_Post, "/user", oRequestRPC, strResponse);
}


void CChattingWebApiTestDlg::OnBnClickedLogout()
{
    UpdateData();

    // TODO: Add your control notification handler code here
}


void CChattingWebApiTestDlg::OnBnClickedSessionCreate()
{
    // TODO: Add your control notification handler code here
}


void CChattingWebApiTestDlg::OnBnClickedSessionListUpdate()
{
    // TODO: Add your control notification handler code here
}


void CChattingWebApiTestDlg::OnBnClickedSessionJoin()
{
    // TODO: Add your control notification handler code here
}

