#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

#include "Tomato/Core/Macro.h"
#include "Tomato/Core/Timer.h"

namespace Tomato
{
	namespace Loader
	{
		template<typename T>
		T Loader(const std::filesystem::path& file_path, size_t size = 1) noexcept
		{
			T buffer;
			std::ifstream ifs(file_path, std::ios::in | std::ios::binary);
			LOG_ASSERT(ifs.is_open(), "Can't open file {0}", file_path.generic_string());
			ifs.seekg(0, std::ios::end);
			buffer.resize(ifs.tellg());
			ifs.seekg(0, std::ios::beg);
			ifs.read((char*)&buffer[0], buffer.size() * size);
			ifs.close();

			return buffer;
		}

		template<typename T>
		void UnLoader(const std::filesystem::path& file_path, const T& data, size_t size = 1) noexcept
		{
			std::ofstream ofs(file_path, std::ios::out | std::ios::binary);
			LOG_ASSERT(ofs.is_open(), "Can't open file {0}", file_path.generic_string());
			ofs.write((char*)data.data(), data.size() * size);
			ofs.close();
		}
	}
}
