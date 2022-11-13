#pragma once

#include "Tomato/Scene/Scene.h"

#include "Tomato/Scene/GameObject.h"

#include <glm/glm.hpp>
#include "Tomato/Renderer/Texture.h"

namespace Tomato{

	class ScenePanel
	{
	public:
		ScenePanel() = default;
		~ScenePanel() = default;

		ScenePanel(const std::shared_ptr<Scene>& context);

		void OnImGuiRenderer();

		void SetContex(const std::shared_ptr<Scene>& context);

		void DrawScenePanel(GameObject GO);
		void DrewDetailPanel(GameObject GO);

		template<typename T, typename Func>
		void DrawComponents(const std::string& name, GameObject GO, Func uiFunction);

		void DrawVector3(const std::string& label, glm::vec3& values, float speed = 0.1f,const glm::vec3& defaltValue = glm::vec3(0.0f));

		GameObject GetSelectedGO() const { return m_SelectedGO; }

	private:
		glm::vec3 dPosition{0.0f};
		glm::vec3 dScale{1.0f};
		glm::vec3 dRotation{0.0f};

		std::shared_ptr<Scene> m_Context;
		GameObject m_SelectedGO;
		bool m_isFirst = true;

		std::shared_ptr<Texture2D> m_moreBtn;
	};

	

}