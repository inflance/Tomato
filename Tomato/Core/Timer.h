#pragma once

#include <chrono>

namespace Tomato
{
	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		void Reset()
		{
			m_start = std::chrono::high_resolution_clock::now();
		}

		float CountTime()
		{
			m_end = std::chrono::high_resolution_clock::now();
			return static_cast<float>(m_end.time_since_epoch().count() - m_start.time_since_epoch().count()) *
				std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_end;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
	};
}
