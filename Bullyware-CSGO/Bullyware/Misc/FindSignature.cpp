#include "../Utils.h"

#include <iostream>

// Signature scanning doesn't work for CSGO?
PVOID Utils::FindSignature(const char* szModule, const char* szPattern, const char* szMask)
{
	//MODULEINFO modinfo;
	//HMODULE hMod = 0;
	//if (GetModuleInformation(GetCurrentProcess(), hMod = GetModuleHandleA(szModule), &modinfo, sizeof(modinfo)))
	//{
	//	return nullptr;
	//}

	//uintptr_t pBase = (uintptr_t)modinfo.lpBaseOfDll;
	//DWORD dwSize = modinfo.SizeOfImage;
	//size_t szLen = strlen(szPattern);

	//for (uintptr_t i = pBase; i < pBase + dwSize - szLen; i++)
	//{
	//	bool found = true;
	//	for (size_t j = 0; j < szLen; j++)
	//	{
	//		if (szMask[j] == '?') continue;

	//		if (*(BYTE*)(i + j) != (BYTE)szPattern[j])
	//		{
	//			found = false;
	//			break;
	//		}
	//	}

	//	if (found)
	//		return (PVOID)i;
	//}

	return nullptr;
}