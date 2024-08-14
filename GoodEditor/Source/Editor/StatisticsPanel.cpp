#include "StatisticsPanel.h"

#include <imgui.h>

namespace GoodEditor {
	void StatisticsPanel::DrawPanel()
	{
		//there may be more things to do in the future but for now this is good.

		ImGui::Begin("Statistics");

		const auto& statisticsToDisplay = Statistics::GetMap();

		for (const auto& [name, time] : statisticsToDisplay)
		{
			ImGui::Text("%s: %.4fms", name.data(), (float)time);
		}

		ImGui::End();
	}
}
