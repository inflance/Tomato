#pragma once

#include "Shader.h"
#include "RendererCommand.h"
#include "OrthographicCamera.h"

namespace Tomato {

	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void OnWindowResize(int x, int y, uint32_t width, uint32_t height);
		static void Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray, glm::mat4 transform = glm::mat4(1.0f));
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjection;
		};

		static std::shared_ptr<SceneData> m_scene_data;
	};
}