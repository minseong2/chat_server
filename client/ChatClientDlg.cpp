
// ChatClientDlg.cpp: 구현 파일
//
#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include "framework.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"
#include "afxdialogex.h"
#include "CusbMainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CChatClientDlg 대화 상자

LPARAM CChatClientDlg::OnReceive(UINT wParam, LPARAM IParam)
{
	char pTmp[256];
	CString strTmp;
	memset(pTmp, '\0', 256);



	m_socCom.Receive(pTmp, 256);
	strTmp.Format(_T("%s"), pTmp);
	CString cstr(pTmp);
	
	//CString cstr;
	//cstr.Format(_T("%s"), pTmp);

	if (strTmp.Find((_T(SOC_CLIENT_CONNECT))) == 0)//맨처음
	{
		switch (path)
		{
			case 1:
			{
				m_socCom.Send(m_strUserID + "L " + m_strUserPW, 256);
				break;

			}
			case 2:
			{
				m_socCom.Send(m_strUserID + "J " + m_strUserPW, 256);
				break;

			}
		default:
			break;
		}

		//m_strUserID = (_T("사용자 : ") + strTmp.Right(1));
		
	}
	else {
		int i = m_list.GetCount();


		m_list.InsertString(i, strTmp); //채팅할떈 이건데
		//m_list.InsertString(i, cstr); //이전채팅 불러올땐 이거임




	}
	UpdateData(FALSE);
	return TRUE;

}

CChatClientDlg::CChatClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATCLIENT_DIALOG, pParent)
	, m_strIP(_T(""))
	, m_strSend(_T(""))
	, m_strUserID(_T(""))
	

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP, m_strIP);
	DDX_Text(pDX, IDC_EDIT_SEND, m_strSend);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CChatClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_RECEIVE, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnReceive)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CChatClientDlg::OnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatClientDlg::OnClickedButtonSend)
	ON_BN_CLICKED(IDCANCEL, &CChatClientDlg::OnBnClickedCancel)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CChatClientDlg 메시지 처리기




BOOL CChatClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == SC_CLOSE)
	{
		m_socCom.Send(SOC_CLIENT_DISCONNECT,256);
		this->EndDialog(IDCANCEL);
	}
	else 
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CChatClientDlg::OnPaint()
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
HCURSOR CChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatClientDlg::OnClickedButtonConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_socCom.Create();
	m_socCom.Connect(m_strIP, 7777);
	m_socCom.Init(this->m_hWnd);


}


void CChatClientDlg::OnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	wchar_t pTmp[256];
	CString strTmp;

	memset(pTmp, '\0', 256);
	wcscpy_s(pTmp,256, m_strSend);

	m_strSend = "";

	m_socCom.Send(pTmp, 256);
	UpdateData(FALSE);

}


void CChatClientDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_socCom.Send(_T(SOC_CLIENT_DISCONNECT), 256);
	this->EndDialog(IDCANCEL);
	CDialogEx::OnCancel();
}

