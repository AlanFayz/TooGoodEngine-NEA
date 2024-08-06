#include "Editor.h"
#include "FileDialogs/FileDialog.h"

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
				}
			}

			ImGui::End();
		}

	}
}