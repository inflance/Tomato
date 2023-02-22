#include "SceneCamera.h"

namespace Tomato {

	void SceneCamera::SetOrthgraphic(float zoomLevel, float orthoNear, float orthoFar)
	{
		m_SceneCameraType = SceneCameraType::Orthographic;
		m_ortho_zoom_level = zoomLevel;
		m_ortho_near = orthoNear;
		m_ortho_far = orthoFar;
		RecalculationProjection();
	}

	void SceneCamera::SetViewPortSize(float width, float height)
	{
		m_ortho_aspectRadio = width / height;
		m_persp_aspectRadio = width / height;
		RecalculationProjection();
	}

	void SceneCamera::RecalculationProjection()
	{
		if (m_SceneCameraType == SceneCameraType::Orthographic)
		{
			float left = -m_ortho_zoom_level * m_ortho_aspectRadio;
			float right = m_ortho_zoom_level * m_ortho_aspectRadio;
			float top = m_ortho_zoom_level;
			float bottom = -m_ortho_zoom_level;
			m_projection = glm::ortho(left, right, bottom, top, m_ortho_near, m_ortho_far);
		}
		else 
		{
			m_projection = glm::perspective(m_persp_FOV, m_persp_aspectRadio, m_persp_near, m_persp_far);
		}
		
	}

	
	void SceneCamera::SetOrthoFar(float orthoFar)
	{
		m_ortho_far = orthoFar;
		RecalculationProjection();
	}

	void SceneCamera::SetOrthoNear(float orthoNear)
	{
		m_ortho_near = orthoNear;
		RecalculationProjection();
	}

	void SceneCamera::SetOrthoZoomLevel(float zoomLevel)
	{
		m_ortho_zoom_level = zoomLevel;
		RecalculationProjection();
	}

	void SceneCamera::SetPerspective(float FOV, float perspNear, float perspFar)
	{
		m_SceneCameraType = SceneCameraType::Perspective;
		m_persp_FOV = FOV;
		m_persp_near = perspNear;
		m_persp_far = perspFar;
		RecalculationProjection();
	}

	void SceneCamera::SetPerspFar(float perspFar)
	{
		m_persp_far = perspFar;
		RecalculationProjection();
	}

	void SceneCamera::SetPerspNear(float perspNear)
	{
		m_persp_near = perspNear;
		RecalculationProjection();
	}

	void SceneCamera::SetPerspFOV(float FOV)
	{
		m_persp_FOV = FOV;
		RecalculationProjection();
	}

	void SceneCamera::SetSceneCameraType(SceneCameraType type)
	{
		m_SceneCameraType = type;
		RecalculationProjection();
	}

}


