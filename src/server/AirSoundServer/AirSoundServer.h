
// AirSoundServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAirSoundServerApp:
// �йش����ʵ�֣������ AirSoundServer.cpp
//

class CAirSoundServerApp : public CWinApp
{
public:
	CAirSoundServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAirSoundServerApp theApp;