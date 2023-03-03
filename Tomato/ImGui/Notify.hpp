#pragma once

namespace Tomato
{
	
	class Notify
	{
	public:
		enum class NotifyLevel
		{
			Success,
			Warning,
			Error,
			Info
		};
		static void Init();
		static void Insert(NotifyLevel level, int time, std::string_view format, std::string_view str = "");
		static void Render();
	private:
	};
}