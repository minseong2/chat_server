// SocServer.cpp: 구현 파일
//

#include "pch.h"
#include "ChatServer.h"
#include "SocServer.h"


// CSocServer

CSocServer::CSocServer()
{
}

CSocServer::~CSocServer()
{
}


// CSocServer 멤버 함수


void CSocServer::Init(HWND hWnd) {
	m_hWnd = hWnd;
}

void CSocServer::OnAccept(int nErrorCode)
{
	Accept(m_socCom[m_index.front()]);
	SendMessage(m_hWnd, UM_ACCEPT, 0, 0);
	CSocket::OnAccept(nErrorCode);
}

CSocCom* CSocServer::GetAcceptSocCom() {
	return &m_socCom[m_index.front()];
}