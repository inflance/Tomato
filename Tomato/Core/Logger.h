#pragma once

#include <filesystem>
#include <format>
#include <queue>
#include <string>
#include <sstream>
#include <time.h>
#include <chrono>
#include <iostream>
#include <fstream>

namespace Tomato
{
	enum class LogLevel
	{
		Debug = 0,
		Warn,
		Error,
		Trace,
		Fatal,
		Info
	};

	enum class LogLevelColor
	{
		Default = 0,
		Black,
		Red,
		Green,
		Yellow,
		Blue,
		Purple,
		Cyan,
		White
	};

	enum class FileStats
	{
		False = 0,
		True
	};

	enum class FileMode
	{
		Override = 0,
		Append
	};

	enum class ConsoleStats
	{
		False = 0,
		True
	};

	struct LogStats
	{
		ConsoleStats _console = ConsoleStats::False;
		FileStats _file = FileStats::False;
		FileMode _mode = FileMode::Append;
	};

	constexpr const char* LogColorHeader[]
	{
		"",
		"\033[30;1m",
		"\033[31;1m",
		"\033[32;1m",
		"\033[33;1m",
		"\033[34;1m",
		"\033[35;1m",
		"\033[36;1m",
		"\033[37;1m",
	};

	constexpr const char* LogColorTail[]
	{
		"",
		"\033[0m",
		"\033[0m",
		"\033[0m",
		"\033[0m",
		"\033[0m",
		"\033[0m",
		"\033[0m",
		"\033[0m",
	};

	inline std::string LogString[]
	{
		" [Debug] ",
		" [Warn]  ",
		" [Error] ",
		" [Trace] ",
		" [Fatal] ",
		" [Info]  ",
	};


	class Logger
	{
	public:
		Logger()
			: Logger("")
		{
		}

		Logger(std::string name)
			: m_name(std::move(name))
		{
			m_thread = std::thread(&Logger::ProcessEntry, this);
		}

		virtual ~Logger()
		{
			{
				std::unique_lock lock(m_mutex);
				m_exit = true;
			}
			m_cond_var.notify_all();
			m_thread.join();
		}

		auto& SetPath(const std::filesystem::path& filepath)
		{
			m_filepath = filepath;
			return *this;
		}

		auto& SetTime(bool set_time)
		{
			m_set_time = set_time;
			return *this;
		}

		auto& SetStats(const LogStats& stats)
		{
			m_stats = stats;
			return *this;
		}

		void ClearPath()
		{
			m_filepath.clear();
		}

		template <LogLevel level>
		auto& SetColor(LogLevelColor color)
		{
			m_color[static_cast<int>(level)] = color;
			return *this;
		}

		template <LogLevel level>
		auto& SetLogStr(std::string str)
		{
			LogString[static_cast<int>(level)] = std::move(str);
			return *this;
		}

		template <typename... TArgs>
		void Log(LogLevel level, std::_Fmt_string<TArgs...> fmt, TArgs&&... args)
		{
			std::stringstream ss;
			ss << LogString[static_cast<int>(level)] << m_name << " ";

			//获得当前时间
			if (m_set_time)
			{
				using namespace std::chrono;

				const std::time_t tt = system_clock::to_time_t(system_clock::now());
				const std::tm tm = *std::gmtime(&tt);
				ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S ");
			}
			ss << std::format(fmt, std::forward<TArgs>(args)...);
			const std::string str = ss.str();
			const std::string console_str = LogColorHeader[static_cast<int>(m_color[static_cast<int>(level)])] + str +
				LogColorTail[static_cast<int>(m_color[static_cast<int>(level)])] + "\n";
			if (m_stats._console == ConsoleStats::True)
				std::cout << console_str;
			Log(str);
		}

		void Log(LogLevel level, std::string_view fmt)
		{
			std::stringstream ss;
			ss << LogString[static_cast<int>(level)] << m_name << " ";

			//获得当前时间
			if (m_set_time)
			{
				using namespace std::chrono;

				const std::time_t tt = system_clock::to_time_t(system_clock::now());
				const std::tm tm = *std::gmtime(&tt);
				ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S ");
			}
			ss << fmt;
			const std::string str = ss.str();
			const std::string console_str = LogColorHeader[static_cast<int>(m_color[static_cast<int>(level)])] + str +
				LogColorTail[static_cast<int>(m_color[static_cast<int>(level)])] + "\n";
			if (m_stats._console == ConsoleStats::True)
				std::cout << console_str;
			Log(str);
		}

