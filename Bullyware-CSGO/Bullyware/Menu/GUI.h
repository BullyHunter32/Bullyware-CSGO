#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

namespace GUI
{
    inline IDirect3DTexture9* pStandingTexture = nullptr;
    inline IDirect3DTexture9* pLyingTexture = nullptr;

    struct
    {
        ImFont* Verdana;
        ImFont* VerdanaBold;
    }Fonts;

    inline ImVec4 LerpColor(const ImVec4& from, const ImVec4& to, float frac)
    {
        if (frac >= 1)
            return to;
        if (frac <= 0)
            return from;

        return ImVec4(
            from.x + (to.x - from.x) * frac,
            from.y + (to.y - from.y) * frac,
            from.z + (to.z - from.z) * frac,
            from.w + (to.w - from.w) * frac
        );
    }

#define GUI_NAVBAR_HEIGHT 32
	inline void BeginNavbar()
	{
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImDrawList* list = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetWindowPos();
		
        ImGui::BeginGroup();

        ImGui::PushFont(Fonts.VerdanaBold);
        ImVec2 size = ImGui::CalcTextSize("Bullyware");
        list->AddText(ImVec2(pos.x + 12, pos.y + (GUI_NAVBAR_HEIGHT/2.f) - (size.y/2.f)), ImColor(255, 255, 255, 210), "Bullyware");
        ImGui::PopFont();
        
        list->AddRectFilledMultiColor(ImVec2(pos.x, pos.y), ImVec2(pos.x + windowSize.x, pos.y + 16), ImColor(80, 80, 80, 80), ImColor(80, 80, 80, 80), ImColor(125, 125, 125, 0), ImColor(125, 125, 125, 0));

        list->AddLine(ImVec2(pos.x, pos.y + GUI_NAVBAR_HEIGHT + 1), ImVec2(pos.x + windowSize.x, pos.y + GUI_NAVBAR_HEIGHT + 1), ImColor(88, 88, 88, 88));

        list = ImGui::GetForegroundDrawList();
        ImGui::SetCursorPosX(12 + size.x + 14);


        float textureWidth = windowSize.x * 0.33;
        float textureHeight = textureWidth * (1296.f / 716.f);
        float sitAdjustmentY = textureHeight * 0.43f;

        // ImGui::Image(pTexture, ImVec2(378, 663));
        list->AddImage(pLyingTexture, ImVec2(pos.x + windowSize.x - 10 - textureWidth, pos.y - textureHeight + sitAdjustmentY), ImVec2(pos.x + windowSize.x - 10, pos.y + sitAdjustmentY));
	}

	inline void EndNavbar()
	{
		ImGui::EndGroup();
	}

    struct checkbox_t
    {
        const char* szLabel;
        float flHover;
    };

