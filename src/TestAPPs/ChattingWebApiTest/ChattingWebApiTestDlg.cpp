
// ChattingWebApiTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChattingWebApiTest.h"
#include "ChattingWebApiTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "ccCoreAPI/ccString.h"
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
    , _strSendMessage(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


    _strServerIP    =   AfxGetApp()->GetProfileString("Config", "IP", "localhost");
    _strServerPort  =   AfxGetApp()->GetProfileString("Config", "Port", "8000");

    _strUserID = AfxGetApp()->GetProfileString("Config", "UserID", "");
    _strUserName = AfxGetApp()->GetProfileString("Config", "UserName", "");

    _strSessionName = AfxGetApp()->GetProfileString("Config", "SessionName", "");
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
    DDX_Text(pDX, IDC_MESSAGE, _strSendMessage);
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
    ON_BN_CLICKED(IDC_SESSION_DELETE, &CChattingWebApiTestDlg::OnBnClickedSessionDelete)
    ON_LBN_DBLCLK(IDC_SESSION_LIST, &CChattingWebApiTestDlg::OnLbnDblclkSessionList)
    ON_WM_TIMER()
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

    //  
    ccWin32WebApiHelper::getInstance()->SetConnectionInfo((LPCTSTR)_strServerIP, atoi(_strServerPort));

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

    AfxGetApp()->WriteProfileString("Config", "IP", _strServerIP);
    AfxGetApp()->WriteProfileString("Config", "Port", _strServerPort);

    ccWin32WebApiHelper::getInstance()->SetConnectionInfo((LPCTSTR)_strServerIP, atoi(_strServerPort));
}

void CChattingWebApiTestDlg::OnBnClickedLogin()
{
    UpdateData();

    AfxGetApp()->WriteProfileString("Config", "UserID", _strUserID);
    AfxGetApp()->WriteProfileString("Config", "UserName", _strUserName);

    std::string strResponse;

    Json::Value oRequestJsonData;
    Json::Value oExtInfo;

    oExtInfo["ID"] = (LPCTSTR)_strUserID;
    oExtInfo["Name"] = (LPCTSTR)_strUserName;

    oRequestJsonData["user_info"] = oExtInfo;

    ccWin32WebApiHelper::getInstance()->RequestAPI(ccWebServerRequest::HttpMethod_Post, "/user", oRequestJsonData, strResponse);

    DoSessionListUpdate();
}


void CChattingWebApiTestDlg::OnBnClickedLogout()
{
    UpdateData();

    std::string strResponse;
    std::string strQueryString;

    ccString::format(strQueryString, "/user?ID=%s", _strUserID);

    ccWin32WebApiHelper::getInstance()->RequestAPI(ccWebServerRequest::HttpMethod_Delete, strQueryString,strResponse);

    _ctlSessionList.ResetContent();
    _ctlSessionMemberList.ResetContent();
    _ctlMessageList.ResetContent();
}


void CChattingWebApiTestDlg::OnBnClickedSessionCreate()
{
    UpdateData();

    AfxGetApp()->WriteProfileString("Config", "SessionName", _strSessionName);

    std::string strResponse;

    Json::Value oRequestJsonData;
    Json::Value oExtInfo;

    oExtInfo["Name"] = (LPCTSTR)_strSessionName;
    oExtInfo["OwnerID"] = (LPCTSTR)_strUserID;

    oRequestJsonData["session_info"] = oExtInfo;

    if (ccWin32WebApiHelper::getInstance()->RequestAPI(ccWebServerRequest::HttpMethod_Post, "/session", oRequestJsonData, strResponse) == 201)
        DoSessionListUpdate();
}

void CChattingWebApiTestDlg::OnBnClickedSessionDelete()
{
    UpdateData();

    std::string strResponse;

    Json::Value oRequestJsonData;
    Json::Value oExtInfo;

    oExtInfo["Name"] = (LPCTSTR)_strSessionName;
    oExtInfo["OwnerID"] = (LPCTSTR)_strUserID;

    oRequestJsonData["session_info"] = oExtInfo;

    if (ccWin32WebApiHelper::getInstance()->RequestAPI(ccWebServerRequest::HttpMethod_Delete, "/session", oRequestJsonData, strResponse) == 202)
        DoSessionListUpdate();
}

void CChattingWebApiTestDlg::DoSessionListUpdate()
{
    std::string strResponse;

    _ctlSessionList.ResetContent();

    if (ccWin32WebApiHelper::getInstance()->RequestAPI(ccWebServerRequest::HttpMethod_Get, "/session", strResponse) == 200)
    {
        Json::Reader    oReader;
        Json::Value     oRootValue;

        if (!oReader.parse(strResponse, oRootValue))
            return;

        if (oRootValue["session_list"].isObject() == false)
            return;

        for (size_t nIndex = 0; nIndex < oRootValue["session_list"]["count"].asInt(); nIndex++)
        {
            Json::Value oInfoValue = oRootValue["session_list"]["info"][nIndex];

            _ctlSessionList.AddString(oInfoValue["Name"].asString().c_str());
        }
    }
}

