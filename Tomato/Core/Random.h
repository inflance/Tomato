#pragma once

#include <random>
#include "Singleton.h"

namespace Tomato
{
	class Random : public Singleton<Random>
	{
	public:
		void Init()
		{
			s_random_engine.seed(std::random_device()());
		}

		double GetDouble(double min, double max)
		{
			s_real_dist = std::uniform_real_distribution<>(min, max);
			return s_real_dist(s_random_engine);
		}

	private:
		std::mt19937 s_random_engine;
		std::uniform_int_distribution<int> s_int_dist;
		std::uniform_real_distribution<double> s_real_dist;
	};
}
