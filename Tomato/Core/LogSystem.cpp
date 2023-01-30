#include "logSystem.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Tomato
{
	void LogSystem::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_logger = spdlog::stdout_color_mt("Tomato");
		s_logger->set_level(spdlog::level::trace);
		Log(LogType::Info, "Log init success");
	}

	void LogSystem::Destroy()
	{

	}

}
