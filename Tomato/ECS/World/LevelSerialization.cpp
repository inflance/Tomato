#include "LevelSerialization.hpp"

#include <fstream>

#include "Tomato/ECS/Entity.h"
#include "Tomato/ECS/World/Level.hpp"
#include "Tomato/ECS/Components.h"
#include "Tomato/ECS/EntityRegistry.h"

namespace nlohmann
{
	template <>
	struct adl_serializer<glm::vec2>
	{
		// note: the return type is no longer 'void', and the method only takes
		// one argument
		static glm::vec2 from_json(const json& j)
		{
			auto f = j.get<std::vector<float>>();
			return {f[0], f[1]};
		}

		// Here's the catch! You must provide a to_json method! Otherwise, you
		// will not be able to convert move_only_type to json, since you fully
		// specialized adl_serializer on that type
		static void to_json(json& j, glm::vec2 t)
		{
			j = {t.x, t.y};
		}
	};

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

	bool LevelSerialization::Serialization(const std::string& filepath) const
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
				if (!camera_component.empty())
				{
					auto& cc = deserialized_entity.AddComponent<CameraComponent>();

					auto& camera_props = camera_component["Camera"];
					cc.camera_.SetCameraType(
						str_to_camera_type[camera_props["ProjectionType"].get<std::string>()]);
					cc.camera_.SetFov(camera_props["FOV"].get<float>());
					cc.camera_.SetNear(camera_props["Near"].get<float>());
					cc.camera_.SetFar(camera_props["Far"].get<float>());
					cc.camera_.SetZoomLevel(camera_props["ZoomLevel"].get<float>());
					cc.is_main_camera_ = camera_component["IsMain"].get<bool>();
					cc.is_resize_ = camera_component["IsResize"].get<bool>();
				}

				auto light_component = entity["LightComponent"];
				if (!light_component.empty())
				{
					auto& lc = deserialized_entity.AddComponent<LightComponent>();

					auto& light = light_component["Light"];
					lc.light_.light_type_ = str_to_light_type[light["LightType"].get<std::string>()];
					lc.light_.color_ = light["Color"].get<glm::vec3>();
					lc.light_.position = light["Position"].get<glm::vec3>();
					lc.light_.direction_ = light["Direction"].get<glm::vec3>();
					lc.light_.intensity_ = light["Intensity"].get<float>();
					lc.light_.constant_ = light["Constant"].get<float>();
					lc.light_.linear_ = light["Linear"].get<float>();
					lc.light_.quadratic_ = light["Quadratic"].get<float>();
					lc.light_.cut_off = light["CutOff"].get<float>();
					lc.light_.outer_cut_off = light["OuterCutOff"].get<float>();
				}

				auto sprite_component_json = entity["SpriteComponent"];
				if (!sprite_component_json.empty())
				{
					auto& sc = deserialized_entity.AddComponent<SpriteComponent>();
					sc.geometry_type_ = str_to_geometry_type[sprite_component_json["GeometryType"].get<std::string>()];
					sc.color_ = sprite_component_json["Color"].get<glm::vec4>();
					sc.tiling_factor_ = sprite_component_json["TilingFactor"].get<float>();
				}

				auto rigid_body2d_component_json = entity["RigidBody2DComponent"];
				if (!rigid_body2d_component_json.empty())
				{
					auto& rb2dc = deserialized_entity.AddComponent<RigidBody2DComponent>();
					rb2dc.type_ = str_to_body_type[rigid_body2d_component_json["BodyType"].get<std::string>()];
					rb2dc.fixed_rotation_ = rigid_body2d_component_json["FixedRotation"].get<bool>();
				}

