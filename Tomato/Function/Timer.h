#pragma once

#include <chrono>
#include <unordered_map>

namespace Tomato {

	class Timer
	{
	public:
		Timer(const char* name)
			:m_name(name)
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
			m_time = static_cast<float>(m_end.time_since_epoch().count() - m_start.time_since_epoch().count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
			LOG_INFO("name:{0} {1}ms", m_name, m_time);
		}

		~Timer() {
			CountTime();
		}
		
	private:
		const char* m_name;
		float m_time = 0;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_end;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
	};

}
