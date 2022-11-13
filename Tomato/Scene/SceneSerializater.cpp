#include "SceneSerializater.h"

#include "GameObject.h"
#include "Components.h"
#include <fstream>

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
			[&](auto entityID)
			{
				Json temp;
				GameObject GO = { entityID, m_Scene.get() };
	
				if (!GO)
					return false;
	
				SerializeGO(temp, GO);
				json["GameObjects"] += temp;
				
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

		auto GOs = json["GameObjects"];
				if (GOs.empty() == false)
		{
			for(auto go : GOs)
			{
				auto g = go["GameObject"];
								std::string name;
				auto nameComponent = g["NameComponent"]["Name"];
								if (nameComponent.empty() == false)
					name = nameComponent.get<std::string>();

				GameObject deserializedGO = m_Scene->CreateGameObject(name);

				auto tranformComponent = g["TransformComponent"];
				if (tranformComponent.empty() == false)
				{
					auto& tc = deserializedGO.GetComponent<TransformComponent>();
					tc.Position= tranformComponent["Position"].get<glm::vec3>();
					tc.Rotation = tranformComponent["Rotation"].get<glm::vec3>();
					tc.Scale = tranformComponent["Scale"].get<glm::vec3>();
				}

				auto cameraComponent = g["CameraComponent"];
				if (cameraComponent.empty() == false)
				{
					auto& cc = deserializedGO.AddComponent<CameraComponent>();

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

				auto colorComponent = g["ColorComponent"];
				if (colorComponent.empty() == false)
				{
					auto& src = deserializedGO.AddComponent<ColorComponent>();
					src.Color = colorComponent["Color"].get<glm::vec4>();
				}

			}
		}

		return true;
	}

	void SceneSerializater::SerializeGO(Json& json, GameObject GO)
	{
		Json js;
		{
			js["GameObject"]["Gid"] = 121212121212;
		}
		if (GO.HasComponent<NameComponent>())
		{
			auto& name = GO.GetComponent<NameComponent>().Name;
			js["GameObject"]["NameComponent"]["Name"] = name;
		}
		if (GO.HasComponent<TransformComponent>())
		{
			auto& position = GO.GetComponent<TransformComponent>().Position;
			auto& rotation = GO.GetComponent<TransformComponent>().Rotation;
			auto& scale = GO.GetComponent<TransformComponent>().Scale;
			js["GameObject"]["TransformComponent"]["Position"] = { position.x, position.y, position.z };
			js["GameObject"]["TransformComponent"]["Rotation"] = { rotation.x, rotation.y, rotation.z };
			js["GameObject"]["TransformComponent"]["Scale"] = { scale.x, scale.y, scale.z };
		}
		if (GO.HasComponent<CameraComponent>())
		{
			auto& cc = GO.GetComponent<CameraComponent>();
			auto camera = cc.Camera;
			//camera
			js["GameObject"]["CameraComponent"]["Camera"]["ProjectionType"]	= camera.GetSceneCameraType();
			js["GameObject"]["CameraComponent"]["Camera"]["PerspectiveFOV"]	= camera.GetPerspFOV();
			js["GameObject"]["CameraComponent"]["Camera"]["PerspectiveFar"]	= camera.GetPerspFar();
			js["GameObject"]["CameraComponent"]["Camera"]["PerspectiveNear"]	= camera.GetPerspNear();
			js["GameObject"]["CameraComponent"]["Camera"]["OrthographicZoomLevel"]	= camera.GetOrthoZoomLevel();
			js["GameObject"]["CameraComponent"]["Camera"]["OrthographicFar"]	= camera.GetOrthoFar();
			js["GameObject"]["CameraComponent"]["Camera"]["OrthographicNear"] = camera.GetOrthoNear();

			js["GameObject"]["CameraComponent"]["IsMain"] = cc.IsMain;
			js["GameObject"]["CameraComponent"]["IsResize"] = cc.IsResize;

		}
		if (GO.HasComponent<ColorComponent>())
		{
			auto& color = GO.GetComponent<ColorComponent>().Color;
			js["GameObject"]["ColorComponent"]["Color"] = { color.r, color.g, color.b, color.a };
		}
		json = js;
		
	}

}