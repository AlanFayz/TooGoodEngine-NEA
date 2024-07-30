#pragma once

#include "ECS/DenseMap.h"

namespace TooGoodEngine {
	
	class Application
	{
	public:
		Application() = default;
		~Application() = default;

		void Run();

	private:
		DenseMap m_DenseMap;
	};

}

