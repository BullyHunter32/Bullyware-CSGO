#pragma once
#include <Windows.h>

namespace Bullyware
{
	extern bool bShuttingDown;
	extern HWND hWnd;
	extern HMODULE hInstance;
	BOOL WINAPI Initialize(HMODULE hModule);
	void Shutdown();
}