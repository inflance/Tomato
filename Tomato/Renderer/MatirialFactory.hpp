#pragma once

#include <unordered_map>
#include <string>

#include "Tomato/Core/Core.hpp"
#include "Material.hpp"

namespace Tomato
{
	class MatirialFactory
	{
	private:
		MatirialFactory() = default;
		~MatirialFactory() = default;

	public:
		static MatirialFactory& Get()
		{
			static MatirialFactory instance;
			return instance;
		}

		void Add(const std::string& path, const MaterialTextureData& data);

		MaterialTextureData& Load(const std::string& path, MaterialTextureData& data);

		MaterialTextureData& GetMatirial(const std::string& path);

		bool Exist(const std::string& path);

	private:
		std::unordered_map<std::string, MaterialTextureData> m_matirial_factory;
	};
}
