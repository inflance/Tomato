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

	class Log
	{
	public:
		static void Init();
		static void Destroy();

		template <typename... TArgs>
		static void Info(TArgs&&... args)
		{
			Get()->info(std::forward<TArgs>(args)...);
		}

		template <typename... TArgs>
		static void Error(TArgs&&... args)
		{
			Get()->error(std::forward<TArgs>(args)...);
		}

		template <typename... TArgs>
		static void Trace(TArgs&&... args)
		{
			Get()->trace(std::forward<TArgs>(args)...);
		}

		template <typename... TArgs>
		static void Warn(TArgs&&... args)
		{
			Get()->warn(std::forward<TArgs>(args)...);
		}

		static auto Get() { return s_logger; }

	private:
		inline static std::shared_ptr<spdlog::logger> s_logger;
	};
}
