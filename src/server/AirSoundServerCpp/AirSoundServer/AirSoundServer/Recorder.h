#pragma once
#include <iostream>
#include <Windows.h>
#include <MMSystem.h>
using namespace std;

// 
typedef void (*EnumDevsProc)(int, string);

class Recorder
{
public:
	Recorder(void);
	~Recorder(void);

	// ö��¼���豸
	void EnumDevs(EnumDevsProc lpProc);
};

