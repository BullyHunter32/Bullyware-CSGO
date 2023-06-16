#pragma once

#include "../SDK/Interfaces.h"
#include "../Render/Drawing.h"
#include "../Config/Settings.h"

namespace F::Visuals
{


	inline void DrawEntity(C_Entity* pEntity, int& iEntIndex)
	{
		Vector screenMins, screenMaxs;

		Vector& origin = pEntity->GetABSOrigin();
		Vector mins = origin + pEntity->GetCollisionProperties()->OBBMins();
		Vector maxs = origin + pEntity->GetCollisionProperties()->OBBMaxs();

		Draw::GetRenderMinsMaxs(mins, maxs, screenMins, screenMaxs);

		if (!pEntity->IsPlayer())
		{
			return;
		}


		if (Config.Get<bool>("Filter.Dead") && pEntity->GetHealth() <= 0)
			return;

		if (Config.Get<bool>("Player.DynamicBoxes"))
			Draw::DrawBounds(screenMins, screenMaxs, D3DCOLOR_RGBA(255, 0, 0, 255), 1.f);

		player_info_t player_info;
		if (Config.Get<bool>("Player.Name") && I::EngineClient->GetPlayerInfo(iEntIndex, &player_info))
			Draw::DrawTextAOutlined(player_info.name, screenMins.x + (screenMaxs.x - screenMins.x) / 2.f, screenMins.y - 1.f, D3DCOLOR_RGBA(255, 255, 255, 255), 1, 0xFF000000, DT_BOTTOM | DT_CENTER);

		if (Config.Get<bool>("Player.Health"))
			Draw::DrawTextAOutlined(std::to_string(pEntity->GetHealth()).c_str(), screenMins.x + (screenMaxs.x - screenMins.x) / 2.f, screenMaxs.y + 1.f, D3DCOLOR_RGBA(255, 255, 255, 255), 1, 0xFF000000, DT_CENTER);

	}


	inline void Run()
	{
		C_Entity* pLocalPlayer = I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer());

		if (!pLocalPlayer) return;
		for (int i = 1; i <= I::gpGlobals->maxClients; i++)
		{
			C_Entity* ent = I::EntityList->GetClientEntity(i);
			if (!ent) continue;

			DrawEntity(ent, i);
		}
	}
}