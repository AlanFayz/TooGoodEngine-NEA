#pragma once

#include "Events/Event.h"

namespace TooGoodEngine {

	class Layer : public EventHandler
	{
	public:
		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		virtual void OnUpdate(double delta) {};
		virtual void OnGuiUpdate(double delta) {};
		virtual void OnEvent(Event* event) {};
	};

}