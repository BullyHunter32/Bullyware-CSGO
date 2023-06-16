#include "Hooks.h"
#include <iostream>
#include <format>
#include <vector>

#include "../Render/Drawing.h"
#include "../Render/ImGui.h"
#include "../SDK/Interfaces.h"
#include "../Menu/Menu.h"
#include "../Features/Visuals.h"

// temporary skeleton esp solution
// TODO: find & use bone names rather than ids
// cl_countbones convar creates skeleton esp
// maybe reverse engineer that
std::vector<std::vector<bone_t>> BONES_CT = {
	{ 0, 5, 6, 7, 8 },
	{ 0, 73, 74, 75 },
	{ 0, 82, 83, 85 },
	{ 7, 11, 12, 14 },
	{ 7, 41, 42, 44 },
};

std::vector<std::vector<bone_t>> BONES_T = {
	{ 0, 5, 6, 7, 8 },
	{ 0, 66, 67, 69 },
	{ 0, 73, 74, 76 },
	{ 7, 10, 11, 12 },
	{ 7, 38, 39, 40 },
};

using namespace H;

HRESULT __stdcall _H::hkPresent(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	Draw::Initialize(pDevice);

	if (Bullyware::bShuttingDown ||
		InitImGui(pDevice) ||
		!Draw::pLine ||
		!Draw::pFont ||
		!Draw::pDevice)
		return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	int w, h;
	I::EngineClient->GetScreenSize(w, h);

	int iLocalPlayer = I::EngineClient->GetLocalPlayer();
	C_Entity* pLocalPlayer = iLocalPlayer != 0 ? I::EntityList->GetClientEntity(iLocalPlayer) : nullptr;

	int yPOS = 5;
	auto AddDebugLine = [=, &yPOS](const std::string& str) -> void
	{
		Draw::DrawTextAOutlined(str.c_str(), 5, yPOS, D3DCOLOR_RGBA(255, 0, 0, 255), 1);
		yPOS += FONT_HEIGHT;
	};

	AddDebugLine(std::format("LocalPlayer: {} ({})", (void*)pLocalPlayer, iLocalPlayer));
	if (pLocalPlayer)
	{
		AddDebugLine(std::format("IsPlayer: {}", pLocalPlayer->IsPlayer()));
		AddDebugLine(std::format("Classname: {}", pLocalPlayer->GetClassname()));

		if (IClientRenderable* renderable = pLocalPlayer->GetClientRenderable())
		{
			AddDebugLine(std::format("Renderable: {}", (void*)renderable));
			AddDebugLine(std::format("Renderable->EntIndex: {}", renderable->EntIndex()));
		}
	}

	F::Visuals::Run();

	//for (int i = 1; i < I::gpGlobals->maxClients; i++)
	//{
	//	if (!pLocalPlayer) break;
	//	if (i == iLocalPlayer) continue;

	//	C_Entity* ply = I::EntityList->GetClientEntity(i);
	//	if (!ply) continue;


	//	if (ply->GetHealth() <= 0)
	//		continue;

	//	CCollisionProperties* props = ply->GetCollisionProperties();
	//	if (!props)
	//		continue;

	//	int localTeam = pLocalPlayer->GetTeam();
	//	int team = ply->GetTeam();
	//	Vector screenpos;
	//	Vector& absorigin = ply->GetABSOrigin();

	//	// TODO: find way to check whether player is enemy
	//	// checking team is not sufficient as in gamemodes such as Deathmatch & DangerZone
	//	// enemies may share the same teamid
	//	D3DCOLOR color = D3DCOLOR_ARGB(210, 255, 50, 90);

	//	Vector mins = props->OBBMins();
	//	Vector maxs = props->OBBMaxs();

	//	mins += ply->GetABSOrigin();
	//	maxs += ply->GetABSOrigin();

	//	Vector bounds[8] = {
	//		Vector(mins.x, mins.y, mins.z),
	//		Vector(mins.x, maxs.y, mins.z),
	//		Vector(maxs.x, maxs.y, mins.z),
	//		Vector(maxs.x, mins.y, mins.z),

	//		Vector(mins.x, mins.y, maxs.z),
	//		Vector(mins.x, maxs.y, maxs.z),
	//		Vector(maxs.x, maxs.y, maxs.z),
	//		Vector(maxs.x, mins.y, maxs.z)
	//	};

	//	Vector min, max;
	//	for (size_t i = 0; i < 8; i++)
	//	{
	//		Vector& vec = bounds[i];
	//		I::DebugOverlay->ScreenPosition(vec, screenpos);
	//		if (i == 0)
	//		{
	//			min = screenpos;
	//			max = screenpos;
	//			continue;
	//		}
	//		min.x = fminf(min.x, screenpos.x);
	//		min.y = fminf(min.y, screenpos.y);
	//		max.x = fmaxf(max.x, screenpos.x);
	//		max.y = fmaxf(max.y, screenpos.y);
	//	}

	//	Draw::DrawBounds(min, max, D3DCOLOR_RGBA(120, 30, 255, 190));
	//	Draw::DrawTextAOutlined(std::format("{}", ply->GetHealth()).c_str(), max.x + 4, min.y, D3DCOLOR_RGBA(255, 255, 255, 200));
	//	Draw::DrawTextA(std::format("{}", ply->GetTeam()).c_str(), max.x + 4, min.y + FONT_HEIGHT, D3DCOLOR_RGBA(255, 255, 255, 200));

	//	player_info_t info;
	//	if (I::EngineClient->GetPlayerInfo(i, &info))
	//	{
	//		Draw::DrawTextAOutlined(info.name, min.x + (max.x - min.x)/2.f, min.y - 0, D3DCOLOR_RGBA(255, 255, 255, 200), 1, 0xFF000000, DT_CENTER | DT_BOTTOM);
	//	}


	//	IClientRenderable* renderable = ply->GetClientRenderable();
	//	if (renderable)
	//	{
	//		matrix3x4_t bones[128];
	//		if (renderable->SetupBones(bones, 128, BONE_USED_BY_HITBOX, I::gpGlobals->curtime))
	//		{
	//			Vector headPos = bones->GetVector(8);
	//			if (I::DebugOverlay->ScreenPosition(headPos, screenpos) != 1)
	//			{
	//				float lenSqr = screenpos.Dist2DSqr({ w / 2.f, h / 2.f });
	//				if (lenSqr < (180 * 180))
	//				{
	//					Draw::DrawLine({ w / 2.f, h / 2.f }, { screenpos.x, screenpos.y }, color, 1);
	//				}
	//			}

	//			bool isCT = ply->GetTeam() == TEAM_CT;
	//			D3DCOLOR color = isCT ? D3DCOLOR_RGBA(0, 0, 255, 255) : D3DCOLOR_RGBA(255, 0, 0, 255);
	//		}
	//	}
	//}

	Menu::Run(pDevice);
	
	return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}