#include "Editor.h"
#include "Platform/Platform.h"
#include "ScenePanel.h"
#include "StatisticsPanel.h"
#include "PropertiesPanel.h"
#include "Utils/Input.h"

#include <imgui.h>

namespace GoodEditor {

	using namespace TooGoodEngine;

	void Editor::OnCreate()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("Resources/Fonts/Open_Sans/static/OpenSans-Regular.ttf", 20.0f);

		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameRounding = 12;
		style.WindowRounding = 12;
		style.ChildRounding = 12;
		style.WindowTitleAlign = ImVec2(0.5, 0.5);

		style.Colors[2] = ImColor(7, 0, 8, 255);       //window bg
		style.Colors[5] = ImColor(0, 0, 0, 255);       //window border 
		style.Colors[7] = ImColor(3, 29, 66, 128);     //frame bg 
		style.Colors[8] = ImColor(103, 173, 255, 102); //frame bg hovered
		style.Colors[9] = ImColor(148, 183, 255, 171); //frame bg active
		style.Colors[11] = ImColor(36, 36, 45, 255);    //title bg active
		style.Colors[18] = ImColor(146, 172, 203, 255); //check mark
		style.Colors[21] = ImColor(69, 71, 75, 102);    //button
		style.Colors[22] = ImColor(76, 94, 114, 255);   //button hovered
		style.Colors[23] = ImColor(170, 191, 211, 255); //button active
		style.Colors[25] = ImColor(67, 69, 71, 204);    //header hovered
		style.Colors[26] = ImColor(122, 117, 123, 204); //header active
		style.Colors[27] = ImColor(70, 70, 154, 128);   //header active
		style.Colors[41] = ImColor(2, 5, 20, 128);      //docking empty background

