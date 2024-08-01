#pragma once

#include "Scripting/ScriptComponent.h"

namespace TooGoodEngine {
	
	class Application
	{
	public:
		Application();
		~Application();

		void Run();

	private:
		ScriptComponent m_Component;
	};

}

