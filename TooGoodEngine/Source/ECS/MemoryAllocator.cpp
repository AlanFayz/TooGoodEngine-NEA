#include "MemoryAllocator.h"

#include "Core/Base.h"

#include <memory>

namespace TooGoodEngine {

	MemoryAllocator::MemoryAllocator()
		: m_Buffer(nullptr), m_Capacity(0), m_Size(0)
	{
	}

	MemoryAllocator::~MemoryAllocator()
	{
		//Better for user to call Deallocate<T> manually so that elements destructors are called.
		if (m_Buffer)
			_Deallocate(m_Buffer);
	}


	void MemoryAllocator::_Deallocate(void* buffer)
	{
		return _aligned_free(buffer);
	}
}