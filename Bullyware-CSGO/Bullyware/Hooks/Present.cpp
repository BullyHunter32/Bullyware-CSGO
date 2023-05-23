#include "Hooks.h"
#include <iostream>
#include <format>

#include "../Render/Drawing.h"
#include "../SDK/Interfaces.h"
#include "../Bullyware.h"

using namespace H;

HRESULT __stdcall _H::hkPresent(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	Draw::Initialize(pDevice);

	if (Bullyware::bShuttingDown ||
		!Draw::pLine ||
		!Draw::pFont ||
		!Draw::pDevice)
		return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);


	int w, h;
	I::EngineClient->GetScreenSize(w, h);

	int iLocalPlayer = I::EngineClient->GetLocalPlayer();
	C_Entity* pLocalPlayer = iLocalPlayer != 0 ? I::EntityList->GetClientEntity(iLocalPlayer) : nullptr;

	Draw::DrawTextA(std::format("maxClients: {}", I::gpGlobals->maxClients).c_str(), 5, 5, D3DCOLOR_RGBA(255, 0, 0, 255));
	Draw::DrawTextA(std::format("LocalPlayer: {} ({})", (void*)pLocalPlayer, iLocalPlayer).c_str(), 5, 5 + FONT_HEIGHT, D3DCOLOR_RGBA(255, 0, 0, 255));

	// Draw::DrawCircle(120, 120, 50, 32, D3DCOLOR_RGBA(255, 0, 255, 255));

	for (int i = 1; i < I::gpGlobals->maxClients; i++)
	{
		if (!pLocalPlayer) break;
		if (i == iLocalPlayer) continue;

		C_Entity* ply = I::EntityList->GetClientEntity(i);
		if (!ply) continue;


		if (ply->GetHealth() <= 0)
			continue;

		CCollisionProperties* props = ply->GetCollisionProperties();
		if (!props)
			continue;

		int localTeam = pLocalPlayer->GetTeam();
		int team = ply->GetTeam();
		Vector screenpos;
		Vector& absorigin = ply->GetABSOrigin();

		// TODO: find way to check whether player is enemy
		// checking team is not sufficient as in gamemodes such as Deathmatch & DangerZone
		// enemies may share the same teamid
		D3DCOLOR color = D3DCOLOR_ARGB(210, 255, 50, 90);

		Vector mins = props->OBBMins();
		Vector maxs = props->OBBMaxs();

		mins += ply->GetABSOrigin();
		maxs += ply->GetABSOrigin();

		Vector bounds[8] = {
			Vector(mins.x, mins.y, mins.z),
			Vector(mins.x, maxs.y, mins.z),
			Vector(maxs.x, maxs.y, mins.z),
			Vector(maxs.x, mins.y, mins.z),

			Vector(mins.x, mins.y, maxs.z),
			Vector(mins.x, maxs.y, maxs.z),
			Vector(maxs.x, maxs.y, maxs.z),
			Vector(maxs.x, mins.y, maxs.z)
		};

		Vector min, max;
		for (size_t i = 0; i < 8; i++)
		{
			Vector& vec = bounds[i];
			I::DebugOverlay->ScreenPosition(vec, screenpos);
			if (i == 0)
			{
				min = screenpos;
				max = screenpos;
				continue;
			}
			min.x = fminf(min.x, screenpos.x);
			min.y = fminf(min.y, screenpos.y);
			max.x = fmaxf(max.x, screenpos.x);
			max.y = fmaxf(max.y, screenpos.y);
		}

		Draw::DrawBounds(*(D3DXVECTOR2*)(&min), *(D3DXVECTOR2*)(&max), D3DCOLOR_RGBA(120, 30, 255, 190));
		Draw::DrawTextA(std::format("{}", ply->GetHealth()).c_str(), max.x + 4, min.y, D3DCOLOR_RGBA(255, 255, 255, 200));

		player_info_t info;
		if (I::EngineClient->GetPlayerInfo(i, &info))
		{
			Draw::DrawTextA(info.name, min.x + (max.x - min.x)/2.f, min.y - 0, D3DCOLOR_RGBA(255, 255, 255, 200), DT_CENTER | DT_BOTTOM);
		}


		IClientRenderable* renderable = ply->GetClientRenderable();
		if (renderable)
		{
			matrix3x4_t bones[128];
			if (renderable->SetupBones(bones, 128, BONE_USED_BY_HITBOX, I::gpGlobals->curtime))
			{
				Vector headPos = Vector(bones[8][0][3], bones[8][1][3], bones[8][2][3]);
				if (I::DebugOverlay->ScreenPosition(headPos, screenpos) != 1)
				{
					float lenSqr = screenpos.Dist2DSqr({ w / 2.f, h / 2.f });
					if (lenSqr < (180 * 180))
					{
						Draw::DrawLine({ w / 2.f, h / 2.f }, { screenpos.x, screenpos.y }, color, 1);
					}
				}
			}
		}
	}
	
	return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}