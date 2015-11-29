
//  WebApiTestDlg.h : 헤더 파일
//

#pragma once

#include "HSWin32API/IHSWin32RgWebApiTransactionNotifier.h"


//  CWebApiTestDlg 대화 상자
class CWebApiTestDlg : public CDialogEx, public IHSWin32RgWebApiTransactionNotifier

{
    //  생성입니다.
public:
    CWebApiTestDlg(CWnd* pParent = NULL);    // 표준 생성자입니다.

    //  대화 상자 데이터입니다.
    enum { IDD = IDD_HSPWEBAPITEST_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    //  DDX/DDV 지원입니다.


    //  구현입니다.
protected:
    HICON m_hIcon;

    //  생성된 메시지 맵 함수
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedPerformTest();
    afx_msg void OnBnClickedVerify();
    afx_msg void OnEnChangeSendData();


protected:
    virtual void    OnTransactionRecveResponse(const CString& strResponse);
    virtual void    OnTransactionRequestTimeout();


protected:
    void    DoUpdateConfiguration();
    BOOL    DoRequestLogin();
    BOOL    DoRequestAPI();

    CString     _strSendData;
    CString     _strReceiveData;

    CString     _strPassword;
    CString     _strRgBoxIP;
    UINT        _uRgBoxPort;
    CString     _strRgBoxID;
    CString     _strRgBoxPassword;
    CString     _strRgOpenAPIToken;
    CString     _strRgMethod;
};
