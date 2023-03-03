#pragma once

#include "nlohmann/json.hpp"

namespace Tomato
{
	class Entity;
	class Level;

	using Json = nlohmann::json;

	class LevelSerialization
	{
		LevelSerialization(const std::shared_ptr<Level>& scene);
		~LevelSerialization();

		bool Serialization(const std::string& filepath);

		bool DeSerialization(const std::string& filepath) const;

		static void SerializeEntity(Json& main_json, Entity entity);

	private:
		std::shared_ptr<Level> m_level;
		friend class EntityRegistry;
	};
	
}