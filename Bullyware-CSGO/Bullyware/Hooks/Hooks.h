#pragma once
#include <Windows.h>

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include "../Hooking.h"

namespace H
{
	void Initialize();

#define CREATE_HOOK(ret, callconvention, name, ...) \
	namespace _H { \
		typedef ret (callconvention* t ## name)(__VA_ARGS__); inline t ## name o ## name = nullptr; \
		ret callconvention hk ## name(__VA_ARGS__); \
	}; \
	inline CTrampHook* name = nullptr;

	CREATE_HOOK(HRESULT, __stdcall, Present, IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);

	using namespace _H;
}