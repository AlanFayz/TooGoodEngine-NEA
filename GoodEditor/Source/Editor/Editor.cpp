#include "Editor.h"

#include <imgui.h>

namespace GoodEditor {

	using namespace TooGoodEngine;

	void Editor::OnCreate()
	{
	}
	void Editor::OnDestroy()
	{
	}
	void Editor::OnUpdate(double delta)
	{
	}
	void Editor::OnGuiUpdate(double delta)
	{
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
	}
	void Editor::OnEvent(TooGoodEngine::Event* event)
	{
	}
}