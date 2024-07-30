#include "Random.h"

namespace TooGoodEngine {
	Random::Random()
		: m_RandomEngine(m_Device()), m_Distribution(0.0, 1.0)
	{
	}
}