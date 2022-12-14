#pragma once

#include <random>

namespace Tomato{

	class Random
	{
	public:
		static Random& Get() { static Random instance; return instance;}

		void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		double GetDouble(double min, double max)
		{
			s_real_dist = std::uniform_real_distribution<>(min, max);
			return s_real_dist(s_RandomEngine);
		}

	private:
		Random() = default;
		~Random() = default;
	private:
		std::mt19937 s_RandomEngine;
		std::uniform_int_distribution<int> s_int_dist;
		std::uniform_real_distribution<double> s_real_dist;
	};
}