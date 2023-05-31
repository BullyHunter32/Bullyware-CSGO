#pragma once
#include <Windows.h>
#include <Psapi.h>

namespace Utils
{
	PVOID FindSignature(const char* szModule, const char* szPattern, const char* szMask);
	template <typename T>
	T FindSignature(const char* szModule, const char* szPattern, const char* szMask)
	{
		return (T)FindSignature(szModule, szPattern, szMask);
	}

	inline PVOID EvaluateRelASM(PVOID pAddy)
	{
		uintptr_t addy = (uintptr_t)pAddy + 1; // E9 <relative offset>
		addy = addy + *(int*)addy + 4;
		return (PVOID)addy;
	}

	template <typename T>
	T EvaluateRelASM(PVOID pAddy)
	{
		return (T)EvaluateRelASM(pAddy);
	}

	template <typename TRET, typename ...Args>
	TRET CallVFunction(PVOID pInstance, int index, Args...args)
	{
		PVOID pFunc = (*(PVOID**)pInstance)[index];
		auto func = (TRET(__thiscall*)(PVOID pThis, decltype(args)...))pFunc;
		return func(pInstance, args...);
	}
}