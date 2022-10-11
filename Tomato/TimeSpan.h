#pragma once

#include <chrono>

namespace Tomato {

	class TimeSpan {

	public:
		TimeSpan(float time = 0.0f)
			:m_time(time)
		{}

		float GetTime() const { return m_time; }

		float GetMinusTime() const { return m_time * 1000; }

		operator float() const { return m_time; }
	private:
		float m_time;
		
	};
}