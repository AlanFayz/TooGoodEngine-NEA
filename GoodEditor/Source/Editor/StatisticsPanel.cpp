#include "StatisticsPanel.h"

#include <imgui.h>

namespace GoodEditor {
	void StatisticsPanel::DrawPanel()
	{
		if (!ImGui::Begin("Statistics"))
		{
			ImGui::End();
			return;
		}

		const auto& statisticsToDisplay = Statistics::GetMap();

		for (const auto& [name, time] : statisticsToDisplay)
		{
			ImGui::Text("%s: %.4fms", name.data(), (float)time);
		}

		ImGui::End();
	}
}
