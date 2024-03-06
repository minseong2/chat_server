  
// ChatServerDlg.cpp: 구현 파일
//
#pragma comment(lib, "ws2_32.lib") //명시적인 라이브러리의 링크. 윈속 라이브러리 참조

#include "pch.h"
#include "framework.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"
#include "afxdialogex.h"
#include "stdafx.h"
#include <jdbc.h>
#include <vector>

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


// CChatServerDlg 대화 상자



CChatServerDlg::CChatServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATSERVER_DIALOG, pParent)
	, m_strSend(_T(""))
	, m_strStatus(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEND, m_strSend);
	DDX_Text(pDX, IDC_STATIC_STATUS, m_strStatus);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

LPARAM CChatServerDlg::OnAccept(UINT wParam, LPARAM lParam) {

	
	try {
		int tmp = m_socServer.m_index.front();

		CString number;
		number.Format(_T("%d"), tmp);

		m_socCom[tmp] = new CSocCom();
		m_socCom[tmp] = m_socServer.GetAcceptSocCom();
		m_socServer.m_index.pop_front();
		m_using.push_back(tmp);

		m_socCom[tmp]->m_index = tmp;
		m_socCom[tmp]->Init(this->m_hWnd);

		m_socCom[tmp]->Send((_T(SOC_CLIENT_CONNECT + number)), 256);

		
		// m_user_id[tmp]와 m_user_pw[tmp]가 비어있을 경우에만 데이터 처리
		char buffer[256];
		CString strReceivedData;
		

		memset(buffer, '\0', sizeof(buffer));
		if (m_user_id[tmp] == nullptr && m_user_pw[tmp] == nullptr)
		{
			// 데이터 수신
			int nBytesReceived = m_socCom[tmp]->Receive(buffer, sizeof(buffer));
			strReceivedData.Format(_T("%s"), buffer);

			if (nBytesReceived > 3)
			{
				// 수신한 데이터를 CString 객체에 저장
				int i = m_list.GetCount();
				// 데이터를 구분하여 처리하는 로직을 추가해주세요
				// 예를 들어, 개행 문자('\n')를 기준으로 데이터를 분리해서 처리할 수 있습니다
				int nIndex = strReceivedData.Find(' ');
				if (nIndex != -1)
				{
					// 분리된 데이터
					m_user_id[tmp] = new CString();
					m_user_pw[tmp] = new CString();

					*m_user_id[tmp] = strReceivedData.Left(nIndex-1);
					this->path = strReceivedData.Mid(nIndex-1, 1);
					*m_user_pw[tmp] = strReceivedData.Mid(nIndex + 1);
					int result=0;
					if (path=="L")
					{

						result=act_sql(tmp, 1);
						if (result)
						{
							CString a;
							int i = m_list.GetCount();
							a.Format(L"%s 님이 로그인 하셨습니다.", *m_user_id[tmp]);
							m_list.InsertString(i, a);
							last_chat(tmp);
							a = "";
						}

					}
					else if (path == "J")
					{
						result=act_sql(tmp, 2);
						if (result)
						{
							CString b;
							int i = m_list.GetCount();
							b.Format(L"%s 님이 회원가입 하셨습니다.", *m_user_id[tmp]);
							m_list.InsertString(i, b);
							last_chat(tmp);
						}
					}
					
				}
				
				
			}
		}
		

	}
	catch (CException* ex)
	{
		ex->ReportError();
	}
	//m_socCom[wParam]->Receive(m_user_id[wParam], 256);

	UpdateData(FALSE);
	return TRUE;
}

