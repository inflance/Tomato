#include "FileSystem.h"

#include "Macro.h"

namespace Tomato
{
	std::filesystem::path FileSystem::m_current_path = {"./"};


	std::filesystem::path FileSystem::GetFullPath(const std::filesystem::path& relative_path)
	{
		return absolute(m_current_path / relative_path);
	}

	std::filesystem::path FileSystem::GetFullPath(const std::string& relative_path)
	{
		return GetFullPath(std::filesystem::path(relative_path));
	}

	std::string FileSystem::GetFileName(const std::filesystem::path& file_path)
	{
		std::string str = file_path.filename().generic_string();
		if (file_path.has_extension())
		{
			str.erase(str.find_first_of('.'), str.size());
		}

		return str;
	}

	bool FileSystem::CreateDirections(const std::filesystem::path& file_path)
	{
		if (file_path.empty() || is_directory(file_path)) return false;
		if (create_directories(file_path))
		{
			//LogSystem::Log(LogType::Info,"Create directory successfully");
			return true;
		}
		//LogSystem::Log(LogType::Error, "Create directory failed");
		return false;
	}
}
