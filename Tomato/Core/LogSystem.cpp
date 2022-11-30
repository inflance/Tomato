#include "logSystem.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Tomato {

	std::shared_ptr<spdlog::logger> LogSystem::s_loger;

	void LogSystem::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_loger = spdlog::stdout_color_mt("Tomato");
		s_loger->set_level(spdlog::level::trace);
	}

	void LogSystem::ConsoleLog(const std::string& str, LogType type)
	{
		switch (type)
		{
		case Tomato::LogType::Trace:
			s_loger->trace(str);
			break;
		case Tomato::LogType::Info:
			s_loger->info(str);
			break;
		case Tomato::LogType::Warn:
			s_loger->warn(str);
			break;
		case Tomato::LogType::Error:
			s_loger->error(str);
			break;
		default:
			break;
		}
	}

}

