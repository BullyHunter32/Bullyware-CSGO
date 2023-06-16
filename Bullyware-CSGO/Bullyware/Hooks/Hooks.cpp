#include "Hooks.h"
#include "../Utils.h"
#include "../Render/D3DDevice.h"
#include "../Bullyware.h"

#ifdef _DEBUG
#include <iostream>
#endif

void H::Initialize()
{
	// should never happen
	if (!g_pDevice)
	{
		Bullyware::bShuttingDown = true;
#ifdef _DEBUG
		throw "g_pDevice is null";
#endif
		return;
	}

	// IDirect3DDevice9::Present
	PVOID pPresent = (*(void***)g_pDevice)[17];

	// Following the two jumps that the Steam present hook implement
	uintptr_t pResolvedSteamPresent = Utils::EvaluateRelASM<uintptr_t>(Utils::EvaluateRelASM(pPresent));

#ifdef _DEBUG
	printf("pResolvedSteamPresent -> %p\n", pResolvedSteamPresent);
#endif

	// Hooking steam's present hook
	Present = new CTrampHook((PVOID)pResolvedSteamPresent, (PVOID)hkPresent, 6, (PVOID*)(&oPresent));
}