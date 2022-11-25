#pragma once

#include <string>
#include <entt/entt.hpp>

#include "Tomato/TimeSpan.h"
#include "Tomato/EditorCamera.h"

namespace Tomato {

	class Entity;

	class Scene
	{
	public:
		Scene() = default;
		Scene(const std::string& scenenName)
			:m_sceneName(m_sceneName) {}

		virtual ~Scene() = default;
		
		Entity CreateEntity(const std::string& name = std::string());

		void TickEditor(float deltaTime, const EditorCamera& camera);

		void Tick(TimeSpan ts);

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