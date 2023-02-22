#include "log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

#include "Macro.h"

namespace Tomato
{
	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_logger = spdlog::stdout_color_mt("Tomato");
		s_logger->set_level(spdlog::level::trace);
		LOG_TRACE("Log Init");
	}

	void Log::Destroy()
	{
		s_logger.reset();
		LOG_TRACE("Log Destroy");
	}
}
