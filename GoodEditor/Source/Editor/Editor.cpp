#include "Editor.h"
#include "FileDialogs/FileDialog.h"

#include <imgui.h>

namespace GoodEditor {

	using namespace TooGoodEngine;

	void Editor::OnCreate()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("Resources/Fonts/Open_Sans/static/OpenSans-Regular.ttf", 20.0f);

		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameRounding    = 12;
		style.WindowRounding   = 12;
		style.ChildRounding    = 12;
		style.WindowTitleAlign = ImVec2(0.5, 0.5);

		style.Colors[2]  = ImColor(7, 0, 8, 255);       //window bg
		style.Colors[5]  = ImColor(0, 0, 0, 255);       //window border 
		style.Colors[7]  = ImColor(3, 29, 66, 128);     //frame bg 
		style.Colors[8]  = ImColor(103, 173, 255, 102); //frame bg hovered
		style.Colors[9]  = ImColor(148, 183, 255, 171); //frame bg active
		style.Colors[11] = ImColor(36, 36, 45, 255);    //title bg active
		style.Colors[18] = ImColor(146, 172, 203, 255); //check mark
		style.Colors[21] = ImColor(69, 71, 75, 102);    //button
		style.Colors[22] = ImColor(76, 94, 114, 255);   //button hovered
		style.Colors[23] = ImColor(170, 191, 211, 255); //button active
		style.Colors[25] = ImColor(67, 69, 71, 204);    //header hovered
		style.Colors[26] = ImColor(122, 117, 123, 204); //header active
		style.Colors[27] = ImColor(70, 70, 154, 128);   //header active
		style.Colors[41] = ImColor(2, 5, 20, 128);   //docking empty background
	}
	void Editor::OnDestroy()
	{
	}
	void Editor::OnUpdate(double delta)
	{
		if(g_SelectedProject)
			g_SelectedProject->GetCurrentScene()->Update(delta);

	}
	void Editor::OnGuiUpdate(double delta)
	{
		if (m_ProjectLoader)
		{
			_TryGetProjectName();
			return;
		}

		//ImGui::DockSpaceOverViewport();
	}
	void Editor::OnEvent(TooGoodEngine::Event* event)
	{
		if(g_SelectedProject)
			g_SelectedProject->GetCurrentScene()->OnEvent(event);
	}
	void Editor::_TryGetProjectName()
	{
		if (ImGui::Begin("Load Project"))
		{
			ImGui::Text("File Location: ");
			ImGui::SameLine();
			ImGui::InputText("##InputFileLocation", m_File, 256);

			if (ImGui::Button("Enter"))
			{
				if (std::filesystem::path(m_File).extension() == ".json")
				{
					m_ProjectPath = m_File;
					m_ProjectLoader = false;
					g_SelectedProject = CreateRef<Project>(m_ProjectPath);
					g_SelectedProject->LoadAllScenes();
				}
			}

			if (ImGui::Button("Open File Dialog"))
			{
				std::filesystem::path path = FileDialog::GetPathFromDialog();
				if (!path.empty() && path.extension() == ".json")
				{
					m_ProjectLoader = false;
					m_ProjectPath = path;
					g_SelectedProject = CreateRef<Project>(m_ProjectPath);
					g_SelectedProject->LoadAllScenes();
				}
			}

			ImGui::End();
		}

	}
}