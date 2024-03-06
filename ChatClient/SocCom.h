#pragma once

// CSocCom 명령 대상
#include <afxsock.h>

#define UM_RECEIVE WM_USER+2

#define SOC_CLIENT_CONNECT "접속성공"
#define SOC_CLIENT_DISCONNECT "클라이언트 종료"

class CSocCom : public CSocket
{
public:
	CSocCom();
	virtual ~CSocCom();

	HWND m_hWnd;
	void CSocCom::Init(HWND hWnd);
	virtual void OnReceive(int nErrorCode);

	int m_index;
};


