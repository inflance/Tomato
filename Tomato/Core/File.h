#pragma once

namespace Tomato::File
{
	class Diolog
	{
	public:
		static std::string SaveFile(const char* filter);
		static std::string OpenFile(const char* filter);
	};

	class Directory
	{
	public:
		static void GetFileNames(std::string path, std::vector<std::string>& files,
		                         std::vector<std::string>& directories);
	};
}
