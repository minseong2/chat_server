
// ChatClientDlg.h: 헤더 파일
//
#pragma once
#include "SocCom.h"

// CChatClientDlg 대화 상자
class CChatClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CChatClientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CSocCom m_socCom;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM IParam);
	DECLARE_MESSAGE_MAP()
public:
	CString m_strIP;
	CString m_strSend;
	CListBox m_list;
	afx_msg void OnClickedButtonConnect();
	afx_msg void OnClickedButtonSend();
	CString m_strUserID;
	CString m_strUserPW;
	short int path;
	afx_msg void OnBnClickedCancel();
};