		void Error(std::string_view str)
		{
			Log(LogLevel::Error, str);
		}

		void Debug(std::string_view str)
		{
			Log(LogLevel::Error, str);
		}

		void Fatal(std::string_view str)
		{
			Log(LogLevel::Error, str);
		}

		void Info(std::string_view str)
		{
			Log(LogLevel::Error, str);
		}

		void Warn(std::string_view str)
		{
			Log(LogLevel::Error, str);
		}

		template <typename... TArgs>
		void Error(std::_Fmt_string<TArgs...> fmt, TArgs&&... args)
		{
			Log(LogLevel::Error, fmt, std::forward<TArgs>(args)...);
		}

		template <typename... TArgs>
		void Debug(std::_Fmt_string<TArgs...> fmt, TArgs&&... args)
		{
			Log(LogLevel::Error, fmt, std::forward<TArgs>(args)...);
		}

		template <typename... TArgs>
		void Fatal(std::_Fmt_string<TArgs...> fmt, TArgs&&... args)
		{
			Log(LogLevel::Error, fmt, std::forward<TArgs>(args)...);
		}

		template <typename... TArgs>
		void Trace(std::_Fmt_string<TArgs...> fmt, TArgs&&... args)
		{
			Log(LogLevel::Error, fmt, std::forward<TArgs>(args)...);
		}

		template <typename... TArgs>
		void Warn(std::_Fmt_string<TArgs...> fmt, TArgs&&... args)
		{
			Log(LogLevel::Error, fmt, std::forward<TArgs>(args)...);
		}

		template <typename... TArgs>
		void Info(std::_Fmt_string<TArgs...> fmt, TArgs&&... args)
		{
			Log(LogLevel::Error, fmt, std::forward<TArgs>(args)...);
		}

		void Log(std::string str)
		{
			std::unique_lock lock{m_mutex};
			m_log_queue.push(std::move(str));
			m_cond_var.notify_all();
		}

		static std::shared_ptr<Logger> Create()
		{
			return std::make_shared<Logger>();
		}

		static std::shared_ptr<Logger> Create(std::string name)
		{
			return std::make_shared<Logger>(std::move(name));
		}

		Logger(const Logger& other) = delete;
		Logger& operator=(const Logger& other) = delete;

	private:
		void ProcessEntry()
		{
			if ((m_stats._file == FileStats::True && !m_filepath.empty()))
			{
				std::ofstream ofs(
					m_filepath, std::ios::out | (m_stats._mode == FileMode::Append ? std::ios::app : std::ios::ate));
				if (ofs.fail())
				{
					const std::string str = "Failed to open file " + m_filepath.string() + "\n";
					std::cerr << str;
				}

				std::unique_lock lock(m_mutex, std::defer_lock);
				while (true)
				{
					lock.lock();
					if (!m_exit)
					{
						m_cond_var.wait(lock);
					}
					else
					{
						ProcessEntryToFile(m_log_queue, ofs);
						break;
					}

					std::queue<std::string> log_queue;
					std::swap(m_log_queue, log_queue);
					lock.unlock();
					ProcessEntryToFile(log_queue, ofs);
				}
			}
		}

		void ProcessEntryToFile(std::queue<std::string>& queue, std::ofstream& ofs) const
		{
			while (!queue.empty())
			{
				//if (m_stats._file == FileStats::True && !m_filepath.empty())
				ofs << queue.front() << "\n";
				queue.pop();
			}
			ofs.flush();
			std::cout.flush();
		}

	private:
		std::queue<std::string> m_log_queue;

		LogStats m_stats{};
		std::filesystem::path m_filepath{};
		std::string m_name{};

		std::mutex m_mutex;
		std::condition_variable m_cond_var;
		std::thread m_thread;

		LogLevelColor m_color[6]{LogLevelColor::Default};

		bool m_exit{false};
		bool m_set_time{false};
	};
}
