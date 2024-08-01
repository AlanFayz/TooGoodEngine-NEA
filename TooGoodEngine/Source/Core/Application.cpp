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

	void Application::Run()
	{
		
	}
}