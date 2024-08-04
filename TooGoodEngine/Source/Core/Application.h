#pragma once

#include "Window.h"
#include "Layers/LayerStack.h"
#include "Utils/Timer.h"

#include "API/OpenGL/Command.h"


namespace TooGoodEngine {

	class Application : public EventHandler
	{
	public:
		Application();
		~Application();

		void AddLayer(const Ref<Layer>& layer);
		void RemoveLayer(const Ref<Layer>& layer);
		void Run();

		virtual void OnEvent(Event* event);

	private:
		LayerStack m_LayerStack;
		EventDispatcher<Application> m_Dispatcher;

		bool m_Runnning = true;
		Window m_Window;
		Timer m_Timer;
	};

}

