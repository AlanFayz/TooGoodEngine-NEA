#pragma once

#include "Base.h"
#include "Application.h"

namespace TooGoodEngine {

	int EngineMain(ApplicationData& data)
	{
		Application app(data);
		app.Run();

		return 0;
	}

}