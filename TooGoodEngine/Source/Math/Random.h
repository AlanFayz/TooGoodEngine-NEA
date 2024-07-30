#pragma once

#include "Core/Base.h"

#include <random>

namespace TooGoodEngine {

	class Random
	{
	public:
		Random();
		~Random() = default;
		
		template<typename T>
		T Generate(T min, T max)
		{
			static_assert(std::is_arithmetic_v<T>, "type must be of an arithmetic type");
			double range = (double)max - (double)min;
			return min + (T)(range * m_Distribution(m_RandomEngine));
		}

	private:
		std::mt19937 m_RandomEngine;
		std::random_device m_Device;

		std::uniform_real_distribution<double> m_Distribution;
	};

}