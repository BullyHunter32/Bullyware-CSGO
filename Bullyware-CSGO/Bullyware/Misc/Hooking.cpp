#include "../Hooking.h"

PVOID Hook::TrampHook(PVOID pTarget, PVOID pHook, SIZE_T cbSize)
{
	if (cbSize < 5) return nullptr;

	PVOID pAllocMem = VirtualAlloc(NULL, cbSize + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!pAllocMem)
		return nullptr;

	DWORD dwProtect;
	VirtualProtect(pTarget, cbSize, PAGE_EXECUTE_READWRITE, &dwProtect);
	memcpy(pAllocMem, pTarget, cbSize);
	memset(pTarget, 0x90, cbSize);
	BYTE* pJMP = (BYTE*)pTarget;
	*pJMP = 0xE9;
	*(uintptr_t*)(pJMP + 1) = (uintptr_t)pHook - (uintptr_t)pTarget - 5;
	VirtualProtect(pTarget, cbSize, PAGE_EXECUTE_READWRITE, &dwProtect);

	pJMP = (BYTE*)((uintptr_t)pAllocMem + cbSize);
	*pJMP = 0xE9;
	*(uintptr_t*)(pJMP + 1) = (uintptr_t)pTarget - (uintptr_t)pJMP - 5 + cbSize;

	return pAllocMem;
}

PVOID Hook::RestoreHook(PVOID pTarget, PVOID pOriginal, SIZE_T cbSize)
{
	DWORD dwProtect;
	VirtualProtect(pTarget, cbSize, PAGE_EXECUTE_READWRITE, &dwProtect);
	FlushInstructionCache(GetCurrentProcess(), pTarget, cbSize);
	memcpy(pTarget, pOriginal, cbSize);
	VirtualProtect(pTarget, cbSize, dwProtect, &dwProtect);
	return nullptr;
}