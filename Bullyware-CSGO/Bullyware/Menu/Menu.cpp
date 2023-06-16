#include "Menu.h"
#include "../SDK/Interfaces.h"
#include "GUI.h"
#include "../Config/Settings.h"

#define GUI_CHECKBOX(label, configid) \
	{ static GUI::checkbox_t checkbox = {label, 0}; \
	GUI::Checkbox(checkbox, &Config.Get<bool>(configid)); }

void Menu::Run(IDirect3DDevice9* pDevice)
{
	Initialize(pDevice);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (GetAsyncKeyState(VK_INSERT) & 1) MenuOpen = !MenuOpen;

	if (MenuOpen)
		Render(pDevice);

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

const ImVec2 WINDOW_SIZE = ImVec2(583.f, 396.f);
void Menu::Render(IDirect3DDevice9* pDevice)
{
	ImGuiViewport* vp = ImGui::GetMainViewport();
	ImDrawList* list = ImGui::GetBackgroundDrawList();
	list->AddRectFilled(vp->Pos, vp->Size, ImColor(16, 16, 16, 140), 0);

	ImGui::SetNextWindowSize(WINDOW_SIZE);
	ImGui::Begin("MainWindow", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	{
		GUI::BeginNavbar();
		{
			static GUI::navtab_t* activeTab = nullptr;

			static GUI::navtab_t visuals = { "Visuals", nullptr, 0.f };
			if (GUI::NavTab(visuals, 0, activeTab == &visuals)) { activeTab = &visuals; } ImGui::SameLine();

			static GUI::navtab_t aimbot = { "Aimbot", nullptr, 0.f };
			if (GUI::NavTab(aimbot, 0, activeTab == &aimbot)) { activeTab = &aimbot; }
		}
		GUI::EndNavbar();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
		ImGui::BeginChild("##Body", ImVec2(), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
		{
			ImGui::Columns(2, nullptr, false);
			GUI::BeginSection("##Player", "Player");
			{
				GUI_CHECKBOX("Dynamic Boxes", "Player.DynamicBoxes");
				GUI_CHECKBOX("Player Name", "Player.Name");
				GUI_CHECKBOX("Player Health", "Player.Health");
			}
			GUI::EndSection();
			ImGui::NextColumn();
			GUI::BeginSection("##VisualFilter", "Filters");
			{
				GUI_CHECKBOX("Ignore Dead", "Filter.Dead");
				GUI_CHECKBOX("Ignore Team", "Filter.Team");
				GUI_CHECKBOX("Ignore Dormant", "Filter.Dormant");
			}
			GUI::EndSection();
		}
		ImGui::EndChild();
		ImGui::PopStyleVar(); // ImGuiStyleVar_WindowPadding
	}
	ImGui::End();

	int scrw, scrh;
	I::EngineClient->GetScreenSize(scrw, scrh);

	float textureHeight = scrh * 0.5f;
	float textureWidth = textureHeight * (378.f / 663.f);

	// ImGui::Image(pTexture, ImVec2(378, 663));
	list->AddImage(GUI::pStandingTexture, ImVec2(scrw * 0.2, scrh - textureHeight), ImVec2(scrw * 0.2 + textureWidth, scrh));
}