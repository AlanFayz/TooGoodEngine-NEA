#pragma once

#include "Base.h"
#include "Application.h"

namespace TooGoodEngine {

	int EngineMain()
	{
		Application app;
		app.Run();

		return 0;
	}

}