#include "Bullyware.h"
#include "Debug/Debug.h"
#include "Utils.h"
#include "Hooks/Hooks.h"

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


	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS params = {};
	ZeroMemory(&params, sizeof(D3DPRESENT_PARAMETERS));
	params.Windowed = TRUE;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.hDeviceWindow = hWnd;

	IDirect3DDevice9* pDevice = nullptr;

	if (HRESULT res = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &params, &pDevice) < 0)
	{
		params.Windowed = !params.Windowed;
		if (res = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &params, &pDevice) < 0)
		{
		}
	}

	// IDirect3DDevice9::Present
	PVOID pPresent = (*(void***)pDevice)[17];

	// Following the two jumps that the Steam present hook implement
	uintptr_t pResolvedSteamPresent = Utils::EvaluateRelASM<uintptr_t>(Utils::EvaluateRelASM(pPresent));

	// Hooking steam's present hook
	CTrampHook* Present = new CTrampHook((PVOID)pResolvedSteamPresent, (PVOID)H::hkPresent, 6, (PVOID*)(&H::oPresent));

	Sleep(10);

	Present->Hook();

	while (!bShuttingDown)
	{
		if (GetAsyncKeyState(VK_END) & 1)
			bShuttingDown = true;
		Sleep(333);
	}

	Present->Restore();

	Sleep(20);

	if (pD3D)
		pD3D->Release();
	if (pDevice)
		pDevice->Release();


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