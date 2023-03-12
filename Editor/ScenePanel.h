#pragma once

#include <glm/glm.hpp>

#include "Tomato/ECS/Entity.h"
#include "Tomato/Scene/Scene.hpp"
#include "Tomato/Renderer/Texture.hpp"

namespace Tomato
{
	class ScenePanel
	{
	public:
		ScenePanel() = default;
		~ScenePanel() = default;

		ScenePanel(const std::shared_ptr<Scene>& context);

		void OnImGuiRenderer();

		void SetContext(const std::shared_ptr<Scene>& context);

		void DrawScenePanel(Entity entity);
		void DrewDetailPanel(Entity entity);

		template <typename T, typename Func>
		void DrawComponents(const std::string& name, Entity entity, Func uiFunction);

		void DrawVector3(const std::string& label, glm::vec3& values, float speed = 0.1f,
			const glm::vec3& defaltValue = glm::vec3(0.0f));

		[[nodiscard]] Entity GetSelectedEntity() const { return m_selectedEntity; }

		void SetSelectedEntity(Entity entity)
		{
			m_selectedEntity = entity;
		}

	private:
		glm::vec3 dPosition{ 0.0f };
		glm::vec3 dScale{ 1.0f };
		glm::vec3 dRotation{ 0.0f };

		std::shared_ptr<Scene> m_context;
		Entity m_selectedEntity;
		bool m_isFirst = true;

		std::shared_ptr<Texture2D> m_moreBtn;
		std::shared_ptr<Texture2D> m_default_texture;
	};
}
