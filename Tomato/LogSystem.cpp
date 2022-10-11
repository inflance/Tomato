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
}

