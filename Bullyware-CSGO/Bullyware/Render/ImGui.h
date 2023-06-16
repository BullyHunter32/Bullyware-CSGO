#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx9.h>
#include <ImGui/imgui_impl_win32.h>

#include "../Bullyware.h"
#include <iostream>

#include <d3dx9.h>
#include <d3d9.h>

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

extern WNDPROC oWndProc;
extern bool MenuOpen;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI hkWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

inline HWND hCSGODX9Wnd = nullptr;
inline bool InitImGui(IDirect3DDevice9* pDevice)
{
	static bool bInit = false;
	if (bInit) return false;
	bInit = !bInit;

	D3DDEVICE_CREATION_PARAMETERS params;
	pDevice->GetCreationParameters(&params);
	hCSGODX9Wnd = params.hFocusWindow;

	ImGuiContext* ctx = ImGui::CreateContext();
	ImGui_ImplWin32_Init(hCSGODX9Wnd);
	ImGui_ImplDX9_Init(pDevice);

	ImGuiIO& io = ImGui::GetIO();
	oWndProc = (WNDPROC)SetWindowLongPtrA(hCSGODX9Wnd, GWLP_WNDPROC, (LONG)hkWndProc);
	return true;
}

inline void UnInitImGui()
{
	SetWindowLongPtrA(hCSGODX9Wnd, GWLP_WNDPROC, (LONG)oWndProc);
}