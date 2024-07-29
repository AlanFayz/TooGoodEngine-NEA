#pragma once

#include "Core/ECS/MemoryAllocator.h"

namespace TooGoodEngine {
	
	class Application
	{
	public:
		Application() = default;
		~Application() = default;

		void Run();

	private:
		MemoryAllocator m_MemoryAllocator;
	};

}

