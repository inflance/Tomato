#include "SceneSerializater.h"

#include <fstream>

#include "Entity.h"
#include "Components.h"

namespace nlohmann {

	template <>
	struct adl_serializer<glm::vec3> {
		// note: the return type is no longer 'void', and the method only takes
		// one argument
		static glm::vec3 from_json(const json& j) {
			std::vector<float> f = j.get<std::vector<float>>();
			return { f[0], f[1], f[2] };
		}

		// Here's the catch! You must provide a to_json method! Otherwise, you
		// will not be able to convert move_only_type to json, since you fully
		// specialized adl_serializer on that type
		static void to_json(json& j, glm::vec3 t) {
			j = { t.x , t.y, t.z };
		}
	};

	template <>
	struct adl_serializer<glm::vec4> {
		// note: the return type is no longer 'void', and the method only takes
		// one argument
		static glm::vec4 from_json(const json& j){
			std::vector<float> f = j.get<std::vector<float>>();
			return { f[0], f[1], f[2], f[3]};
		}

		// Here's the catch! You must provide a to_json method! Otherwise, you
		// will not be able to convert move_only_type to json, since you fully
		// specialized adl_serializer on that type
		static void to_json(json& j, glm::vec4 t) {
			j = { t.x , t.y, t.z, t.a};
		}
	};
}

namespace Tomato {

	SceneSerializater::SceneSerializater(const std::shared_ptr<Scene>& scene)
		:m_Scene(scene)
	{

	}

	bool SceneSerializater::Serialization(const std::string& filepath)
	{
		Json json;
		json["Scene"]["SceneName"] = "Untitled";

		Json j;
		m_Scene->m_Registry.each(
			[&](auto entt)
			{
				Json temp;
				Entity entity = { entt, m_Scene.get() };
	
				if (!entity)
					return false;
	
				SerializeEntity(temp, entity);
				json["Entities"] += temp;
				
			});

		std::ofstream ofs(filepath);
		ofs << json.dump(2);
		ofs.close();
		return true;
	}

	bool SceneSerializater::DeSerialization(const std::string& filepath)
	{
		Json json;

		try
		{
			std::ifstream ifs(filepath);
			json << ifs;
			ifs.close();
		}
		catch (Json::exception e) {

			LOG_ERROR("{0}:{1}",e.id, e.what());
			return false;
		}
		auto sceneName = json["Scene"]["SceneName"];
		LOG_TRACE("Scene {0}", sceneName);

		auto Entitys = json["Entities"];
		if (Entitys.empty() == false)
		{
			for(auto go : Entitys)
			{
				auto entityid = go["Entity"]["EntityID"].get<uint64_t>();
				auto g = go["Entity"];
				std::string name;
				auto nameComponent = g["NameComponent"]["Name"];
				if (nameComponent.empty() == false)
					name = nameComponent.get<std::string>();

				Entity deserializedEntity = m_Scene->CreateEntity(name);

				deserializedEntity.SetEntityID(entityid);

				auto tranformComponent = g["TransformComponent"];
				if (tranformComponent.empty() == false)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Position= tranformComponent["Position"].get<glm::vec3>();
					tc.Rotation = tranformComponent["Rotation"].get<glm::vec3>();
					tc.Scale = tranformComponent["Scale"].get<glm::vec3>();
				}

				auto cameraComponent = g["CameraComponent"];
				if (cameraComponent.empty() == false)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					cc.Camera.SetSceneCameraType((SceneCameraType)cameraProps["ProjectionType"].get<int>());

					cc.Camera.SetPerspFOV(cameraProps["PerspectiveFOV"].get<float>());
					cc.Camera.SetPerspNear(cameraProps["PerspectiveNear"].get<float>());
					cc.Camera.SetPerspFar(cameraProps["PerspectiveFar"].get<float>());

					cc.Camera.SetOrthoZoomLevel(cameraProps["OrthographicZoomLevel"].get<float>());
					cc.Camera.SetOrthoNear(cameraProps["OrthographicNear"].get<float>());
					cc.Camera.SetOrthoFar(cameraProps["OrthographicFar"].get<float>());

					cc.IsMain = cameraComponent["IsMain"].get<bool>();
					cc.IsResize = cameraComponent["IsResize"].get<bool>();
				}

				auto spriteComponent = g["SpriteComponent"];
				if (spriteComponent.empty() == false)
				{
					auto& src = deserializedEntity.AddComponent<SpriteComponent>();
					src.Color = spriteComponent["Color"].get<glm::vec4>();
					src.Texture = Texture2D::Create(spriteComponent["TexturePath"].get<std::string>());
					src.TilingFactor = spriteComponent["TilingFactor"].get<int>();
				}

			}
		}

		return true;
	}

	void SceneSerializater::SerializeEntity(Json& json, Entity entity)
	{
		Json js;
		{
			js["Entity"]["EntityID"] = (uint64_t)entity.GetEntityID();
		}
		if (entity.HasComponent<NameComponent>())
		{
			auto& name = entity.GetComponent<NameComponent>().Name;
			js["Entity"]["NameComponent"]["Name"] = name;
		}
		if (entity.HasComponent<TransformComponent>())
		{
			auto& position	= entity.GetComponent<TransformComponent>().Position;
			auto& rotation	= entity.GetComponent<TransformComponent>().Rotation;
			auto& scale		= entity.GetComponent<TransformComponent>().Scale;
			js["Entity"]["TransformComponent"]["Position"] = { position.x, position.y, position.z };
			js["Entity"]["TransformComponent"]["Rotation"] = { rotation.x, rotation.y, rotation.z };
			js["Entity"]["TransformComponent"]["Scale"] = { scale.x, scale.y, scale.z };
		}
		if (entity.HasComponent<CameraComponent>())
		{
			auto& cc = entity.GetComponent<CameraComponent>();
			auto camera = cc.Camera;
			//camera
			js["Entity"]["CameraComponent"]["Camera"]["ProjectionType"]			= camera.GetSceneCameraType();
			js["Entity"]["CameraComponent"]["Camera"]["PerspectiveFOV"]			= camera.GetPerspFOV();
			js["Entity"]["CameraComponent"]["Camera"]["PerspectiveFar"]			= camera.GetPerspFar();
			js["Entity"]["CameraComponent"]["Camera"]["PerspectiveNear"]		= camera.GetPerspNear();
			js["Entity"]["CameraComponent"]["Camera"]["OrthographicZoomLevel"]	= camera.GetOrthoZoomLevel();
			js["Entity"]["CameraComponent"]["Camera"]["OrthographicFar"]		= camera.GetOrthoFar();
			js["Entity"]["CameraComponent"]["Camera"]["OrthographicNear"]		= camera.GetOrthoNear();

			js["Entity"]["CameraComponent"]["IsMain"]							= cc.IsMain;
			js["Entity"]["CameraComponent"]["IsResize"]							= cc.IsResize;

		}
		if (entity.HasComponent<SpriteComponent>())
		{
			auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			auto& color = spriteComponent.Color;
			auto& texture = spriteComponent.Texture;
			auto& tilingFactor = spriteComponent.TilingFactor;
			js["Entity"]["SpriteComponent"]["Color"] = { color.r, color.g, color.b, color.a };
			if(texture)
				js["Entity"]["SpriteComponent"]["TexturePath"] = texture->GetPath();
			else
				js["Entity"]["SpriteComponent"]["TexturePath"] = "";
			js["Entity"]["SpriteComponent"]["TilingFactor"] = tilingFactor;
		}
		json = js;
		
	}

}