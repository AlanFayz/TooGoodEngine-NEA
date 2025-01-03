#include "Timer.h"

namespace TooGoodEngine {

	void Timer::Start()
	{
		m_Start = m_Clock.now();
	}

	int64_t Timer::ElapsedMilli()
	{
		m_End = m_Clock.now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(m_End - m_Start).count();
	}

	double Timer::ElapsedSeconds()
	{
		m_End = m_Clock.now();
		return std::chrono::duration_cast<std::chrono::duration<double>>(m_End - m_Start).count();
	}

}