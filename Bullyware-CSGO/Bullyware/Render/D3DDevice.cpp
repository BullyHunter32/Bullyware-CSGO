#include "D3DDevice.h"
#include "../Bullyware.h"

IDirect3DDevice9* g_pDevice = nullptr;

IDirect3DDevice9* GetD3DDevice()
{
	if (g_pDevice) return g_pDevice;

	using namespace Bullyware;
	if (!hWnd)
		return nullptr; // cant retrive device without hWnd

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS params = {};
	ZeroMemory(&params, sizeof(D3DPRESENT_PARAMETERS));
	params.Windowed = TRUE;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.hDeviceWindow = hWnd;

	if (HRESULT res = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &params, &g_pDevice) < 0)
	{
		params.Windowed = !params.Windowed;
		pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &params, &g_pDevice);
	}

	// clean up some memory as we dont need d3d anymore
	if (pD3D)
		pD3D->Release();

	return g_pDevice;
}