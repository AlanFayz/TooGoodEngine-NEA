#pragma once

#include "Core/Base.h"
#include "Math/Random.h"

namespace TooGoodEngine {

	//stands for universially unique identifer and this class is just that.
	class UUID
	{
	public:
		UUID(uint64_t other);
		UUID();
		~UUID() = default;

		inline operator uint64_t() const { return m_ID; }

	private:
		uint64_t m_ID;
	};

}