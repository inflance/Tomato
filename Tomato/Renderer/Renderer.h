#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "BaseShape.h"
#include "RendererCommand.h"
#include "OrthographicCamera.h"
#include "Tomato/Scene/Light.h"

namespace Tomato {

	class Renderer
	{
	public:
		//≥ı ºªØ‰÷»æ∆˜
		static void Init();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		//‰÷»ææ≤Ã¨Õ¯∏Ò
		static void RenderBaseShape(Mesh& mesh, const glm::mat4& ViewProjection, const glm::mat4& transform);
		static void RenderBaseShapeWithMatirial(BaseShape& baseShap, const glm::vec4& color ,Matirial& matirial,const glm::mat4& ViewProjection, const glm::mat4& transform);
		static void RenderBaseShape(BaseShape& baseShap, const glm::vec4& color,const glm::mat4& ViewProjection, const glm::mat4& transform);
		static void RenderBaseLight(BaseShape& baseShap,Light& baseLight,const glm::mat4& ViewProjection, const glm::mat4& transform, const glm::vec3& lightPos ,int pointLightSize);
		
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