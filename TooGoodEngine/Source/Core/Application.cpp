#include "Application.h"

#include <vector>

namespace TooGoodEngine {

	void Application::Run()
	{
		m_MemoryAllocator.Insert<int>(5);
		m_MemoryAllocator.Emplace<int>(10);
		m_MemoryAllocator.Insert<int>(15);

		std::cout << m_MemoryAllocator.GetElement<int>(0) << std::endl;

		m_MemoryAllocator.Remove<int>(0);

		std::cout << m_MemoryAllocator.GetElement<int>(0) << std::endl;

		std::cout << *m_MemoryAllocator.Begin<int>() << std::endl;
		std::cout << *m_MemoryAllocator.End<int>() << std::endl;
	}
}