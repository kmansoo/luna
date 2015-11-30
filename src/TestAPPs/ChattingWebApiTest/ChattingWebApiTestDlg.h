
// ChattingWebApiTestDlg.h : header file
//

#pragma once
#include "afxwin.h"


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
    CString _strUserID;
    CString _strUserName;
    CString _strSessionName;
    CListBox _ctlSessionList;
    CString _strJoinedSessionName;
    CListBox _ctlSessionMemberList;
    CListBox _ctlMessageList;
    CString _strServerIP;
    CString _strServerPort;
    afx_msg void OnBnClickedServerInfoUpdate();
};
