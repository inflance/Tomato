#pragma once

#include <nlohmann/json.hpp>

#include "Scene.h"
#include "Tomato/ECS/EntityRegistry.h"

namespace Tomato
{
	using Json = nlohmann::json;

	class SceneSerializater
	{
	public:
		SceneSerializater(const std::shared_ptr<Scene>& scene);

		bool Serialization(const std::string& filepath);
		bool DeSerialization(const std::string& filepath);

		void SerializeEntity(Json& json, Entity entity);
	private:
		std::shared_ptr<Scene> m_scene;
		friend class EntityRegistry;
	};
}
