
// ChattingWebApiTestDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include <memory>
#include "ccWebsocketClient/easywsclient/easywsclient.hpp"
//#include "ccWebsocketClient/easywsclient/easywsclient.hpp"

// CChattingWebApiTestDlg dialog
class CChattingWebApiTestDlg : public CDialogEx
{
// Construction
public:
	CChattingWebApiTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATTINGWEBAPITEST_DIALOG };
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
    afx_msg void OnBnClickedMessageSend();
    afx_msg void OnBnClickedSessionLeave();
    afx_msg void OnBnClickedSessionMemberUpdate();
    afx_msg void OnBnClickedLogin();
    afx_msg void OnBnClickedLogout();
    afx_msg void OnBnClickedSessionCreate();
    afx_msg void OnBnClickedSessionListUpdate();
    afx_msg void OnBnClickedSessionJoin();
    afx_msg void OnBnClickedServerInfoUpdate();
    afx_msg void OnBnClickedSessionDelete();

private:
    void    DoSessionListUpdate();
    void    DoSessionMemberListUpdate();

private:
    void    DoWebsocketCleintHandleMessage(const std::string & message);

private:
    CString _strUserID;
    CString _strUserName;
    CString _strSessionName;
    CString _strJoinedSessionName;
    CString _strServerIP;
    CString _strServerPort;

    CListBox _ctlSessionList;
    CListBox _ctlSessionMemberList;
    CListBox _ctlMessageList;
    CString _strSendMessage;

    std::unique_ptr<easywsclient::WebSocket> _pWSClient;

public:
    afx_msg void OnLbnDblclkSessionList();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
