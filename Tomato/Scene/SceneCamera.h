#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "Tomato/Function/Camera/Camera.h"

namespace Tomato
{
	enum class SceneCameraType
	{
		Orthographic = 0,
		Perspective = 1
	};

	class SceneCamera : public Camera
	{
	public:
		SceneCamera() = default;
		~SceneCamera() override = default;

		void SetOrthgraphic(float zoomLevel, float orthoNear, float orthoFar);
		void SetPerspective(float FOV, float perspNear, float perspFar);


		float GetOrthoFar() const { return m_ortho_far; };
		void SetOrthoFar(float orthoFar);

		float GetOrthoNear() const { return m_ortho_near; };
		void SetOrthoNear(float orthoNear);

		void SetOrthoZoomLevel(float zoomLevel);
		float GetOrthoZoomLevel() const { return m_ortho_zoom_level; }


		float GetPerspFar() const { return m_persp_far; };
		void SetPerspFar(float perspFar);

		float GetPerspNear() const { return m_persp_near; };
		void SetPerspNear(float perspNear);

		void SetPerspFOV(float FOV);
		float GetPerspFOV() const { return m_persp_FOV; }

		void SetSceneCameraType(SceneCameraType type);
		[[nodiscard]] SceneCameraType GetSceneCameraType() const { return m_SceneCameraType; }

		void SetViewPortSize(float width, float height);

		void RecalculationProjection();

	private:
		float m_ortho_zoom_level = 1.0f;
		float m_ortho_near = 1.0f;
		float m_ortho_far = -1.0f;
		float m_ortho_aspectRadio = 0.0f;

		float m_persp_FOV = glm::radians(45.0f);
		float m_persp_near = 0.01f;
		float m_persp_far = 1000.0f;
		float m_persp_aspectRadio = 0.0f;

		SceneCameraType m_SceneCameraType;
	};
}
