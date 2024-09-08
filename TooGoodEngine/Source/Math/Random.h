#pragma once

#include "Core/Base.h"

#include <random>

namespace TooGoodEngine {

	template<typename Type>
	concept Arithmetic = std::is_arithmetic_v<Type>;

	class Random
	{
	public:
		Random();
		~Random() = default;
		
		template<Arithmetic T>
		T Generate(T min, T max)
		{
			//generate a random number between 0 and 1.
			//we can then scale this up based on the range
			//and add it to the min.
			double range = (double)max - (double)min;
			return min + (T)(range * m_Distribution(m_RandomEngine));
		}

	private:
		std::mt19937 m_RandomEngine;
		std::random_device m_Device;

		std::uniform_real_distribution<double> m_Distribution;
	};

}