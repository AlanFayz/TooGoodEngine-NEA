#pragma once

#include "MemoryAllocator.h"
#include "Entity.h"

#include <vector>

namespace TooGoodEngine {

	class DenseMap
	{
	public:
		DenseMap()  = default;
		~DenseMap() = default;


	private:
		std::vector<size_t> m_Sparse;
		MemoryAllocator m_Dense;
	};

}

