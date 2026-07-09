#pragma once

// ImGui
#include "Addons/imgui/imgui.h"

// CPP
#include <cfloat>

namespace SH
{
	// UE5-style property table: name (with tooltip) in the left column, editor in the right.
	// Pair with ImGui::EndTable(), and use BeginPropertyRow() for each property.
	inline bool BeginPropertyTable(const char* InStrId)
	{
		constexpr ImGuiTableFlags TableFlags = ImGuiTableFlags_Resizable
			| ImGuiTableFlags_BordersInnerV
			| ImGuiTableFlags_RowBg
			| ImGuiTableFlags_PadOuterX;

		if (!ImGui::BeginTable(InStrId, 2, TableFlags))
		{
			return false;
		}

		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, 0.4f);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 0.6f);
		return true;
	}

	// Start a property row: draws the name (hover for tooltip) and primes the value column
	// so the next widget stretches across it. Give the widget a hidden "##" label.
	inline void BeginPropertyRow(const char* InName, const char* InTooltip = nullptr)
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		ImGui::TextUnformatted(InName);
		if (InTooltip && ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip | ImGuiHoveredFlags_AllowWhenDisabled))
		{
			ImGui::SetTooltip("%s", InTooltip);
		}

		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
	}
}
