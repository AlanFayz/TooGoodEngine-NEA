#pragma once

#include "Window.h"
#include "Layers/LayerStack.h"
#include "Utils/Timer.h"

#include "API/OpenGL/Command.h"
#include "Scenes/Scene.h"

namespace TooGoodEngine {

	struct ApplicationData
	{
		std::vector<Ref<Layer>> Layers;
		uint32_t WindowWidth, WindowHeight;
		std::string Title;
	};

	//holds the current state of the application.
	class Application : public EventHandler
	{
	public:
		Application(ApplicationData& data);
		~Application();

		void AddLayer(const Ref<Layer>& layer);
		void RemoveLayer(const Ref<Layer>& layer);
		void Run();

		virtual void OnEvent(Event* event);

	private:
		void _InitImGui();
		void _UpdateImGui(double delta);
		void _ShutdownImGui();

	private:
		LayerStack m_LayerStack;
		EventDispatcher<Application> m_Dispatcher;

		bool m_Running = true;
		Window m_Window;
		Timer m_Timer;

	};

}

