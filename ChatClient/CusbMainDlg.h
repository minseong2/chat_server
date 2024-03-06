#pragma once
#include "afxdialogex.h"
#include "ChatClientDlg.h"


// CusbMainDlg 대화 상자

class CusbMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CusbMainDlg)

public:
	CusbMainDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CusbMainDlg();



// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoginButton();
	afx_msg void OnBnClickedCreateButton();
	CString input_ID;
	CString input_PW;
};
