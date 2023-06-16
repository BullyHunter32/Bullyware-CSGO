#include "ImGui.h"
#include <iostream>


WNDPROC oWndProc = nullptr;
bool MenuOpen = false;
LRESULT WINAPI hkWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (MenuOpen && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

	return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
}