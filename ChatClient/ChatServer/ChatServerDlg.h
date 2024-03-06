
// ChatServerDlg.h: 헤더 파일
//

#pragma once

#include "stdAfx.h"
#include "afxwin.h"
#include <string>
#include "mysql.h"
// CChatServerDlg 대화 상자

#define LOGIN_CLIENT 1
#define JOIN_CLIENT 2
class CChatServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CChatServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	CSocServer m_socServer;
	//CSocCom* m_socCom;
	CSocCom* m_socCom[MAX_CLIENT_COUNT];
	CString* m_user_id[MAX_CLIENT_COUNT] = { nullptr };
	CString* m_user_pw[MAX_CLIENT_COUNT] = { nullptr };


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATSERVER_DIALOG };
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
	afx_msg LPARAM OnAccept(UINT wParam, LPARAM lparam);
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM lparam);
	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	CString m_strSend;
	CString m_strStatus;
	CListBox m_list;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnClickedButtonSend();
	CString path;

	std::list<int>m_using;
private:

public:
	afx_msg void OnBnClickedButtonDb();
	afx_msg int act_sql(UINT wParam, int action);
	afx_msg void chat_insert(CString * id, CString detail);//채팅db입력
	afx_msg void last_chat(int idx);//채팅db입력


};
