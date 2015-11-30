
// WebApiTestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "WebApiTest.h"
#include "WebApiTestDlg.h"
#include "afxdialogex.h"

#include <afxinet.h>  

#include "ccWin32API/ccWin32WebApiHelper.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // 대화 상자 데이터입니다.
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    // 구현입니다.
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWebApiTestDlg 대화 상자




CWebApiTestDlg::CWebApiTestDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CWebApiTestDlg::IDD, pParent)
    , _strRequestData(_T(""))
    , _strResponseData(_T(""))
    , _strServerIP(_T(""))
    , _uServerPort(0)
    , _strWebAPI(_T(""))
    , _strMethod(_T(""))
    , _uStatusCode(0)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWebApiTestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_REQUEST_DATA, _strRequestData);
    DDX_Text(pDX, IDC_RESPONSE_DATA, _strResponseData);

    DDX_Text(pDX, IDC_SERVER_IP, _strServerIP);
    DDX_Text(pDX, IDC_SERVER_PORT, _uServerPort);
    DDX_Text(pDX, IDC_WEB_API, _strWebAPI);
    DDX_CBString(pDX, IDC_METHOD, _strMethod);
    DDX_Text(pDX, IDC_STATUS_CODE, _uStatusCode);
}

BEGIN_MESSAGE_MAP(CWebApiTestDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_REQUEST_API, &CWebApiTestDlg::OnBnClickedRequestAPI)
    //  ON_BN_CLICKED(IDC_VERIFY, &CWebApiTestDlg::OnBnClickedVerify)
    ON_EN_CHANGE(IDC_REQUEST_DATA, &CWebApiTestDlg::OnEnChangeRequestData)
END_MESSAGE_MAP()


// CWebApiTestDlg 메시지 처리기

BOOL CWebApiTestDlg::OnInitDialog()
{
    _strServerIP    =   AfxGetApp()->GetProfileString(_T("Config"), _T("ServerIP"), _T("localhost"));
    _uServerPort    =   AfxGetApp()->GetProfileInt(_T("Config"), _T("ServerPort"), 8000);

    _strRequestData =   AfxGetApp()->GetProfileString(_T("Config"), _T("RequestData"), _T(""));
    _strMethod      =   AfxGetApp()->GetProfileString(_T("Config"), _T("Method"), _T("GET"));
    _strWebAPI      = AfxGetApp()->GetProfileString(_T("Config"), _T("WebAPI"), _T(""));

    CDialogEx::OnInitDialog();

    // 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

    // IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

    // 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
    //  프레임워크가 이 작업을 자동으로 수행합니다.
    SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
    SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

    // TODO: 여기에 추가 초기화 작업을 추가합니다.

    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CWebApiTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CWebApiTestDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 아이콘을 그립니다.
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CWebApiTestDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CWebApiTestDlg::DoUpdateConfiguration()
{
    UpdateData();

    AfxGetApp()->WriteProfileString(_T("Config"), _T("ServerIP"), _strServerIP);
    AfxGetApp()->WriteProfileInt(_T("Config"), _T("ServerPort"), _uServerPort);

    AfxGetApp()->WriteProfileString(_T("Config"), _T("RequestData"),   _strRequestData);
    AfxGetApp()->WriteProfileString(_T("Config"), _T("Method"),   _strMethod);
    AfxGetApp()->WriteProfileString(_T("Config"), _T("WebAPI"),  _strWebAPI);

    ccWin32WebApiHelper::getInstance()->SetConnectionInfo((LPCTSTR)_strServerIP, _uServerPort);
}

void CWebApiTestDlg::OnTransactionRecveResponse(std::uint16_t uStatusCode, const std::string& strResponse)
{
    if (!IsWindow(m_hWnd))
        return;

    Json::Value     oValue;
    Json::Reader    oReader;

    std::string strData = strResponse;

    if (oReader.parse(strData, oValue) == false)
        OnTransactionRequestTimeout();
}

void CWebApiTestDlg::OnTransactionRequestTimeout()
{
    if (!IsWindow(m_hWnd))
        return;
}

BOOL CWebApiTestDlg::DoRequestAPI()
{
    UpdateData();

    _strResponseData = "";

    std::string     strResponse;

    ccWebServerRequest::HttpMethod eMethod;

    if (_strMethod == "GET")
        eMethod = ccWebServerRequest::HttpMethod_Get;

    if (_strMethod == "POST")
        eMethod = ccWebServerRequest::HttpMethod_Post;

    if (_strMethod == "PUT")
        eMethod = ccWebServerRequest::HttpMethod_Put;

    if (_strMethod == "DELETE")
        eMethod = ccWebServerRequest::HttpMethod_Delete;


    if (_strResponseData.GetLength())
    {
        Json::Value     oParams;
        Json::Reader    oRequestReader;

        if (oRequestReader.parse((const char *)_strRequestData, oParams) == false)
        {
            MessageBox("JCON Parser Error!", "Warring", MB_ICONWARNING);
            UpdateData(FALSE);
            return FALSE;
        }

        if ((_uStatusCode = ccWin32WebApiHelper::getInstance()->RequestAPI(eMethod, (LPCTSTR)_strWebAPI, oParams, strResponse)))
        {
            _strResponseData = strResponse.c_str();
            _strResponseData.Replace("\n", "\r\n");
        }
        else
            MessageBox("Could not connect to Web API Server!", "Warring", MB_ICONWARNING);
    }
    else
    {
        if ((_uStatusCode = ccWin32WebApiHelper::getInstance()->RequestAPI(eMethod, (LPCTSTR)_strWebAPI, strResponse)))
        {
            _strResponseData = strResponse.c_str();
            _strResponseData.Replace("\n", "\r\n");
        }
        else
            MessageBox("Could not connect to Web API Server!", "Warring", MB_ICONWARNING);
    }

    UpdateData(FALSE);

    return TRUE;
}

void CWebApiTestDlg::OnBnClickedRequestAPI()
{
    DoUpdateConfiguration();

    DoRequestAPI();
}


void CWebApiTestDlg::OnEnChangeRequestData()
{
    // TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
    // __super::OnInitDialog() 함수를 재지정 
    //하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
    // 이 알림 메시지를 보내지 않습니다.

    // TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
