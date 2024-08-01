#include "Timer.h"

namespace TooGoodEngine {

	void Timer::Start()
	{
		m_Start = m_Clock.now();
	}

	int64_t Timer::EllapsedMilli()
	{
		m_End = m_Clock.now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(m_End - m_Start).count();
	}

}