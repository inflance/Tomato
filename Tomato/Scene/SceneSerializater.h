#pragma once

#include <nlohmann/json.hpp>
#include "Scene.h"

namespace Tomato {

	using Json = nlohmann::json;

	class SceneSerializater
	{
	
	public:
		SceneSerializater(const std::shared_ptr<Scene>& scene);

		bool Serialization(const std::string& filepath);
		bool DeSerialization(const std::string& filepath);

		void SerializeGO(Json& json,GameObject GO);
	private:
		std::shared_ptr<Scene> m_Scene;
	};
}
