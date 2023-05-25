#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "../SDK/Interfaces.h"

#include <vector>

#define FONT_FAMILY "Arial"
#define FONT_HEIGHT 12
#define FONT_WIDTH 6
#define FONT_WEIGHT 400

namespace Draw
{
	inline ID3DXLine* pLine = nullptr;
	inline ID3DXFont* pFont = nullptr;
	inline IDirect3DDevice9* pDevice = nullptr;
	inline void Initialize(IDirect3DDevice9* pDevice)
	{
		Draw::pDevice = pDevice;
		if (pLine == nullptr)
			D3DXCreateLine(pDevice, &pLine);
		if (pFont == nullptr)
			D3DXCreateFontA(pDevice, FONT_HEIGHT, FONT_WIDTH, FONT_WEIGHT, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, FONT_FAMILY, &pFont);

	}

	inline void Shutdown()
	{
		if (pLine) { pLine->Release(); pLine = nullptr; }
		if (pFont) { pFont->Release(); pFont = nullptr; }
	}

	inline FLOAT flLineThickness = 1.f;
	inline void DrawLine(D3DXVECTOR2 from, D3DXVECTOR2 to, D3DCOLOR col, FLOAT thickness = 1.f)
	{
		if (pLine == nullptr) return;

		pLine->Begin();
		if (flLineThickness != thickness)
			pLine->SetWidth(flLineThickness = thickness);

		D3DXVECTOR2 pVertices[2] = {};
		pVertices[0] = from;
		pVertices[1] = to;

		pLine->Draw(pVertices, 2, col);
		pLine->End();
	}

	inline void DrawLine(FLOAT x0, FLOAT y0, FLOAT x1, FLOAT y1, D3DCOLOR col, FLOAT thickness = 1.f)
	{
		DrawLine({ x0, y0 }, { x1, y1 }, col, thickness);
	}

	inline void DrawTextA(LPCSTR szText, LONG x, LONG y, D3DCOLOR col, DWORD format = 0)
	{
		if (pFont == nullptr) return;

		RECT rect = {};
		rect.left = rect.right = x;
		rect.top = rect.bottom = y;

		format |= DT_NOCLIP;
		pFont->DrawTextA(nullptr, szText, -1, &rect, format, col);
	}

	inline void DrawBounds(const D3DXVECTOR2& mins, const D3DXVECTOR2& maxs, D3DCOLOR col, FLOAT thickness = 1.f)
	{
		DrawLine({ mins.x, mins.y }, { maxs.x, mins.y }, col, thickness); // tl tr
		DrawLine({ maxs.x, mins.y }, { maxs.x, maxs.y }, col, thickness); // tr br
		DrawLine({ maxs.x, maxs.y }, { mins.x, maxs.y }, col, thickness); // br bl
		DrawLine({ mins.x, maxs.y }, { mins.x, mins.y }, col, thickness); // bl tl
	}

	// yet to figure out DrawPrimitive
	inline void DrawCircle(FLOAT x, FLOAT y, FLOAT radius, SHORT vertices, D3DCOLOR col)
	{
#if 0
		D3DXVECTOR2* pVertices = new D3DXVECTOR2[vertices + 1];
		
		constexpr FLOAT r = D3DXToRadian(1);

		for (int i = 0; i < vertices; i++)
		{
			pVertices[i] = {
				x + cos(r * i) * radius,
				y + sin(r * i) * radius
			};
		}

		pDevice->SetTexture(0, nullptr);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, vertices - 1, (void*)pVertices, sizeof(D3DXVECTOR2));
		delete[] pVertices;
#endif
	}

	inline void DrawBoneArray(matrix3x4_t* pBones, std::vector<bone_t>& vecBoneArray, D3DCOLOR col, FLOAT thickness = 1.f)
	{
		Vector screenpos;
		Vector prevScreenpos;
		for (int i = 1; i < vecBoneArray.size(); i++)
		{
			if (I::DebugOverlay->ScreenPosition(pBones->GetVector(vecBoneArray.at(i - 1)), prevScreenpos) != 1 &&
				I::DebugOverlay->ScreenPosition(pBones->GetVector(vecBoneArray.at(i)), screenpos) != 1)
			{
				DrawLine(screenpos, prevScreenpos, col, thickness);
			}

		}
	}
}