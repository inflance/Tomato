#pragma once

#include <spdlog/spdlog.h>


namespace Tomato {

	class LogSystem
	{
	private:
		static std::shared_ptr<spdlog::logger> s_loger;

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetLoger() { 
			return s_loger; 
		}
	};
}