int CChatServerDlg::act_sql(UINT wParam,int ort)//로그인 or 회원가입
{
	int result = 0;


	MYSQL* connection = mysql_init(NULL);
	if (mysql_real_connect(connection, "localhost", "root", "0000", "sqlmy", 3306, NULL, 0) != NULL) {
		// 연결 성공한 경우
		//MessageBox(L"MySQL에 연결되었습니다.", L"연결 성공", MB_OK | MB_ICONINFORMATION);
	}
	else {
		// 연결 실패한 경우
		MessageBox(L"MySQL 연결에 실패했습니다.", L"연결 실패", MB_OK | MB_ICONERROR);
	}
	try
	{

		// MySQL Connector/C++ 초기화
		sql::mysql::MySQL_Driver* driver; // 추후 해제하지 않아도 Connector/C++가 자동으로 해제해 줌
		sql::Connection* con{};
		sql::Statement* stmt{};
		sql::PreparedStatement* pstmt{};
		sql::ResultSet* res{};




		driver = sql::mysql::get_mysql_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "0000");
		con->setSchema("sqlmy"); // 데이터베이스 선택

		stmt = con->createStatement();
		stmt->execute("set names euckr");
		if (stmt) { delete stmt; stmt = nullptr; }
		switch (ort)
		{
	
		case 1: //로그인
		{
			pstmt = con->prepareStatement("SELECT id FROM user WHERE id = ? AND pw = ?");
			pstmt->setString(1, sql::SQLString(CT2CA(*m_user_id[wParam])));
			pstmt->setString(2, sql::SQLString(CT2CA(*m_user_pw[wParam])));
			res = pstmt->executeQuery();

			if (res->next()) {
				//MessageBox(L"로그인 성공", m_user_id[wParam]->GetString(), MB_OK | MB_ICONINFORMATION);
				
				result = 1;
			}
			else {
				MessageBox(L"로그인 실패", m_user_id[wParam]->GetString(), MB_OK | MB_ICONINFORMATION);
			}
			break;
		}

		
		case 2://회원가입
		{
			pstmt = con->prepareStatement("INSERT INTO user(id, pw) VALUES(?,?)"); // INSERT
			pstmt->setString(1, sql::SQLString(CT2CA(*m_user_id[wParam])));
			pstmt->setString(2, sql::SQLString(CT2CA(*m_user_pw[wParam])));
			//회원가입 성공
			if (pstmt->executeQuery())
			{
				//MessageBox(L"회원가입 성공", m_user_id[wParam]->GetString(), MB_OK | MB_ICONINFORMATION);
				result = 1;
			}
			
			break;
		}

		default:
			break;
		}

		// MySQL Connector/C++ 정리
		delete res;
		delete stmt;
		delete con;
		delete pstmt;

	}
	catch (sql::SQLException& e) {
		CString errorMessage;
		if (e.getErrorCode() != 0) {
			errorMessage.Format(L"MySQL 오류 발생: %d %S", e.getErrorCode(), e.what());
			// 오류 코드와 오류 메시지를 출력합니다.
			AfxMessageBox(errorMessage, MB_OK | MB_ICONINFORMATION);
		}
		else {
			//errorMessage = L"MySQL 작업이 성공적으로 완료되었습니다.";
		}
		//AfxMessageBox(errorMessage, MB_OK | MB_ICONINFORMATION);
	}

	

	return result;
}

LPARAM CChatServerDlg::OnReceive(UINT wParam, LPARAM lParam) {

	char pTmp[256];
	CString strTmp;
	wchar_t nxt_Tmp[256];

	memset(pTmp, '\0', 256);



	m_socCom[wParam]->Receive(pTmp, 256);
	strTmp.Format(_T("%s"), pTmp);
	
	//MessageBox(m_user_id[wParam]->GetString(), L"id", MB_OK | MB_ICONINFORMATION);

	//MessageBox(strTmp, L"receive", MB_OK | MB_ICONINFORMATION);


	

	if (strTmp.Compare(_T(SOC_CLIENT_DISCONNECT)) == 0)
	{
		m_socServer.m_socCom[wParam].Close();
		m_socCom[wParam]->Close();
		m_socServer.m_index.push_back(wParam);
		m_using.erase(std::remove(m_using.begin(), m_using.end(), wParam), m_using.end());
		delete m_user_id[wParam];
		delete m_user_pw[wParam];
		m_user_id[wParam] = nullptr;
		m_user_pw[wParam] = nullptr;
	}
	else if (m_user_id[wParam] != nullptr && m_user_id[wParam]->GetLength() != 0)
	{
		
		CString id;
		id.Format(_T("%d"), wParam);

		int i = m_list.GetCount();
		CString combinedString;
		combinedString.Format(L"%s 님 : %s", *m_user_id[wParam],strTmp);
		m_list.InsertString(i, combinedString);
		//m_list.InsertString(i, (_T("사용자" + id + " : " + strTmp)));
		
		chat_insert(m_user_id[wParam], strTmp);

		for each (int i in m_using)
		{
			if (i != _ttoi(id))
			{
				wcscpy_s(nxt_Tmp, 256, combinedString.GetBuffer());

				m_socCom[i]->Send(nxt_Tmp, 256);
				//m_socCom[i]->Send((_T("사용자" + id + " : " + strTmp)), 256);

			}
		}	
	}

	return TRUE;
}

void CChatServerDlg::chat_insert(CString* id, CString detail)//채팅db입력
{
	try
	{

		// MySQL Connector/C++ 초기화
		sql::mysql::MySQL_Driver* driver; // 추후 해제하지 않아도 Connector/C++가 자동으로 해제해 줌
		sql::Connection* con{};
		sql::Statement* stmt{};
		sql::PreparedStatement* pstmt{};
		sql::ResultSet* res{};



		driver = sql::mysql::get_mysql_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "0000");
		con->setSchema("sqlmy"); // 데이터베이스 선택

		stmt = con->createStatement();
		stmt->execute("set names euckr");
		if (stmt) { delete stmt; stmt = nullptr; }





		pstmt = con->prepareStatement("INSERT INTO chatting(details,user_id) VALUES(?,?)"); // INSERT
		pstmt->setString(1, sql::SQLString(CT2CA(detail)));
		pstmt->setString(2, sql::SQLString(CT2CA(*id)));

		pstmt->execute();//채팅입력
		// MySQL Connector/C++ 정리
		delete res;
		delete stmt;
		delete con;
		delete pstmt;

	}
	catch (sql::SQLException& e) {
		CString errorMessage;
		if (e.getErrorCode() != 0) {
			errorMessage.Format(L"MySQL 오류 발생: %d %S", e.getErrorCode(), e.what());
			// 오류 코드와 오류 메시지를 출력합니다.
			AfxMessageBox(errorMessage, MB_OK | MB_ICONINFORMATION);
		}
		else {
			//errorMessage = L"MySQL 작업이 성공적으로 완료되었습니다.";
		}
		//AfxMessageBox(errorMessage, MB_OK | MB_ICONINFORMATION);
	}
	return;
}

