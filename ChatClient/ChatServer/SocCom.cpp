// SocCom.cpp: 구현 파일
//

#include "pch.h"
#include "ChatServer.h"
#include "SocCom.h"


// CSocCom

CSocCom::CSocCom()
{
}

CSocCom::~CSocCom()
{
}


// CSocCom 멤버 함수

void CSocCom::Init(HWND hWnd) {
	m_hWnd = hWnd;
}
void CSocCom::OnReceive(int nErrorCode)
{
	SendMessage(m_hWnd, UM_RECEIVE, m_index, 0);
	CSocket::OnReceive(nErrorCode);
}