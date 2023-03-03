#pragma once

#include <memory>

#include "Tomato/Core/Macro.h"
#include "Tomato/Renderer/RenderPass.hpp"
#include "Tomato/Renderer/Pipeline.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>

namespace Tomato
{
	struct Light;
	class UniformBuffer;
	class SubTexture2D;
	class Texture2D;
	class VertexBuffer;
	class Camera;
	class EditorCamera;
	class FrameBuffer;
	class IndexBuffer;

	inline static constexpr uint32_t MaxQuadCount = 10000;
	inline static constexpr uint32_t MaxVertices = MaxQuadCount * 4;
	inline static constexpr uint32_t MaxIndices = MaxQuadCount * 6;
	inline static constexpr uint32_t MaxTextures = 32;

	inline static constexpr uint32_t MaxLines = 2000;
	inline static constexpr uint32_t MaxLineVertices = MaxLines * 2;
	inline static constexpr uint32_t MaxLineIndices = MaxLines * 6;

	struct Vertex2D
	{
		glm::vec3 position_; // 3 * 4
		glm::vec3 normal_; // 3 * 4
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
		glm::vec4 color_;
		glm::vec2 local_position_;
	};

	struct CircleData
	{
		std::shared_ptr<VertexBuffer> vertex_buffer_;
		std::shared_ptr<IndexBuffer> index_buffer_;
		std::shared_ptr<Pipeline> pipeline_;
		std::vector<CircleVertex> circle_vertex_data_;
	};

	struct QuadData
	{
		std::shared_ptr<VertexBuffer> vertex_buffer_;
		std::shared_ptr<IndexBuffer> index_buffer_;
		std::shared_ptr<Pipeline> pipeline_;
		Vertex2D* quad_vertex_data_;
		Vertex2D* quad_vertex_ptr_;
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
		static void RenderQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tiling_factor = 1.0f,
		                       const glm::vec4& color = glm::vec4(1.0f));
		static void RenderQuad(const glm::mat4& transform, const Ref<SubTexture2D>& sub_texture, float tiling_factor = 1.0f,
		                       const glm::vec4& color = glm::vec4(1.0f));
		static void RenderQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
		                       float tiling_factor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
		static void RenderQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
			float tiling_factor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));


		static void RenderCircle(const glm::mat4& transform, const glm::vec4& color);
		static void RenderText(const std::string& str, const glm::mat4& transform, const glm::vec4& color);
		static void RenderLine();
		static void RenderLight(const Light& light);

	private:
		inline static std::shared_ptr<RenderPass> m_render_pass = nullptr;
		inline static std::shared_ptr<FrameBuffer> m_frame_buffer = nullptr;
		inline static std::shared_ptr<UniformBuffer> m_uniform_buffer = nullptr;
		inline static std::shared_ptr<UniformBuffer> m_light_uniform_buf = nullptr;

		inline static QuadData m_quad_data;
		inline static glm::vec4 m_quad_position[4];
		inline static glm::vec4 m_normal[4];
		inline static uint32_t m_quad_index_count{};
		inline static uint32_t m_quad_vertex_count{};

		inline static CircleData m_circle_data;
		inline static uint32_t m_circle_index_count{};
		inline static uint32_t m_circle_vertex_count{};

		inline static std::array<Ref<Texture2D>, MaxTextures> m_texture_array {nullptr};
		inline static uint32_t m_texture_index{};

		inline static uint32_t m_light_count{};
	};
}
