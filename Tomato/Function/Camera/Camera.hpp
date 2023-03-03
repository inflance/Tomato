#pragma once

#include <glm/glm.hpp>

#ifdef TMT_PLATFORM_WINDOWS
#undef near
#undef far
#endif

namespace Tomato
{
	enum class CameraType
	{
		Orthographic = 0,
		Perspective
	};

	class Camera
	{
	public:
		using Type = CameraType;

		Camera() { SetCameraType(Type::Perspective); }
		Camera(float fov, float aspect, float near, float far);
		Camera(float aspect, float scale);
		Camera(float pitch, float yaw, const glm::vec3& position, float fov, float near, float far, float aspect);
		~Camera() = default;

		[[nodiscard]] auto GetCameraType() const { return m_camera_type; }

		void SetCameraType(Type type)
		{
			m_camera_type = type;
			if (m_camera_type == CameraType::Orthographic)
			{
				SetFar(-10);
				SetNear(10);
			}
			else
			{
				SetFar(1000.0f);
				SetNear(0.001f);
			}
			UpdateProjectionMatrix();
		}

		[[nodiscard]] auto GetAspectRatio() const { return m_aspect_ratio; }

		void SetAspectRatio(float aspectRatio) { m_aspect_ratio = aspectRatio; }

		[[nodiscard]] auto GetFar() const { return m_far; }

		[[nodiscard]] auto GetNear() const { return m_near; }

		void SetFar(float far) { m_far = far; }

		void SetNear(float near) { m_near = near; }

		[[nodiscard]] auto GetFov() const { return m_fov; }

		[[nodiscard]] auto GetScale() const { return m_scale; }

		void SetScale(float scale) { m_scale = scale; }

		void SetFov(float fov) { m_fov = fov; }

		void SetZoomLevel(float zoom_level) { m_zoom_level = zoom_level; }
		[[nodiscard]] auto GetZoomLevel() const { return m_zoom_level; }

		[[nodiscard]] glm::mat4& GetProjectionMatrix() { return m_project_matrix; };

		void SetProjectionMatrix(const glm::mat4& projection)
		{
			m_project_matrix = projection;
		}

		void UpdateProjectionMatrix();

	protected:
		float m_aspect_ratio = 1.0f;
		float m_scale = 100.0f;
		float m_zoom_level = 1.0f;

		float m_fov = glm::radians(45.0f);
		float m_near = 0.0f;
		float m_far = 0.0f;

		CameraType m_camera_type = CameraType::Perspective;
		glm::mat4 m_project_matrix = glm::mat4(1.f);
	};
}
