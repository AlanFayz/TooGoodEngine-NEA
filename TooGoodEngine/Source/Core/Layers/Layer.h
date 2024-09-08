#pragma once

#include "Events/Event.h"

namespace TooGoodEngine {

	//this is the base class for any "layer" that an external module
	//such as the editor/runtime can inherit from.
	//the below functions will be called as required by the engine.
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