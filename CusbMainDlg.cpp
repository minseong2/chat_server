// CusbMainDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ChatClient.h"
#include "afxdialogex.h"
#include "CusbMainDlg.h"

// CusbMainDlg 대화 상자

IMPLEMENT_DYNAMIC(CusbMainDlg, CDialogEx)

CusbMainDlg::CusbMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN_DIALOG, pParent)
	, input_ID(_T(""))
	, input_PW(_T(""))
{
}

CusbMainDlg::~CusbMainDlg()
{
}

void CusbMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, input_ID);
	DDX_Text(pDX, IDC_EDIT_PW, input_PW);
}


BEGIN_MESSAGE_MAP(CusbMainDlg, CDialogEx)
	ON_BN_CLICKED(IDC_LOGIN_BUTTON, &CusbMainDlg::OnBnClickedLoginButton)
	ON_BN_CLICKED(IDC_CREATE_BUTTON, &CusbMainDlg::OnBnClickedCreateButton)
END_MESSAGE_MAP()


// CusbMainDlg 메시지 처리기





void CusbMainDlg::OnBnClickedLoginButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	 
	UpdateData(TRUE);

	CChatClientDlg* m_pChatClientDlg = new CChatClientDlg(); // 다른 다이얼로그 객체 동적 생성
	m_pChatClientDlg->m_strUserID = input_ID;
	m_pChatClientDlg->m_strUserPW = input_PW;
	m_pChatClientDlg->path = LOGIN_CLIENT;

	m_pChatClientDlg->DoModal(); // 다른 다이얼로그 표시
	delete m_pChatClientDlg; // 다른 다이얼로그 객체 해제
	EndDialog(IDOK); // 현재 다이얼로그 닫기

}


void CusbMainDlg::OnBnClickedCreateButton()
{

	UpdateData(TRUE);

	CChatClientDlg* m_pChatClientDlg = new CChatClientDlg(); // 다른 다이얼로그 객체 동적 생성
	m_pChatClientDlg->m_strUserID = input_ID;
	m_pChatClientDlg->m_strUserPW = input_PW;
	m_pChatClientDlg->path = JOIN_CLIENT;

	m_pChatClientDlg->DoModal(); // 다른 다이얼로그 표시
	delete m_pChatClientDlg; // 다른 다이얼로그 객체 해제
	EndDialog(IDOK); // 현재 다이얼로그 닫기
}

