#include "Tomato/pch.h"

#include "Renderer2D.hpp"

#include "Shader.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <Tomato/ECS/Entity.h>

#include "FrameBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Pipeline.hpp"
#include "Renderer.hpp"
#include "RenderPass.hpp"
#include "VertexBuffer.hpp"
#include "Tomato/Core/Engine.h"
#include "Tomato/Manager/AssetManager.hpp"

namespace Tomato
{
	inline static constexpr uint32_t MaxQuadCount = 10000;
	inline static constexpr uint32_t MaxVertices = MaxQuadCount * 4;
	inline static constexpr uint32_t MaxIndices = MaxQuadCount * 6;
	inline static constexpr uint32_t MaxTextureSlots = 32;

	inline static constexpr uint32_t MaxLines = 2000;
	inline static constexpr uint32_t MaxLineVertices = MaxLines * 2;
	inline static constexpr uint32_t MaxLineIndices = MaxLines * 6;

	struct UniformBufferObject2D
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
		float lod = 1.0f;
	} ubo;

	void Renderer2D::Init()
	{
		ubo.model = translate(glm::mat4(1.0f), {0.0, 0.0, 1.0});

		m_quad_position[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
		m_quad_position[1] = {0.5f, -0.5f, 0.0f, 1.0f};
		m_quad_position[2] = {0.5f, 0.5f, 0.0f, 1.0f};
		m_quad_position[3] = {-0.5f, 0.5f, 0.0f, 1.0f};

		RenderPassInfo renderPassProps;
		renderPassProps.clear_value = ClearValue();
		renderPassProps.width_ = Engine::Get().GetWindow().GetWidth();
		renderPassProps.height_ = Engine::Get().GetWindow().GetHeight();
		//renderPassProps.sample_count = 8;
		m_render_pass = RenderPass::Create(renderPassProps);

		FrameBufferInfo fb_info;
		fb_info.height_ = Engine::Get().GetWindow().GetHeight();
		fb_info.width_ = Engine::Get().GetWindow().GetWidth();
		fb_info.render_pass = m_render_pass;
		m_frame_buffer = FrameBuffer::Create(fb_info);

		//quad init
		{
			const Ref<Shader> shader = (Shader::Create({
				"Triangle",
				{
					{
						ShaderType::Vertex, ShaderCreateFlag::ByPath, {"PreCompile/Assets/Shader/Quad.vert"},
						{},
						{}
					},
					{
						ShaderType::Fragment, ShaderCreateFlag::ByPath, "PreCompile/Assets/Shader/Quad.frag",
						{},
						{}
					},
				}
			}));
			m_quad_data.vertex_buffer_ = VertexBuffer::Create(sizeof(Vertex2D) * MaxQuadCount);
			/*m_quad_data.vertex_buffer_->SetData(m_quad_data.quad_vertex_data_.data(),
			                                    sizeof(Vertex2D) * m_quad_data.quad_vertex_data_.size());*/
			m_quad_data.vertex_buffer_->SetLayout({
				{ShaderDataType::Float4, "inPosition"},
				{ShaderDataType::Float4, "inColor"},
				{ShaderDataType::Float2, "inTexCoord"},
				{ShaderDataType::Float, "inTexIndex"},
				{ShaderDataType::Float, "inTilingFactor"},
			});


			m_uniform_buffer = UniformBuffer::Create(sizeof(ubo), 0);
			m_uniform_buffer->SetData(&ubo, sizeof(ubo));
			auto texture = AssetManager::LoadWhiteTexture();
			//auto texture = AssetManager::LoadTexture(R"(C:\Users\liyun\source\repos\Tomato\PreCompile\Assets\Image\awesomeface.png)");

			PipelineInfo props;
			props.vertex_layout_ = m_quad_data.vertex_buffer_->GetLayout();
			props.shader_ = shader;
			props.render_pass_ = m_render_pass;
			props.uniform_buffer_set_ = std::make_shared<UniformBufferSet>();
			props.uniform_buffer_set_->SetUniformBuffer(m_uniform_buffer);
			props.uniform_buffer_set_->SetTexture(texture, 1);
			props.uniform_buffer_set_->SetUniformBufferSetLayout({
				{DescriptorType::UniformBuffer, 1, ShaderStageFlags::Vertex, 0, 0},
				{DescriptorType::CombinedImageSample, 1, ShaderStageFlags::Fragment, 1, 0}
			});

			m_quad_data.pipeline_ = Pipeline::Create(props);

			auto quad_indices = new uint32_t[MaxIndices];

			uint32_t offset = 0;
			for (uint32_t i = 0; i < MaxIndices; i += 6)
			{
				quad_indices[i + 0] = offset + 0;
				quad_indices[i + 1] = offset + 1;
				quad_indices[i + 2] = offset + 2;

				quad_indices[i + 3] = offset + 2;
				quad_indices[i + 4] = offset + 3;
				quad_indices[i + 5] = offset + 0;

				offset += 4;
			}
			m_quad_data.index_buffer_ = IndexBuffer::Create(quad_indices, MaxIndices);

			delete[] quad_indices;
		}

		
	
	}

	void Renderer2D::Shutdown()
	{
	}

	void Renderer2D::Begin(EditorCamera& camera)
	{
		ubo.view = camera.GetViewMatrix();
		ubo.proj = camera.GetProjectionMatrix();
		m_uniform_buffer->SetData(&ubo, sizeof(ubo));
	}

	void Renderer2D::Begin(Camera& camera, glm::mat4& transform)
	{
		ubo.view = glm::inverse(transform);
		ubo.proj = camera.GetProjectionMatrix();
		m_uniform_buffer->SetData(&ubo, sizeof(ubo));
	}

	void Renderer2D::Begin(const Ref<EditorCamera>& camera)
	{
		ubo.view = camera->GetViewMatrix();
		ubo.proj = camera->GetProjectionMatrix();
		m_uniform_buffer->SetData(&ubo, sizeof(ubo));
	}

	void Renderer2D::End()
	{
		if(!m_quad_data.quad_vertex_data_.empty())
		{
			m_quad_data.vertex_buffer_->SetData(m_quad_data.quad_vertex_data_.data(),
				sizeof(Vertex2D) * m_quad_data.quad_vertex_data_.size());

			Renderer::BeginRenderPass(m_render_pass, m_frame_buffer);
			Renderer::RenderQuad(nullptr, m_quad_data.pipeline_, m_quad_data.vertex_buffer_,
				m_quad_data.index_buffer_, m_quad_count);
			Renderer::EndRenderPass();
			
		}
		Clear();
	}

	void Renderer2D::Clear()
	{
		m_quad_data.quad_vertex_data_.clear();
		m_quad_count = 0;
	}

	void Renderer2D::RenderQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		constexpr size_t quad_vertex_count = 4;
		constexpr float texture_index = 0.0f;
		constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
		constexpr float tiling_factor = 1.0f;

		for (size_t i = 0; i < quad_vertex_count; i++)
		{
			Vertex2D quad_vertex{};
			quad_vertex.position_ = transform * m_quad_position[i];
			quad_vertex.color_ = color;
			quad_vertex.tex_coord_ = textureCoords[i];
			quad_vertex.tex_index_ = texture_index;
			quad_vertex.tiling_factor_ = tiling_factor;
			m_quad_data.quad_vertex_data_.emplace_back(quad_vertex);
		}
		m_quad_count += 6;
	}

	void Renderer2D::RenderQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
	                            float tilingFactor, const glm::vec4& tintColor)
	{
	}

	void Renderer2D::RenderCircle()
	{
	}

	void Renderer2D::RenderLine()
	{
	}
}