void CChattingWebApiTestDlg::DoSessionMemberListUpdate()
{
    _ctlSessionMemberList.ResetContent();

    std::string strResponse;
    std::string strQueryString;

    ccString::format(strQueryString, "/session/member?Name=%s&UserID=%s", _strJoinedSessionName, _strUserID);

    if (ccWin32WebApiHelper::getInstance()->RequestAPI(ccWebServerRequest::HttpMethod_Get, strQueryString, strResponse) == 200)
    {
        Json::Reader    oReader;
        Json::Value     oRootValue;
        Json::Value     oMemberListValue;

        if (!oReader.parse(strResponse, oRootValue))
            return;

        oMemberListValue = oRootValue["session_member_list"];

        if (oMemberListValue.isObject() == false)
            return;

        for (size_t nIndex = 0; nIndex < oMemberListValue["count"].asInt(); nIndex++)
        {
            Json::Value oInfoValue = oMemberListValue["info"][nIndex];

            _ctlSessionMemberList.AddString(oInfoValue["ID"].asString().c_str());
        }
    }
}

void CChattingWebApiTestDlg::OnBnClickedSessionListUpdate()
{
    DoSessionListUpdate();
}


void CChattingWebApiTestDlg::OnBnClickedSessionJoin()
{
    UpdateData();

    std::string strResponse;
    std::string strQueryString;

    ccString::format(strQueryString, "/session/member?Name=%s&UserID=%s", _strSessionName, _strUserID);

    if (ccWin32WebApiHelper::getInstance()->RequestAPI(ccWebServerRequest::HttpMethod_Put, strQueryString, strResponse) == 200)
    {
        _strJoinedSessionName = _strSessionName;

        UpdateData(false);

        DoSessionMemberListUpdate();

        ccString strSessionUri;

        ccString::format(strSessionUri, "ws://%s:%d/session/chat/%s", _strServerIP, atoi(_strServerPort), _strSessionName);

        _pWSClient.reset(easywsclient::WebSocket::from_url(strSessionUri));

        SetTimer(0x100, 50, NULL);
    }
}

void CChattingWebApiTestDlg::OnBnClickedSessionLeave()
{
    if (_pWSClient)
    {
        _pWSClient->close();
        _pWSClient = NULL;
    }

    UpdateData();

    std::string strResponse;
    std::string strQueryString;

    ccString::format(strQueryString, "/session/member?Name=%s&UserID=%s", _strSessionName, _strUserID);

    if (ccWin32WebApiHelper::getInstance()->RequestAPI(ccWebServerRequest::HttpMethod_Delete, strQueryString, strResponse) == 200)
    {

    }

    _strJoinedSessionName = "";
    UpdateData(false);

    _ctlSessionMemberList.ResetContent();
    _ctlMessageList.ResetContent();
}


void CChattingWebApiTestDlg::OnBnClickedSessionMemberUpdate()
{
    DoSessionMemberListUpdate();
}

void CChattingWebApiTestDlg::OnBnClickedMessageSend()
{
    //  UpdateData();
    GetDlgItem(IDC_MESSAGE)->GetWindowText(_strSendMessage);

    std::string strSendString;
    ccString::format(strSendString, "%s: %s", _strUserID, _strSendMessage);

    if (_pWSClient)
        _pWSClient->send(strSendString);

    GetDlgItem(IDC_MESSAGE)->SetWindowText(CString(""));
    GetDlgItem(IDC_MESSAGE)->SetFocus();
}

void CChattingWebApiTestDlg::OnLbnDblclkSessionList()
{
    UpdateData();

    _ctlSessionList.GetText(_ctlSessionList.GetCurSel(), _strSessionName);

    AfxGetApp()->WriteProfileString("Config", "SessionName", _strSessionName);

    UpdateData(false);
}

void CChattingWebApiTestDlg::DoWebsocketCleintHandleMessage(const std::string & message)
{
    _ctlMessageList.AddString(message.c_str());
    _ctlMessageList.SetTopIndex(_ctlMessageList.GetCount() - 1);
}

void CChattingWebApiTestDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
    case 0x100:
        if (_pWSClient)
        {
            if (_pWSClient->getReadyState() != easywsclient::WebSocket::CLOSED)
            {
                _pWSClient->poll(1);

#if __cplusplus > 199711L || defined(WIN32)
                _pWSClient->dispatch(std::bind(&CChattingWebApiTestDlg::DoWebsocketCleintHandleMessage, this, std::placeholders::_1));
#else
                _pWSClient->dispatch(easywsclient::DispatchFunction(&CChattingWebApiTestDlg::DoWebsocketCleintHandleMessage, this));
#endif
            }
        }
        break;
    }

    CDialogEx::OnTimer(nIDEvent);
}
