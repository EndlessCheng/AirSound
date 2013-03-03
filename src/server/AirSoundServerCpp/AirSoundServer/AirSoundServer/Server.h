#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include "jthread\jthread.h"

// ������������Socket���������ݽ���
class Server
{
private:
	SOCKET servSocket;
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

