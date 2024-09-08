#include "UUID.h"

#include "Math/Random.h"

namespace TooGoodEngine {
	//generates a random 64 bit unsigned integer
	//range is from 0 to 2^64, this makes collisions highly unlickley.

	static std::random_device s_Device;
	static std::mt19937 s_RandomEngine(s_Device());
	static std::uniform_int_distribution<uint64_t> s_Distribution;

	UUID::UUID(uint64_t other)
		: m_ID(other)
	{
	}
	UUID::UUID()
		: m_ID(s_Distribution(s_RandomEngine))
	{
	
	}
}