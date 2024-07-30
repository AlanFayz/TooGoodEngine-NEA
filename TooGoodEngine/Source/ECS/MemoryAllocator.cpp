#include "MemoryAllocator.h"

#include "Core/Base.h"

#include <memory>

namespace TooGoodEngine {

	MemoryAllocator::MemoryAllocator()
		: m_Buffer(nullptr), m_Capacity(0), m_Size(0)
	{
	}

	MemoryAllocator::MemoryAllocator(size_t capacity)
		: m_Buffer(nullptr), m_Capacity(capacity), m_Size(0)
	{
		Allocate(m_Capacity);
	}

	MemoryAllocator::~MemoryAllocator()
	{
		if (m_Buffer)
			Deallocate();
	}

	void MemoryAllocator::Allocate(size_t newBytes)
	{
		if (m_Buffer)
			Deallocate();

		m_Buffer = _Allocate(newBytes);
		m_Capacity = newBytes;
	}

	void MemoryAllocator::Deallocate()
	{
		_Deallocate(m_Buffer);
		m_Buffer = nullptr;
	}

	void MemoryAllocator::Reallocate(size_t newBytes)
	{
		//if buffer is already null then we can just allocate memory normally
		if (!m_Buffer)
		{
			Allocate(newBytes);
			return;
		}

		void* newBuffer = _Allocate(newBytes);

		memcpy_s(newBuffer, newBytes, m_Buffer, m_Capacity);

		Deallocate();

		m_Buffer = newBuffer;
		m_Capacity = newBytes;
	}

	void* MemoryAllocator::_Allocate(size_t bytes)
	{
		TGE_VERIFY(bytes > 0, "bytes needs to be bigger than 0");
		return ::operator new(bytes);
	}
	void MemoryAllocator::_Deallocate(void* buffer)
	{
		return ::operator delete(buffer);
	}
}