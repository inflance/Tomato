#pragma once

#include <string>
#include "Tomato/TimeSpan.h"
#include <entt/entt.hpp>

namespace Tomato {

	class GameObject;

	class Scene
	{
	public:
		Scene() = default;
		virtual ~Scene() = default;
		
		GameObject CreateGameObject(const std::string& name = std::string());

		void Tick(TimeSpan ts);

		void SetViewPortResize(float width, float height);

		void DestroyEntity(GameObject GO);

		GameObject GetMainCameraGO();

	private:
		entt::registry m_Registry;

		float m_viewPortWidth = 0.0f;
		float m_viewPortHeight = 0.0f;
		
		friend class GameObject;
		friend class ScenePanel;
		friend class SceneSerializater;
	};
}