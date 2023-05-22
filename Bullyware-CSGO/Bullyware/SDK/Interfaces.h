#pragma once
#include "../Utils.h"
#include "IClientEntityList.h"
#include "IVDebugOverlay.h"
#include "IVEngineClient.h"
#include "CGlobalVars.h"

struct SOURCE_INTERFACE
{
public:
	PVOID (*_GetAddress)(void*);
	const char* szName;
	SOURCE_INTERFACE* pNext;

	PVOID GetAddress()
	{
		return _GetAddress(this);
	}
};
typedef SOURCE_INTERFACE *PSOURCE_INTERFACE;

namespace I
{
	template <typename T>
	T* CreateInterface(const char* szModule, const char* szInterface)
	{
		uintptr_t pCreateInterface = Utils::EvaluateRelASM<uintptr_t>((PVOID)((uintptr_t)GetProcAddress(GetModuleHandleA(szModule), "CreateInterface") + 4));
		PSOURCE_INTERFACE pRegistry = **(PSOURCE_INTERFACE**)(pCreateInterface + 6);

		PVOID pRet = nullptr;
		for (; pRegistry != nullptr; pRegistry = pRegistry->pNext)
		{
			if (!strcmp(pRegistry->szName, szInterface))
			{
				pRet = pRegistry->GetAddress();
				break;
			}
		}

#ifdef _DEBUG
		printf("%s - %s -> %p\n", szModule, szInterface, pRet);
#endif
		return (T*)pRet;
	}

	inline IVDebugOverlay* DebugOverlay = nullptr;
	inline IClientEntityList* EntityList = nullptr;
	inline IVEngineClient* EngineClient = nullptr;
	inline void* CHLClient = nullptr;
	inline CGlobalVarsBase* gpGlobals = nullptr;
	inline void Initialize()
	{
		EntityList = CreateInterface<IClientEntityList>("client.dll", "VClientEntityList003");
		DebugOverlay = CreateInterface<IVDebugOverlay>("engine.dll", "VDebugOverlay004");
		EngineClient = CreateInterface<IVEngineClient>("engine.dll", "VEngineClient014");
		CHLClient = CreateInterface<void>("client.dll", "VClient018");

		gpGlobals = **(CGlobalVarsBase***)((DWORD)((*(void***)CHLClient)[11]) + 0xA);
	}
}