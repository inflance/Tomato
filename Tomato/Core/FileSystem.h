#pragma once

#include <filesystem>

namespace Tomato
{
	class FileSystem
	{
	public:
		static std::filesystem::path GetFullPath(const std::filesystem::path& file_path);
		static std::filesystem::path GetFullPath(const std::string& file_path);
		static std::string GetFileName(const std::filesystem::path& file_path);
		static bool CreateDirections(const std::filesystem::path& file_path);

	private:
		static std::filesystem::path m_current_path;
	};
}
