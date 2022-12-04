#pragma once

#include <unordered_map>
#include <string>

#include "Tomato/Core/Core.h"
#include "Matirial.h"

namespace Tomato {
	
	class MatirialFactory
	{
	private:
		MatirialFactory() = default;
		~MatirialFactory() = default;

	public:
		static MatirialFactory& GetInstance() { static MatirialFactory instance; return instance; }

		void Add(const std::string& path, const MatirialTextureData& data);

		MatirialTextureData& Load(const std::string& path, MatirialTextureData& data);

		MatirialTextureData& GetMatirial(const std::string& path);

		bool Exist(const std::string& path);

	private:
		std::unordered_map<std::string, MatirialTextureData> m_matirial_factory;
	};

}