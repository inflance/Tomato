#include "LevelSerialization.hpp"

#include <fstream>

#include "Tomato/ECS/Entity.h"
#include "Tomato/ECS/World/Level.hpp"
#include "Tomato/ECS/Components.h"
#include "Tomato/ECS/EntityRegistry.h"

namespace nlohmann
{
	template <>
	struct adl_serializer<glm::vec3>
	{
		// note: the return type is no longer 'void', and the method only takes
		// one argument
		static glm::vec3 from_json(const json& j)
		{
			auto f = j.get<std::vector<float>>();
			return {f[0], f[1], f[2]};
		}

		// Here's the catch! You must provide a to_json method! Otherwise, you
		// will not be able to convert move_only_type to json, since you fully
		// specialized adl_serializer on that type
		static void to_json(json& j, glm::vec3 t)
		{
			j = {t.x, t.y, t.z};
		}
	};

	template <>
	struct adl_serializer<glm::vec4>
	{
		// note: the return type is no longer 'void', and the method only takes
		// one argument
		static glm::vec4 from_json(const json& j)
		{
			auto f = j.get<std::vector<float>>();
			return {f[0], f[1], f[2], f[3]};
		}

		// Here's the catch! You must provide a to_json method! Otherwise, you
		// will not be able to convert move_only_type to json, since you fully
		// specialized adl_serializer on that type
		static void to_json(json& j, glm::vec4 t)
		{
			j = {t.x, t.y, t.z, t.a};
		}
	};
}

namespace Tomato
{
	LevelSerialization::LevelSerialization(const std::shared_ptr<Level>& scene)
		: m_level(scene)
	{
	}

	LevelSerialization::~LevelSerialization()
	{
	}

	bool LevelSerialization::Serialization(const std::string& filepath)
	{
		Json main_json;
		main_json["Level"]["LevelName"] = m_level->GetLevelName().empty() ? "UnTitled" : m_level->GetLevelName();

		m_level->GetLevelRegistry()->Get().each(
			[&](auto entity)
			{
				Json temp;
				const Entity serialization_entity = {entity, m_level->GetLevelRegistry().get()};

				if (!serialization_entity)
					return false;

				SerializeEntity(temp, serialization_entity);
				main_json["Entities"] += temp;
			});

		std::ofstream ofs(filepath);
		ofs << main_json.dump(2);
		ofs.close();
		return true;
	}

	bool LevelSerialization::DeSerialization(const std::string& filepath) const
	{
		Json json;

		try
		{
			std::ifstream ifs(filepath);
			json << ifs;
			ifs.close();
		}
		catch (Json::exception& e)
		{
			LOG_ERROR("{0}:{1}", e.id, e.what());
			return false;
		}
		auto scene_name = json["Level"]["LevelName"];
		auto& entities = json["Entities"];
		if (entities.empty() == false)
		{
			for (auto entity_json : entities)
			{
				auto entity = entity_json["Entity"];
				std::string name;
				auto tag_json = entity["TagComponent"]["Tag"];
				if (tag_json.empty() == false)
					name = tag_json.get<std::string>();

				Entity deserialized_entity = m_level->CreateDefaultEntity(name);

				auto transform_component_json = entity["TransformComponent"];
				if (!transform_component_json.empty())
				{
					auto& tc = deserialized_entity.GetComponent<TransformComponent>();
					tc.position_ = transform_component_json["Position"].get<glm::vec3>();
					tc.rotation_ = transform_component_json["Rotation"].get<glm::vec3>();
					tc.scale_ = transform_component_json["Scale"].get<glm::vec3>();
				}

				auto camera_component = entity["CameraComponent"];
				if (camera_component.empty() == false)
				{
					auto& cc = deserialized_entity.AddComponent<CameraComponent>();

					auto& camera_props = camera_component["Camera"];
					cc.camera_.SetCameraType(
						static_cast<CameraType>(camera_props["ProjectionType"].get<int>()));

					cc.camera_.SetFov(camera_props["FOV"].get<float>());
					cc.camera_.SetNear(camera_props["Near"].get<float>());
					cc.camera_.SetFar(camera_props["Far"].get<float>());

					cc.camera_.SetZoomLevel(camera_props["OrthographicZoomLevel"].get<float>());

					cc.is_main_camera_ = camera_component["IsMain"].get<bool>();
					cc.is_resize_ = camera_component["IsResize"].get<bool>();
				}

				auto sprite_component_json = entity["SpriteComponent"];
				if (sprite_component_json.empty() == false)
				{
					auto& sc = deserialized_entity.AddComponent<SpriteComponent>();
					sc.color_ = sprite_component_json["Color"].get<glm::vec4>();
					sc.tiling_factor_ = sprite_component_json["TilingFactor"].get<float>();
				}
			}
		}

		return true;
	}

	void LevelSerialization::SerializeEntity(Json& main_json, Entity entity)
	{
		Json json;
		{
			json["Entity"]["EntityID"] = static_cast<uint32_t>(entity);
		}
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().tag_;
			json["Entity"]["TagComponent"]["Tag"] = tag;
		}
		if (entity.HasComponent<TransformComponent>())
		{
			const auto& tc = entity.GetComponent<TransformComponent>();
			auto& position = tc.position_;
			auto& rotation = tc.rotation_;
			auto& scale = tc.scale_;
			json["Entity"]["TransformComponent"]["Position"] = {position.x, position.y, position.z};
			json["Entity"]["TransformComponent"]["Rotation"] = {rotation.x, rotation.y, rotation.z};
			json["Entity"]["TransformComponent"]["Scale"] = {scale.x, scale.y, scale.z};
		}
		if (entity.HasComponent<CameraComponent>())
		{
			const auto& cc = entity.GetComponent<CameraComponent>();
			const auto& camera = cc.camera_;
			//camera
			json["Entity"]["CameraComponent"]["Camera"]["ProjectionType"] = camera.GetCameraType();
			json["Entity"]["CameraComponent"]["Camera"]["FOV"] = camera.GetFov();
			json["Entity"]["CameraComponent"]["Camera"]["Far"] = camera.GetFar();
			json["Entity"]["CameraComponent"]["Camera"]["Near"] = camera.GetNear();
			json["Entity"]["CameraComponent"]["Camera"]["ZoomLevel"] = camera.GetZoomLevel();

			json["Entity"]["CameraComponent"]["IsMain"] = cc.is_main_camera_;
			json["Entity"]["CameraComponent"]["IsResize"] = cc.is_resize_;
		}
		if (entity.HasComponent<SpriteComponent>())
		{
			const auto& sc = entity.GetComponent<SpriteComponent>();
			auto& color = sc.color_;
			auto& tilingFactor = sc.tiling_factor_;
			json["Entity"]["SpriteComponent"]["Color"] = {color.r, color.g, color.b, color.a};

			json["Entity"]["SpriteComponent"]["TilingFactor"] = tilingFactor;
		}
		main_json = json;
	}
}