				auto box_collider2d_component_json = entity["BoxCollider2DComponent"];
				if (!box_collider2d_component_json.empty())
				{
					auto& bc2dc = deserialized_entity.AddComponent<BoxCollider2DComponent>();
					bc2dc.offset_ = box_collider2d_component_json["Offset"].get<glm::vec2>();
					bc2dc.size_ = box_collider2d_component_json["Size"].get<glm::vec2>();
					bc2dc.density_ = box_collider2d_component_json["Density"].get<float>();
					bc2dc.friction_ = box_collider2d_component_json["Friction"].get<float>();
					bc2dc.restitution_ = box_collider2d_component_json["Restitution"].get<float>();
					bc2dc.restitution_threshold_ = box_collider2d_component_json["RestitutionThreshold"].get<float>();
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
			json["Entity"]["CameraComponent"]["Camera"]["ProjectionType"] = camera_type_to_str[camera.GetCameraType()];
			json["Entity"]["CameraComponent"]["Camera"]["FOV"] = camera.GetFov();
			json["Entity"]["CameraComponent"]["Camera"]["Far"] = camera.GetFar();
			json["Entity"]["CameraComponent"]["Camera"]["Near"] = camera.GetNear();
			json["Entity"]["CameraComponent"]["Camera"]["ZoomLevel"] = camera.GetZoomLevel();

			json["Entity"]["CameraComponent"]["IsMain"] = cc.is_main_camera_;
			json["Entity"]["CameraComponent"]["IsResize"] = cc.is_resize_;
		}
		if (entity.HasComponent<LightComponent>())
		{
			const auto& lc = entity.GetComponent<LightComponent>();
			const auto& light = lc.light_;
			//light
			json["Entity"]["LightComponent"]["Light"]["LightType"] = light_type_to_str[light.light_type_];
			json["Entity"]["LightComponent"]["Light"]["Color"] = light.color_;
			json["Entity"]["LightComponent"]["Light"]["Position"] = light.position;
			json["Entity"]["LightComponent"]["Light"]["Direction"] = light.direction_;
			json["Entity"]["LightComponent"]["Light"]["Intensity"] = light.intensity_;
			json["Entity"]["LightComponent"]["Light"]["Constant"] = light.constant_;
			json["Entity"]["LightComponent"]["Light"]["Linear"] = light.linear_;
			json["Entity"]["LightComponent"]["Light"]["Quadratic"] = light.quadratic_;
			json["Entity"]["LightComponent"]["Light"]["CutOff"] = light.cut_off;
			json["Entity"]["LightComponent"]["Light"]["OuterCutOff"] = light.outer_cut_off;
		}

		if (entity.HasComponent<SpriteComponent>())
		{
			const auto& sc = entity.GetComponent<SpriteComponent>();
			auto& color = sc.color_;
			auto& tiling_factor = sc.tiling_factor_;
			json["Entity"]["SpriteComponent"]["GeometryType"] = geometry_type_to_str[sc.geometry_type_];
			json["Entity"]["SpriteComponent"]["Color"] = color;
			json["Entity"]["SpriteComponent"]["TilingFactor"] = tiling_factor;
		}

		if (entity.HasComponent<RigidBody2DComponent>())
		{
			const auto& rb2dc = entity.GetComponent<RigidBody2DComponent>();
			auto& type = body_type_to_str[rb2dc.type_];
			auto& fixed_rotation = rb2dc.fixed_rotation_;
			json["Entity"]["RigidBody2DComponent"]["BodyType"] = type;
			json["Entity"]["RigidBody2DComponent"]["FixedRotation"] = fixed_rotation;
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			const auto& bc2dc = entity.GetComponent<BoxCollider2DComponent>();
			auto& offset = bc2dc.offset_;
			auto& size = bc2dc.size_;
			auto& density = bc2dc.density_;
			auto& friction = bc2dc.friction_;
			auto& restitution = bc2dc.restitution_;
			auto& restitution_threshold = bc2dc.restitution_threshold_;

			json["Entity"]["BoxCollider2DComponent"]["Offset"] = offset;
			json["Entity"]["BoxCollider2DComponent"]["Size"] = size;
			json["Entity"]["BoxCollider2DComponent"]["Density"] = density;
			json["Entity"]["BoxCollider2DComponent"]["Friction"] = friction;
			json["Entity"]["BoxCollider2DComponent"]["Restitution"] = restitution;
			json["Entity"]["BoxCollider2DComponent"]["RestitutionThreshold"] = restitution_threshold;
		}

		main_json = json;
	}
}
