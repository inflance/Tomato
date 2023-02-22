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
	public:
		static void Init();

		template<class... TArgs>
		static void ConsoleLog(LogType type, TArgs... args)
		{
			switch (type)
			{
			case Tomato::LogType::Trace:
				s_loger->trace(args...);
				break;
			case Tomato::LogType::Info:
				s_loger->info(args...);
				break;
			case Tomato::LogType::Warn:
				s_loger->warn(args...);
				break;
			case Tomato::LogType::Error:
				s_loger->error(args...);
				break;
			default:
				break;
			}
		}

		static std::shared_ptr<spdlog::logger>& GetLoger() { 
			return s_loger; 
		}
	private:
		static std::shared_ptr<spdlog::logger> s_loger;
	};
}

