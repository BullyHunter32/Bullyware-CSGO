#pragma once
#include <Windows.h>

namespace Hook
{
	PVOID TrampHook(PVOID pTarget, PVOID pHook, SIZE_T cbSize);
	PVOID RestoreHook(PVOID pTarget, PVOID pOriginal, SIZE_T cbSize);
	template <typename T>
	T TrampHook(PVOID pTarget, PVOID pHook, SIZE_T cbSize)
	{
		return (T)TrampHook(pTarget, pHook, cbSize);
	}
}

class CTrampHook
{
public:
	PVOID pTarget;
	PVOID pHook;
	PVOID* ppOriginal;
	SIZE_T cbSize;
	BOOL bHooked = FALSE;
protected:
	PVOID _pOriginal;
public:

	~CTrampHook()
	{
		Restore();
	}

	CTrampHook(PVOID pTarget, PVOID pHook, SIZE_T cbSize, PVOID* ppOriginal = nullptr)
	{
		if (ppOriginal == nullptr)
			ppOriginal = &_pOriginal;

		this->pTarget = pTarget;
		this->pHook = pHook;
		this->cbSize = cbSize;
		this->ppOriginal = ppOriginal;
	}

	PVOID Hook()
	{
		if (bHooked)
			return nullptr;

		if (*ppOriginal = Hook::TrampHook(pTarget, pHook, cbSize))
			bHooked = TRUE;
		return *ppOriginal;
	}

	bool Restore()
	{
		if (!bHooked)
			return false;
		bHooked = FALSE;
		Hook::RestoreHook(pTarget, *ppOriginal, cbSize);
	}
};