#pragma once

#include <spdlog/spdlog.h>

namespace Tomato {

	enum class LogType {
		Trace = 0,
		Info,
		Warn,
		Error
	};

	class LogSystem
	{
	private:
		static std::shared_ptr<spdlog::logger> s_loger;

	public:
		static void Init();

		static void ConsoleLog(const std::string& str, LogType type);

		inline static std::shared_ptr<spdlog::logger>& GetLoger() { 
			return s_loger; 
		}
	};
}

