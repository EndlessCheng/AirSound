#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include "jthread\jthread.h"

// 服务器，负责Socket监听及数据交互
class Server
{
private:
	SOCKET servSocket;
public:
	Server(void);
	~Server(void);

	// 服务器状态
	enum State
	{
		Started,
		Stopped
	};

	void Start();
	void Stop();
	static DWORD WINAPI WorkThread(LPVOID lpParam);
};

