#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include "ServerSession.h"


// ������������Socket���������ݽ���
class Server
{
private:
	SOCKET servSocket;
	ServerSession sess;
public:
	Server(void);
	~Server(void);

	// ������״̬
	enum State
	{
		Started,
		Stopped
	};

	void Start();
	void Stop();
	static DWORD WINAPI WorkThread(LPVOID lpParam);
};

