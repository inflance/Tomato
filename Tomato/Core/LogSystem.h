#pragma once

#include <spdlog/spdlog.h>

namespace Tomato
{
	enum class LogType
	{
		Trace = 0,
		Info,
		Warn,
		Error
	};

	class LogSystem
	{
	public:
		static void Init();
		static void Destroy();

		template <typename... TArgs>
		static void Log(LogType type, TArgs&&... args)
		{
			switch (type)
			{
			case LogType::Trace:
				s_logger->trace(std::forward<TArgs>(args)...);
				break;
			case LogType::Info:
				s_logger->info(std::forward<TArgs>(args)...);
				break;
			case LogType::Warn:
				s_logger->warn(std::forward<TArgs>(args)...);
				break;
			case LogType::Error:
				s_logger->error(std::forward<TArgs>(args)...);
				break;
			default:
				break;
			}
		}

		template <typename... TArgs>
		static void LogInfo(TArgs&&... args)
		{
			GetLoger()->info(std::forward<TArgs>(args)...);
		}

		template <typename... TArgs>
		static void LogError(TArgs&&... args)
		{
			GetLoger()->error(std::forward<TArgs>(args)...);
		}

		template <typename... TArgs>
		static void LogTrace(TArgs&&... args)
		{
			GetLoger()->trace(std::forward<TArgs>(args)...);
		}

		template <typename... TArgs>
		static void LogWarn(TArgs&&... args)
		{
			GetLoger()->warn(std::forward<TArgs>(args)...);
		}

		static auto GetLoger() { return s_logger; }

	private:
		inline static std::shared_ptr<spdlog::logger> s_logger;
	};
}
