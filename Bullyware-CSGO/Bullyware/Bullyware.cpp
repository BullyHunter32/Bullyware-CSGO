#include "Bullyware.h"
#include "Debug/Debug.h"
#include "Utils.h"
#include "Hooks/Hooks.h"
#include "Render/D3DDevice.h"
#include "Render/Drawing.h"
#include "SDK/Interfaces.h"

void* (*oCreateInterface)(const char*, int*) = nullptr;
void* hkCreateInterface(const char* szModule, int* ret)
{
	printf("CreateInterface(%s, %p)\n", szModule, ret);
	return oCreateInterface(szModule, ret);
}

HWND Bullyware::hWnd = NULL;
BOOL CALLBACK EnumWndCallback(HWND hWnd, LPARAM lParam) 
{
	DWORD dwProcId;
	GetWindowThreadProcessId(hWnd, &dwProcId);
	if (dwProcId == lParam)
	{
		Bullyware::hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}

HMODULE Bullyware::hInstance = NULL;
bool Bullyware::bShuttingDown = false;
BOOL WINAPI Bullyware::Initialize(HMODULE hModule)
{
	FILE* f;
	hInstance = hModule;
#ifdef _DEBUG
	AllocConsole();
	freopen_s(&f, "CONOUT$", "w", stdout);
	printf("[Bullyware Debug Console]\n");
	SetConsoleTitleA("Bullyware Debug Console");

	printf("Initialize!\n");
	printf("hWnd -> %p\n", hWnd);
#endif
	EnumWindows(EnumWndCallback, GetCurrentProcessId());
	Sleep(10);

	I::Initialize();

	GetD3DDevice();

	H::Initialize();
	H::Present->Hook();

	while (!bShuttingDown)
	{
		if (GetAsyncKeyState(VK_END) & 1)
			bShuttingDown = true;
		Sleep(333);
	}

	H::Present->Restore();

	Sleep(20);

	if (g_pDevice)
		g_pDevice->Release();

	Draw::Shutdown();

	FreeConsole();
	if (pFile)
		fclose(pFile);
	FreeLibraryAndExitThread(hModule, 0);
	return TRUE;
}

void Bullyware::Shutdown()
{
#ifdef _DEBUG

#endif
}