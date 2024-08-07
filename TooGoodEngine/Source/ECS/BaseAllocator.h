#pragma once

namespace TooGoodEngine {

	class BaseAllocator
	{
	public:
		virtual ~BaseAllocator() = default;

		inline virtual const size_t GetCapacity() = 0;
		inline virtual const size_t GetSize() = 0;

		virtual void Allocate(size_t newCapacity) = 0;
		virtual void Deallocate() = 0;


	};

}

