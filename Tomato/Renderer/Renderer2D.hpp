#pragma once

#include <memory>

#include "RenderPass.hpp"
#include "UniformBuffer.hpp"
#include "VertexBuffer.hpp"
#include "Tomato/ECS/Components.h"
#include "Tomato/Function/Camera/Camera.hpp"
#include "Tomato/Function/Camera/EditorCamera.hpp"
#include "Tomato/Renderer/Texture.hpp"
#include "Tomato/Renderer/SubTexture2D.hpp"

namespace Tomato
{
	class Pipeline;
	class IndexBuffer;


	struct Vertex2D
	{
		glm::vec4 position_; // 3 * 4
		glm::vec4 color_; // 4 * 4
		glm::vec2 tex_coord_;
		float tex_index_;
		float tiling_factor_;
	};

	struct TextVertex
	{
		glm::vec3 position_;
		glm::vec4 color_;
		glm::vec2 tex_coord_;
		float tex_index_;
	};

	struct LineVertex
	{
		glm::vec3 position_;
		glm::vec4 color_;
	};

	struct CircleVertex
	{
		glm::vec3 world_position_;
		float thickness_;
		glm::vec2 local_position_;
		glm::vec4 color_;
	};

	struct QuadData
	{
		std::shared_ptr<VertexBuffer> vertex_buffer_;
		std::shared_ptr<IndexBuffer> index_buffer_;
		std::shared_ptr<Pipeline> pipeline_;
		std::vector<Vertex2D> quad_vertex_data_;
	};

	class Renderer2D
	{
	public:
		static void Init();
		static const auto& GetUniformBuffer() { return m_frame_buffer; };
		static void Shutdown();
		static void Begin(EditorCamera& camera);
		static void Begin(Camera& camera, glm::mat4& transform);
		static void Begin(const Ref<EditorCamera>& camera);
		static void End();
		static void Clear();
		static void RenderQuad(const glm::mat4& transform, const glm::vec4& color);
		static void RenderQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
		                       float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void RenderCircle();
		static void RenderLine();

	private:
		inline static std::shared_ptr<RenderPass> m_render_pass = nullptr;
		inline static std::shared_ptr<FrameBuffer> m_frame_buffer = nullptr;
		inline static std::shared_ptr<UniformBuffer> m_uniform_buffer = nullptr;
		inline static QuadData m_quad_data;
		inline static glm::vec4 m_quad_position[4];
		inline static uint32_t m_quad_count{};
	};
}
