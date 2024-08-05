#include "Application.h"

#include "Scripting/ScriptingEngine.h"
#include "Utils/Input.h"
#include "Math/PerspectiveCamera.h"

namespace TooGoodEngine {
	
	static RenderSettings s_Settings{ 600, 600, {0.0f, 0.0f, 0.0f, 0.0f} };

	Application::Application()
		: m_Dispatcher(this), m_Window(600, 600, "window", m_Dispatcher), m_TestRenderer(s_Settings)
	{
		ScriptingEngine::Init();
		Input::Init(m_Window.GetWindow());
	}

	Application::~Application()
	{
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
	
		glm::mat4 transform = glm::identity<glm::mat4>();
		transform = glm::translate(transform, { 0.0f, 0.0f, -1.0f }) *
			glm::rotate(transform, 0.0f, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(transform, { 1.0f, 1.0f, 1.0f });

		PerspectiveCameraData data{};
		PerspectiveCamera camera(data);

		while (m_Runnning)
		{
			m_Timer.Start();

			m_LayerStack.OnUpdateLayers(delta);
			m_LayerStack.OnGuiUpdateLayers(delta);

			m_Window.Update();

			delta = (double)m_Timer.EllapsedMilli();
			delta /= 1000.0;

			m_TestRenderer.Begin(&camera);
			m_TestRenderer.DrawQuad(transform);
			m_TestRenderer.End();
		}
	}

	void Application::OnEvent(Event* event)
	{
		if (event->GetType() == EventType::ApplicationClose)
			m_Runnning = false;

		if (event->GetType() == EventType::WindowResize)
		{
			WindowResizeEvent* windowResizeEvent = (WindowResizeEvent*)event;
			m_TestRenderer.OnWindowResize(windowResizeEvent->GetWidth(), windowResizeEvent->GetHeight());
		}

		m_LayerStack.OnEvent(event);
	}
}