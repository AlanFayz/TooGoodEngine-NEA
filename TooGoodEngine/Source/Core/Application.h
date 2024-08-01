#pragma once

#include "Layers/LayerStack.h"

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
		bool m_Runnning = true;
	};

}