    inline bool Checkbox(checkbox_t& data, bool* v)
    {
        constexpr float rounding = 4;

        using namespace ImGui;

        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(data.szLabel);
        const ImVec2 label_size = CalcTextSize(data.szLabel, NULL, true);

        const float square_sz = GetFrameHeight();
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
        ItemSize(total_bb, style.FramePadding.y);
        if (!ItemAdd(total_bb, id))
        {
            IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
            return false;
        }

        bool hovered, held;
        bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
        if (pressed)
        {
            *v = !(*v);
            MarkItemEdited(id);
        }

        ImGuiIO& io = GetIO();
        if (*v)
        {
            if (data.flHover < 1.f)
            {
                data.flHover += io.DeltaTime * 8.f;
                if (data.flHover > 1.f)
                    data.flHover = 1.f;
            }
        }
        else if (data.flHover != 0.f)
        {
            data.flHover -= io.DeltaTime * 8.f;
            if (data.flHover < 0.f)
                data.flHover = 0.f;
        }

        ImColor bgColor = (held && hovered) ? ImColor(93, 93, 97) : hovered ? ImColor(72, 72, 76) : ImColor(64, 64, 64);
        // (held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg)
        const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
        RenderNavHighlight(total_bb, id);
        RenderFrame(check_bb.Min, check_bb.Max, ImColor(54, 54, 54, 110), true, rounding);
        ImDrawList* list = ImGui::GetWindowDrawList();

        ImColor cornercol = LerpColor(ImColor(54, 75, 99, 120), ImColor(60, 85, 105, 180), data.flHover);
        ImColor topleft = ImColor(48, 58, 88, 150);

        list->AddRectFilledMultiColor(check_bb.Min, check_bb.Max, topleft, cornercol, cornercol, cornercol);
        ImColor check_col = LerpColor(ImColor(54, 140, 188, 0), ImColor(60, 140, 195, 255), data.flHover); //GetColorU32(ImGuiCol_CheckMark);
        bool mixed_value = (g.LastItemData.InFlags & ImGuiItemFlags_MixedValue) != 0;
        if (mixed_value)
        {
            // Undocumented tristate/mixed/indeterminate checkbox (#2644)
            // This may seem awkwardly designed because the aim is to make ImGuiItemFlags_MixedValue supported by all widgets (not just checkbox)
            ImVec2 pad(ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)), ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)));
            window->DrawList->AddRectFilled(check_bb.Min + pad, check_bb.Max - pad, check_col, rounding);
        }
        else if (*v || data.flHover != 0.f)
        {
            const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
            RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), check_col, square_sz - pad * 2.0f);
        }

        ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);
        if (g.LogEnabled)
            LogRenderedText(&label_pos, mixed_value ? "[~]" : *v ? "[x]" : "[ ]");
        if (label_size.x > 0.0f)
            RenderText(label_pos, data.szLabel);

        IMGUI_TEST_ENGINE_ITEM_INFO(id, szLabel, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return pressed;
    }

    inline void BeginSection(const char* szID, const char* szLabel)
    {
        //ImGuiStyle& style = ImGui::GetStyle();
        //style.WindowPadding = ImVec2(8, 8);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));

        ImDrawList* list = ImGui::GetWindowDrawList();
        ImGui::BeginChild(szID, ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImVec2& mins = window->ClipRect.Min;
        ImVec2& maxs = window->InnerClipRect.Max; // mins + window->ContentSize;
        // list->AddRect(mins, maxs, ImColor(64, 66, 72, 166), 4.f, 0, 2.f);

        static constexpr float thickness = 1.f;
        static ImColor color = ImColor(64, 66, 72, 166);

        ImVec2 textSize = ImGui::CalcTextSize(szLabel);

        float marginX = 4.f;

        float width = maxs.x - mins.x;

        list->AddLine({ mins.x, mins.y }, { mins.x + (width/2.f) - (textSize.x / 2.f) - marginX, mins.y }, color, thickness); // tl text
        list->AddLine({ mins.x + (width / 2.f) + (textSize.x / 2.f) + marginX, mins.y }, { maxs.x, mins.y }, color, thickness); // text tr
        list->AddLine({ maxs.x, mins.y }, { maxs.x, maxs.y }, color, thickness); // tr br
        list->AddLine({ maxs.x, maxs.y }, { mins.x, maxs.y }, color, thickness); // br bl
        list->AddLine({ mins.x, maxs.y }, { mins.x, mins.y }, color, thickness); // bl tl

        list->AddText({ mins.x + width / 2.f - textSize.x / 2.f, mins.y - textSize.y/2.f }, ImColor(255, 255, 255), szLabel);
    }

    inline void EndSection()
    {
        ImGui::EndChild();
        //ImGuiStyle& style = ImGui::GetStyle();
        //style.WindowPadding = ImVec2(0, 0);
        ImGui::PopStyleVar();
    }

	struct navtab_t
	{
		const char* szLabel;
		bool* pSelected;
		float flHover = 0.f;
	};

    inline bool NavTab(navtab_t& tab, ImGuiSelectableFlags flags, bool selected)
    {
        using namespace ImGui;
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImVec2 size_arg = CalcTextSize(tab.szLabel);
        size_arg.x += 8;
        size_arg.y = GUI_NAVBAR_HEIGHT;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        const char* label = tab.szLabel;
        // bool selected = (tab.pSelected != nullptr) ? *tab.pSelected : false;

        // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
        ImGuiID id = window->GetID(label);
        ImVec2 label_size = CalcTextSize(label, NULL, true);
        ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
        ImVec2 pos = window->DC.CursorPos;
        pos.y += window->DC.CurrLineTextBaseOffset;
        ItemSize(size, 0.0f);

        // Fill horizontal space
        // We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
        const bool span_all_columns = false; // (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
        const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
        const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
        if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
            size.x = ImMax(label_size.x, max_x - min_x);

        // Text stays at the submission position, but bounding box may be extended on both sides
        const ImVec2 text_min = pos;
        const ImVec2 text_max(min_x + size.x, pos.y + size.y);

        // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
        ImRect bb(min_x, pos.y, text_max.x, text_max.y);
        if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
        {
            const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
            const float spacing_y = style.ItemSpacing.y;
            const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
            const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
            bb.Min.x -= spacing_L;
            bb.Min.y -= spacing_U;
            bb.Max.x += (spacing_x - spacing_L);
            bb.Max.y += (spacing_y - spacing_U);
        }
        //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

        // Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackground for every Selectable..
        const float backup_clip_rect_min_x = window->ClipRect.Min.x;
        const float backup_clip_rect_max_x = window->ClipRect.Max.x;
        if (span_all_columns)
        {
            window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
            window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
        }

        const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
        const bool item_add = ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
        if (span_all_columns)
        {
            window->ClipRect.Min.x = backup_clip_rect_min_x;
            window->ClipRect.Max.x = backup_clip_rect_max_x;
        }

        if (!item_add)
            return false;

        const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
        if (disabled_item && !disabled_global) // Only testing this as an optimization
            BeginDisabled();

        // FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
        // which would be advantageous since most selectable are not selected.
        if (span_all_columns && window->DC.CurrentColumns)
            PushColumnsBackground();
        else if (span_all_columns && g.CurrentTable)
            TablePushBackgroundChannel();

        // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
        ImGuiButtonFlags button_flags = 0;
        if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
        if (flags & ImGuiSelectableFlags_NoSetKeyOwner) { button_flags |= ImGuiButtonFlags_NoSetKeyOwner; }
        if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
        if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
        if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
        if (flags & ImGuiSelectableFlags_AllowItemOverlap) { button_flags |= ImGuiButtonFlags_AllowItemOverlap; }

        const bool was_selected = selected;
        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

        ImGuiIO& io = GetIO();
        if ((selected || hovered))
        {
            
            if (tab.flHover < 1.f)
            {
                tab.flHover += io.DeltaTime * 5.f;
                if (tab.flHover > 1.f)
                    tab.flHover = 1.f;
            }
        }
        else if (tab.flHover != 0.f)
        {
            tab.flHover -= io.DeltaTime * 6.f;
            if (tab.flHover < 0.f)
                tab.flHover = 0.f;
        }

        // Auto-select when moved into
        // - This will be more fully fleshed in the range-select branch
        // - This is not exposed as it won't nicely work with some user side handling of shift/control
        // - We cannot do 'if (g.NavJustMovedToId != id) { selected = false; pressed = was_selected; }' for two reasons
        //   - (1) it would require focus scope to be set, need exposing PushFocusScope() or equivalent (e.g. BeginSelection() calling PushFocusScope())
        //   - (2) usage will fail with clipped items
        //   The multi-select API aim to fix those issues, e.g. may be replaced with a BeginSelection() API.
        if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId)
            if (g.NavJustMovedToId == id)
                selected = pressed = true;

        // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
        if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
        {
            if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
            {
                SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, WindowRectAbsToRel(window, bb)); // (bb == NavRect)
                g.NavDisableHighlight = true;
            }
        }
        if (pressed)
            MarkItemEdited(id);

        if (flags & ImGuiSelectableFlags_AllowItemOverlap)
            SetItemAllowOverlap();

        // In this branch, Selectable() cannot toggle the selection so this will never trigger.
        if (selected != was_selected) //-V547
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;


        // Render
        // if (hovered || selected)
        {
            // const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
            const ImColor col = LerpColor(ImColor(24, 24, 28, 0), ImColor(42, 42, 48), tab.flHover);

            ImGuiContext& g = *GImGui;
            ImGuiWindow* window = g.CurrentWindow;
            // window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding);

            ImColor top = LerpColor(ImColor(65, 89, 106, 0), ImColor(65, 89, 106, 0), tab.flHover);
            ImColor bottom = LerpColor(ImColor(89, 102, 136, 0), ImColor(89, 102, 136, 84), tab.flHover);

            window->DrawList->AddRectFilledMultiColor(ImVec2(bb.Min.x, bb.Min.y + 16), bb.Max, top, top, bottom, bottom);
            // RenderFrame(bb.Min, bb.Max, col, false, 0.0f);
        }
        RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);

        if (span_all_columns && window->DC.CurrentColumns)
            PopColumnsBackground();
        else if (span_all_columns && g.CurrentTable)
            TablePopBackgroundChannel();

        ImGui::PushStyleColor(ImGuiCol_Text, GUI::LerpColor(ImColor(199, 199, 202, 220), ImColor(252, 252, 255, 255), tab.flHover));
        RenderTextClipped(text_min, text_max, label, NULL, &label_size, ImVec2(0.5, 0.5), &bb);
        ImGui::PopStyleColor();

        // Automatically close popups
        if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.LastItemData.InFlags & ImGuiItemFlags_SelectableDontClosePopup))
            CloseCurrentPopup();

        if (disabled_item && !disabled_global)
            EndDisabled();

        if (tab.pSelected)
            *tab.pSelected = selected;

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
        return pressed; //-V1020
    }
}