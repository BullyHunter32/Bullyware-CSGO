#pragma once
#include "../Render/ImGui.h"
#include "Textures.h"
#include "Verdana.h"
#include "GUI.h"

namespace Menu
{
	inline bool bInit = false;
	inline void Initialize(IDirect3DDevice9* pDevice)
	{
		if (bInit) return;
		bInit = true;

		D3DXCreateTextureFromFileInMemoryEx(pDevice, StandingTextureBinary, sizeof(StandingTextureBinary), 378, 663, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, NULL, NULL, &GUI::pStandingTexture);
		D3DXCreateTextureFromFileInMemoryEx(pDevice, LyingTextureBinary, sizeof(LyingTextureBinary), 716, 1269, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, NULL, NULL, &GUI::pLyingTexture);

		ImGuiIO& io = ImGui::GetIO();
		GUI::Fonts.Verdana = io.Fonts->AddFontFromMemoryCompressedTTF((void*)Font_Verdana_compressed_data, Font_Verdana_compressed_size, 16);
		io.Fonts->AddFontDefault();

		GUI::Fonts.VerdanaBold = io.Fonts->AddFontFromMemoryCompressedTTF((void*)Font_VerdanaBold_compressed_data, Font_VerdanaBold_compressed_size, 18);

		ImGuiStyle& style = ImGui::GetStyle();

		style.FrameRounding = 4.f;
		style.WindowRounding = 6.f;
		style.WindowPadding = ImVec2(0, 0);
	}

	void Render(IDirect3DDevice9* pDevice);
	void Run(IDirect3DDevice9* pDevice);
}