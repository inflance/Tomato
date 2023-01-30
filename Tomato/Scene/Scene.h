#pragma once

#include <string>
#include <entt/entt.hpp>

#include "Tomato/Function/Camera/EditorCamera.h"
#include "Tomato/Renderer/Shader.h"
#include "Tomato/Core/Core.h"

namespace Tomato
{
	class Entity;

	class Scene
	{
	public:
		Scene() = default;

		Scene(const std::string& sceneName)
			: m_sceneName(sceneName)
		{
		}

		virtual ~Scene() = default;

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateSprite(const std::string& name = std::string());
		Entity CreateCamera(const std::string& name = std::string());
		Entity CreateStaticMesh(const std::string& name = std::string());
		Entity CreateBaseShape(const std::string& name = std::string());
		Entity CreateLight(const std::string& name = std::string());

		void TickEditor(float deltaTime, const EditorCamera& camera);

		void Tick(float deltaTime);

		void SetViewPortResize(float width, float height);

		void DestroyEntity(Entity);

		void Clear();

		Entity GetMainCameraEntity();

	private:
		std::string m_sceneName = "Default";
		entt::registry m_Registry;

		float m_viewPortWidth = 0.0f;
		float m_viewPortHeight = 0.0f;

		friend class Entity;
		friend class ScenePanel;
		friend class SceneSerializater;
	};
}
