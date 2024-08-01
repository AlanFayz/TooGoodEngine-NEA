#include "Application.h"

#include "Scripting/ScriptingEngine.h"

namespace TooGoodEngine {

	Application::Application()
	{
		ScriptingEngine::Init();
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
			m_LayerStack.OnUpdateLayers(delta);
			m_LayerStack.OnGuiUpdateLayers(delta);
		}
	}

	void Application::OnEvent(Event* event)
	{
		if (event->GetType() == EventType::ApplicationClose)
			m_Runnning = false;

		//dispatch event to layers incase any behaviour needs to happen
		//when application is closing
		m_LayerStack.OnEvent(event);
	}

}