void CChatServerDlg::last_chat(int idx)//채팅db입력
{
	try
	{

		// MySQL Connector/C++ 초기화
		sql::mysql::MySQL_Driver* driver; // 추후 해제하지 않아도 Connector/C++가 자동으로 해제해 줌
		sql::Connection* con{};
		sql::Statement* stmt{};
		sql::PreparedStatement* pstmt{};
		sql::ResultSet* res{};

		std::string str = {};
		wchar_t charStr[256];
		memset(charStr, '\0', 256);

		driver = sql::mysql::get_mysql_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "0000");
		con->setSchema("sqlmy"); // 데이터베이스 선택

		stmt = con->createStatement();
		stmt->execute("set names euckr");
		if (stmt) { delete stmt; stmt = nullptr; }

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT details,user_id FROM chatting WHERE num>0");
		while (res->next()) {
			
			str=res->getString("user_id") + " : " + res->getString("details")+"\n";
			//std::wstring name = std::wstring(str.begin(), str.end());
			//const wchar_t* pTmp = name.c_str();
			// char to wchar_t 
			CString a; 
			a=str.c_str();
			wcscpy_s(charStr, 256, a);


			m_socCom[idx]->Send(charStr, 256);//strlen(str.c_str())



			//cout << str;
		}

		// MySQL Connector/C++ 정리
		delete res;
		delete stmt;
		delete con;
		delete pstmt;

	}
	catch (sql::SQLException& e) {
		CString errorMessage;
		if (e.getErrorCode() != 0) {
			errorMessage.Format(L"MySQL 오류 발생: %d %S", e.getErrorCode(), e.what());
			// 오류 코드와 오류 메시지를 출력합니다.
			AfxMessageBox(errorMessage, MB_OK | MB_ICONINFORMATION);
		}
	}
	return;
}


BEGIN_MESSAGE_MAP(CChatServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_LBN_SELCHANGE(IDC_LIST1, &CChatServerDlg::OnLbnSelchangeList1)
	ON_MESSAGE(UM_ACCEPT, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnAccept)
	ON_MESSAGE(UM_RECEIVE, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnReceive)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatServerDlg::OnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_DB, &CChatServerDlg::OnBnClickedButtonDb)
END_MESSAGE_MAP()


void CChatServerDlg::OnOK()
{
	// 다이얼로그 종료 전에 MySQL 연결을 해제하지 않습니다.
	// 필요한 경우, 다른 이벤트 핸들러 등을 사용하여 연결 해제를 수행해야 합니다.

	CDialogEx::OnOK();
}

// CChatServerDlg 메시지 처리기

BOOL CChatServerDlg::OnInitDialog()
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

	for (int i = 0; i < MAX_CLIENT_COUNT; i++)
	{
		m_socServer.m_index.push_back(i);
	}

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_socServer.Create(7777);
	m_socServer.Listen();
	m_socServer.Init(this->m_hWnd);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CChatServerDlg::OnPaint()
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
HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatServerDlg::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CChatServerDlg::OnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	wchar_t pTmp[256];
	CString strTmp;

	memset(pTmp, '\0', 256);
	wcscpy_s(pTmp,256, m_strSend);
	
	

	for each (int i in m_using)
	{
		m_socCom[i]->Send(pTmp, 256);

	}
	m_strSend = "";
	/*
	strTmp.Format(_T("%s"), pTmp);
	int i = m_list.GetCount();
	m_list.InsertString(i, strTmp);
	*/
	UpdateData(FALSE);
}


void CChatServerDlg::OnBnClickedButtonDb()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MYSQL* connection = mysql_init(NULL);
	if (mysql_real_connect(connection, "localhost", "root", "0000", "sqlmy", 3306, NULL, 0) != NULL) {
		// 연결 성공한 경우
		//MessageBox(L"MySQL에 연결되었습니다.", L"연결 성공", MB_OK | MB_ICONINFORMATION);
	}
	else {
		// 연결 실패한 경우
		MessageBox(L"MySQL 연결에 실패했습니다.", L"연결 실패", MB_OK | MB_ICONERROR);
	}


}
