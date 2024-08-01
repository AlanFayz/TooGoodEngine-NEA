#pragma once

#include <chrono>

namespace TooGoodEngine {

	class Timer
	{
	public:
		Timer() = default;
		~Timer() = default;

		void Start();
		int64_t EllapsedMilli();

	private:
		std::chrono::steady_clock m_Clock;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_End;
	};

}