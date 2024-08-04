#include "Application.h"

#include "Scripting/ScriptingEngine.h"
#include "Utils/Input.h"

namespace TooGoodEngine {
	

	Application::Application()
		: m_Dispatcher(this), m_Window(1200, 800, "window", m_Dispatcher)
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

		while (m_Runnning)
		{
			m_Timer.Start();

			m_LayerStack.OnUpdateLayers(delta);
			m_LayerStack.OnGuiUpdateLayers(delta);

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
}