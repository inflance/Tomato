#pragma once

#include <string>
#include <entt/entt.hpp>

#include "Tomato/Function/Camera/EditorCamera.hpp"
#include "Tomato/Renderer/Shader.hpp"
#include "Tomato/Core/Core.hpp"
#include "Tomato/ECS/EntityRegistry.h"

namespace Tomato
{
	class Entity;

	class Scene
	{
	public:
		Scene() = default;

		Scene(const std::string& sceneName)
			: m_scene_name(sceneName)
		{
		}

		virtual ~Scene() = default;

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateSprite(const std::string& name = std::string()) const;
		Entity CreateCamera(const std::string& name = std::string());
#if 0
		Entity CreateStaticMesh(const std::string& name = std::string());
		Entity CreateBaseShape(const std::string& name = std::string());
		Entity CreateLight(const std::string& name = std::string());
#endif
		//void TickEditor(float deltaTime, const EditorCamera& camera);

		void Tick(float deltaTime);

		void SetViewPortResize(float width, float height);

		void DestroyEntity(Entity);

		void Clear();
	private:
		std::string m_scene_name = "Default";
		Ref<EntityRegistry> m_entity_registry{};
		float m_view_port_width = 0.0f;
		float m_view_port_height = 0.0f;

		friend class Entity;
		friend class ScenePanel;
		friend class SceneSerializater;
	};
}
