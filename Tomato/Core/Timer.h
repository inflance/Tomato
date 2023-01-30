#pragma once

#include <chrono>
#include <iostream>

namespace Tomato
{
	class Timer
	{
	public:
		Timer(std::string&& name)
			: m_name(std::forward<std::string>(name))
		{
			Reset();
		}

		void Reset()
		{
			m_start = std::chrono::high_resolution_clock::now();
		}

		void CountTime()
		{
			m_end = std::chrono::high_resolution_clock::now();
			m_time = static_cast<float>(m_end.time_since_epoch().count() - m_start.time_since_epoch().count()) *
				std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
			std::cout << m_name << " " << m_time << "ms" << std::endl;
		}

		~Timer()
		{
			CountTime();
		}

	private:
		std::string m_name;
		float m_time = 0;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_end;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
	};
}