		m_ExtensionMap["folder"] = Image::LoadImageAssetFromFile("Resources/Textures/folder_icon.png");
		m_ExtensionMap["back"]	 = Image::LoadImageAssetFromFile("Resources/Textures/back_icon.png");
		m_ExtensionMap[".png"]	 = Image::LoadImageAssetFromFile("Resources/Textures/image_icon.png");
		m_ExtensionMap[".obj"]	 = Image::LoadImageAssetFromFile("Resources/Textures/obj_icon.png");
		m_ExtensionMap[".fbx"]	 = Image::LoadImageAssetFromFile("Resources/Textures/fbx_icon.png");
		m_ExtensionMap[".hdr"]	 = Image::LoadImageAssetFromFile("Resources/Textures/hdr_icon.png");
		m_ExtensionMap[".py"]	 = Image::LoadImageAssetFromFile("Resources/Textures/python_icon.png");

	}
	void Editor::OnDestroy()
	{
		if(g_SelectedProject)
			g_SelectedProject->SaveState();
	}
	void Editor::OnUpdate(double delta)
	{
		if (g_SelectedProject && !m_Playing)
			g_SelectedProject->GetCurrentScene()->Update(delta);
		else if (g_SelectedProject && m_Playing)
			g_SelectedProject->GetCurrentScene()->Play(delta);

	}
	void Editor::OnGuiUpdate(double delta)
	{
		if (m_ProjectLoader)
		{
			_TryGetProjectName();
			return;
		}

		if (m_QueueRecreation && m_Playing)
		{

			//when played project gets saved to disk. The player can completly mess up
			//every single scene. We then destroy the current project without saving. Then reload from disk
			//preserving the state before it was played.

			g_SelectedProject.reset();
			g_SelectedProject = CreateRef<Project>(m_ProjectPath);
			g_SelectedProject->LoadProject();

			m_Playing = false;
			m_QueueRecreation = false;
		}

		Ref<Scene>    currentScene = g_SelectedProject->GetCurrentScene();
		Ref<Renderer> currentSceneRenderer = currentScene->GetSceneRenderer();

		if (m_PreviousWindowSize.x != ImGui::GetWindowSize().x || m_PreviousWindowSize.y != ImGui::GetWindowSize().y)
		{
			ViewportResizeEvent event((uint32_t)ImGui::GetWindowSize().x, (uint32_t)ImGui::GetWindowSize().y);
			OnEvent(&event);

			m_PreviousWindowSize = ImGui::GetWindowSize();
		}


		ImGui::DockSpaceOverViewport();
		
		AssetPanel::DrawPanel(m_ExtensionMap);
		ScenePanel::DrawPanel();
		StatisticsPanel::DrawPanel();
		PropertiesPanel::DrawPanel();

		auto& image = currentSceneRenderer->GetImage();

		_RenderViewport(image);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					g_SelectedProject->SaveState();
				}

				ImGui::EndMenu();
			}


			if (ImGui::MenuItem("Build"))
			{
				try
				{
					std::filesystem::path directory = Platform::GetDirectoryFromDialog();

					if (!directory.empty() && std::filesystem::exists(directory))
						g_SelectedProject->Build(directory);
				}
				catch (const std::exception& e)
				{
					TGE_LOG_INFO("failed to build ", e.what());
				}
				
			}


			ImGui::EndMainMenuBar();
		}

		if (!ImGui::Begin("Scene Camera Settings"))
		{
			ImGui::End();
			return;
		}
		
		static float s_CameraSpeed = 100.0f;
		if (ImGui::DragFloat("Camera Speed", &s_CameraSpeed, 0.01f, 0.0f, FLT_MAX / 2.0f))
		{
			currentScene->GetSceneCameraController().SetCameraSpeed(s_CameraSpeed);
			currentScene->GetSceneCameraController2D().SetCameraSpeed(s_CameraSpeed);
		}

		static float s_Sensitivity = 1.0f;
		if (ImGui::DragFloat("Camera Sensitivity", &s_Sensitivity, 0.01f, 0.0f, FLT_MAX / 2.0f))
		{
			currentScene->GetSceneCameraController().SetCameraSensitivity(s_Sensitivity);
		}

		ImGui::End();
		
	}
	void Editor::OnEvent(Event* event)
	{
		if(g_SelectedProject)
			g_SelectedProject->GetCurrentScene()->OnEvent(event);

		if (event->GetType() == EventType::WindowResize)
		{
			WindowResizeEvent* resizeEvent = (WindowResizeEvent*)event;
			m_CurrentWidth = resizeEvent->GetWidth();
			m_CurrentHeight = resizeEvent->GetHeight();
		}
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
				std::filesystem::path path(m_File);
				if (path.extension() == ".json" && std::filesystem::exists(path))
				{
					m_ProjectPath = m_File;
					m_ProjectLoader = false;
					
					try
					{
						g_SelectedProject = CreateRef<Project>(m_ProjectPath);
						g_SelectedProject->LoadProject();
					}
					catch (const std::exception& e)
					{
						TGE_LOG_INFO(e.what());
						g_SelectedProject = nullptr;
						m_ProjectLoader = true;
					}
					
				}
			}

			if (ImGui::Button("Open File"))
			{
				std::filesystem::path path = Platform::GetPathFromDialog();
				if (path.extension() == ".json" && std::filesystem::exists(path))
				{
					m_ProjectLoader = false;
					m_ProjectPath = path;

					try
					{
						g_SelectedProject = CreateRef<Project>(m_ProjectPath);
						g_SelectedProject->LoadProject();
					}
					catch (const std::exception& e)
					{
						TGE_LOG_INFO(e.what());
						g_SelectedProject = nullptr;
						m_ProjectLoader = true;
					}
				}
			}

			if (ImGui::Button("Create New"))
			{
				std::filesystem::path path = Platform::GetDirectoryFromDialog();
				if (!path.empty())
				{
					m_ProjectLoader = false;
					g_SelectedProject = CreateRef<Project>(path.filename().string(), path);
				}
			}

			ImGui::End();
		}

	}
	void Editor::_RenderViewport(const Ref<OpenGL::Texture2D>& image)
	{
		if (!ImGui::Begin("Viewport", (bool*)0, ImGuiWindowFlags_MenuBar))
		{
			ImGui::End();
			return;
		}
		
		ImGui::Image((ImTextureID)(intptr_t)image->GetHandle(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

		_RenderViewportMenuBar();

		ImGui::End();
		
	}
	void Editor::_RenderViewportMenuBar()
	{
		if (ImGui::BeginMenuBar())
		{  
			if (ImGui::Button("2D View"))
			{
				g_SelectedProject->GetCurrentScene()->SetSceneView(SceneView::View2D);
			}

			if (ImGui::Button("3D View"))
			{
				g_SelectedProject->GetCurrentScene()->SetSceneView(SceneView::View3D);
			}

			if (ImGui::Button("Play"))
			{
				if (!m_Playing)
				{
					g_SelectedProject->SaveState();
					m_Playing = true;
				}
			}

			if(ImGui::Button("Stop"))
			{
				m_QueueRecreation = true;
			}

			ImGui::EndMenuBar();
		}
	}
}