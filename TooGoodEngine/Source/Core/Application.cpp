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
		//init critical systems first such as detecting input and imgui.

		Input::Init(m_Window.GetWindow());

		_InitImGui();

		//taking the layers from the application data and moving them to the application class
		for (auto& layer : data.Layers)
			AddLayer(layer);

		data.Layers.clear();
	}

	Application::~Application()
	{
		_ShutdownImGui();
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

		while (m_Running)
		{
			//swap window buffers and poll events
			m_Window.Update();

			//if the window is minimized do nothing
			if (m_Window.GetWidth() == 0 || m_Window.GetHeight() == 0)
			{
				glfwWaitEvents();
				continue;
			}

			TGE_PROFILE_SCOPE(Frame);

			//start a timer to calculate delta for this current frame
			m_Timer.Start();

			{
				//updates imgui data and calls OnGuiUpdate on the layers
				TGE_PROFILE_SCOPE(UpdateGui);
				_UpdateImGui(delta);
			}

			{
				//calls OnUpdate on the layers
				//also passes the delta of the previous frame
				TGE_PROFILE_SCOPE(UpdateLayers);
				m_LayerStack.OnUpdateLayers(delta);
			}

			//calculates the delta of the current frame in seconds
			delta = (double)m_Timer.EllapsedMilli();
			delta /= 1000.0;
		}
	}

	void Application::OnEvent(Event* event)
	{
		//if the user has closed the application we stop the application loop
		if (event->GetType() == EventType::ApplicationClose)
			m_Running = false;

		m_LayerStack.OnEvent(event);
	}
	void Application::_UpdateImGui(double delta)
	{
		//clear the screen to black
		OpenGL::Command::ClearColor({0.0f, 0.0f, 0.0f, 1.0f});

		ImGuiIO& io = ImGui::GetIO();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//calls OnGuiUpdate on the layers with the delta
		m_LayerStack.OnGuiUpdateLayers(delta);

		//renders the results of what was registered to imgui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		//if multiviewports are enabled we update the platforms with their viewports
		//and then restore the current context
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

		//default style, actual style is set in the editor.
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