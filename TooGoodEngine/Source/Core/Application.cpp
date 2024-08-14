#include "Application.h"

#include "Scripting/ScriptingEngine.h"
#include "Utils/Input.h"
#include "Math/PerspectiveCamera.h"
#include "Scenes/Scene.h"
#include "Project/Project.h"
#include "Utils/Statistics.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace TooGoodEngine {

	Application::Application(ApplicationData& data)
		: m_Dispatcher(this), m_Window(data.WindowWidth, data.WindowHeight, data.Title, m_Dispatcher)
	{
		ScriptingEngine::Init();
		Input::Init(m_Window.GetWindow());

		_InitImGui();

		for (auto& layer : data.Layers)
			AddLayer(layer);

		data.Layers.clear();
	}

	Application::~Application()
	{
		_ShutdownImGui();
		ScriptingEngine::Shutdown();
	}

	void Application::AddLayer(const Ref<Layer>& layer)
	{
		m_LayerStack.Push(layer);
	}

	void Application::RemoveLayer(const Ref<Layer>& layer)
	{
		m_LayerStack.Remove(layer);
	}

	void Application::Run()
	{
		double delta = 0.0;

		while (m_Runnning)
		{
			TGE_PROFILE_SCOPE(Frame);

			m_Timer.Start();

			{
				TGE_PROFILE_SCOPE(UpdateLayers);
				m_LayerStack.OnUpdateLayers(delta);
			}

			{
				TGE_PROFILE_SCOPE(UpdateGui);
				_UpdateImGui(delta);
			}

			m_Window.Update();

			delta = (double)m_Timer.EllapsedMilli();
			delta /= 1000.0;
		}
	}

	void Application::OnEvent(Event* event)
	{
		if (event->GetType() == EventType::ApplicationClose)
			m_Runnning = false;

		m_LayerStack.OnEvent(event);
	}
	void Application::_UpdateImGui(double delta)
	{
		OpenGL::Command::ClearColor({0.0f, 0.0f, 0.0f, 1.0f});

		ImGuiIO& io = ImGui::GetIO();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		m_LayerStack.OnGuiUpdateLayers(delta);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
	void Application::_ShutdownImGui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void Application::_InitImGui()
	{
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL(m_Window.GetWindow(), true);
		ImGui_ImplOpenGL3_Init();
	}
}