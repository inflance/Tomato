#include "Renderer2D.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "Tomato/Core/Engine.h"
#include "Tomato/Function/Camera/EditorCamera.hpp"

#include "Tomato/Manager/AssetManager.hpp"
#include "Tomato/Renderer/Material.hpp"

#include "Tomato/ECS/Entity.h"
#include "Tomato/ECS/Components.h"

#include "Tomato/Renderer/Shader.hpp"
#include "Tomato/Renderer/SubTexture2D.hpp"
#include "Tomato/Renderer/Texture.hpp"
#include "Tomato/Renderer/FrameBuffer.hpp"
#include "Tomato/Renderer/VertexBuffer.hpp"
#include "Tomato/Renderer/IndexBuffer.hpp"
#include "Tomato/Renderer/Renderer.hpp"
#include "Tomato/Renderer/UniformBuffer.hpp"

namespace Tomato
{
	struct UBO
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
		float lod = 1.0f;
	} ubo;

	struct LightUBO
	{
		Light light[50];
		Material materials[50];
		alignas(16)glm::vec3 camera_pos{};
		uint32_t light_count{};
	} light_ubo;

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

		m_render_pass = RenderPass::Create(renderPassProps);

		FrameBufferInfo fb_info;
		fb_info.height_ = Engine::Get().GetWindow().GetHeight();
		fb_info.width_ = Engine::Get().GetWindow().GetWidth();
		fb_info.render_pass = m_render_pass;
		m_frame_buffer = FrameBuffer::Create(fb_info);

		m_uniform_buffer = UniformBuffer::Create(sizeof(ubo), 0);
		m_uniform_buffer->SetData(&ubo, sizeof(ubo));

		m_light_uniform_buf = UniformBuffer::Create(sizeof(light_ubo), 1);
		m_light_uniform_buf->SetData(&light_ubo, sizeof(light_ubo));
		auto texture = AssetManager::LoadWhiteTexture();
		//AssetManager::LoadWhiteTexture();

		//quad init
		{
			const Ref<Shader> quad_shader = (Shader::Create({
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
			m_quad_data.vertex_buffer_->SetLayout({
				{ShaderDataType::Float3, "inPosition"},
				{ShaderDataType::Float3, "inNormal"},
				{ShaderDataType::Float4, "inColor"},
				{ShaderDataType::Float2, "inTexCoord"},
				{ShaderDataType::Float, "inTexIndex"},
				{ShaderDataType::Float, "inTilingFactor"},
			});


			PipelineInfo props;
			props.vertex_layout_ = m_quad_data.vertex_buffer_->GetLayout();
			props.shader_ = quad_shader;
			props.render_pass_ = m_render_pass;
			props.uniform_buffer_set_ = std::make_shared<UniformBufferSet>();
			props.uniform_buffer_set_->SetUniformBuffer(m_uniform_buffer);
			props.uniform_buffer_set_->SetUniformBuffer(m_light_uniform_buf);
			props.uniform_buffer_set_->SetTexture(texture, 2);
			props.uniform_buffer_set_->SetUniformBufferSetLayout({
				{DescriptorType::UniformBuffer, 1, ShaderStageFlags::Vertex, 0, 0},
				{DescriptorType::UniformBuffer, 1, ShaderStageFlags::Fragment, 1, 0},
				{DescriptorType::CombinedImageSample, 1, ShaderStageFlags::Fragment, 2, 0}
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
			//m_quad_data.quad_vertex_data_.resize(MaxQuadCount);
			delete[] quad_indices;
			m_quad_data.quad_vertex_data_ = new Vertex2D[MaxQuadCount];
			m_quad_data.quad_vertex_ptr_ = m_quad_data.quad_vertex_data_;
		}
		{
			const Ref<Shader> circle_shader = (Shader::Create({
				"Circle",
				{
					{
						ShaderType::Vertex, ShaderCreateFlag::ByPath, {"PreCompile/Assets/Shader/Circle.vert"},
						{},
						{}
					},
					{
						ShaderType::Fragment, ShaderCreateFlag::ByPath, "PreCompile/Assets/Shader/Circle.frag",
						{},
						{}
					},
				}
			}));
			m_circle_data.vertex_buffer_ = VertexBuffer::Create(sizeof(CircleVertex) * MaxQuadCount);
			m_circle_data.vertex_buffer_->SetLayout({
				{ShaderDataType::Float3, "inWorldPosition"},
				{ShaderDataType::Float, "inThickness"},
				{ShaderDataType::Float4, "inColor"},
				{ShaderDataType::Float2, "inLocalPosition"},
			});

			//auto texture = AssetManager::LoadTexture(R"(C:\Users\liyun\source\repos\Tomato\PreCompile\Assets\Image\awesomeface.png)");

			PipelineInfo props;
			props.vertex_layout_ = m_circle_data.vertex_buffer_->GetLayout();
			props.shader_ = circle_shader;
			props.render_pass_ = m_render_pass;
			props.uniform_buffer_set_ = std::make_shared<UniformBufferSet>();
			props.uniform_buffer_set_->SetUniformBuffer(m_uniform_buffer);
			props.uniform_buffer_set_->SetTexture(texture, 1);
			props.uniform_buffer_set_->SetUniformBufferSetLayout({
				{DescriptorType::UniformBuffer, 1, ShaderStageFlags::Vertex, 0, 0},
				{DescriptorType::CombinedImageSample, 1, ShaderStageFlags::Fragment, 1, 0}
			});

			m_circle_data.pipeline_ = Pipeline::Create(props);
			//m_circle_data.circle_vertex_data_.resize(MaxQuadCount);
			m_circle_data.index_buffer_ = m_quad_data.index_buffer_;
		}
		//default is White Texture
		m_texture_array[0] = texture;
		m_texture_index++;
	}

	void Renderer2D::Shutdown()
	{
		delete[] m_quad_data.quad_vertex_data_;
	}

	void Renderer2D::Begin(EditorCamera& camera)
	{
		ubo.view = camera.GetViewMatrix();
		ubo.proj = camera.GetProjectionMatrix();
		m_uniform_buffer->SetData(&ubo, sizeof(ubo));
		light_ubo.camera_pos = camera.GetPosition();
	}

	void Renderer2D::Begin(Camera& camera, glm::mat4& transform)
	{
		ubo.view = inverse(transform);
		ubo.proj = camera.GetProjectionMatrix();
		m_uniform_buffer->SetData(&ubo, sizeof(ubo));
	}

	void Renderer2D::Begin(const Ref<EditorCamera>& camera)
	{
		ubo.view = camera->GetViewMatrix();
		ubo.proj = camera->GetProjectionMatrix();
		m_uniform_buffer->SetData(&ubo, sizeof(ubo));
		light_ubo.camera_pos = camera->GetPosition();
	}

	void Renderer2D::End()
	{
		//submit light
		light_ubo.light_count = m_light_count;
		m_light_uniform_buf->SetData(&light_ubo, sizeof(light_ubo));
		// render quad
		Renderer::BeginRenderPass(m_render_pass, m_frame_buffer);
		{
			if (m_quad_vertex_count)
			{
				//if vertex buffer has value , set the vertex buffer data
				m_quad_data.vertex_buffer_->SetData(m_quad_data.quad_vertex_data_,
				                                    m_quad_vertex_count * sizeof(Vertex2D));
			}
			else
			{
				//else reset the vertex buffer
				m_quad_data.vertex_buffer_ = VertexBuffer::Create(sizeof(Vertex2D) * MaxQuadCount);
			}


			Renderer::RenderIndexed(nullptr, m_quad_data.pipeline_, m_quad_data.vertex_buffer_,
			                        m_quad_data.index_buffer_, m_quad_index_count);
		}

		// render circle
		{
			if (!m_circle_data.circle_vertex_data_.empty())
			{
				//if vertex buffer has value , set the vertex buffer data
				m_circle_data.vertex_buffer_->SetData(m_circle_data.circle_vertex_data_.data(),
				                                      sizeof(CircleVertex) * m_circle_data.circle_vertex_data_.size());
			}
			else
			{
				//else reset the vertex buffer
				m_circle_data.vertex_buffer_ = VertexBuffer::Create(sizeof(CircleVertex) * MaxQuadCount);
			}

			Renderer::RenderIndexed(nullptr, m_circle_data.pipeline_, m_circle_data.vertex_buffer_,
			                        m_circle_data.index_buffer_, m_circle_index_count);
		}
		Renderer::EndRenderPass();

		Clear();
	}

	void Renderer2D::Clear()
	{
		m_quad_data.quad_vertex_ptr_ = m_quad_data.quad_vertex_data_;
		m_quad_index_count = 0;
		m_quad_vertex_count = 0;

		m_circle_data.circle_vertex_data_.clear();
		m_circle_index_count = 0;

		m_texture_index = 1;
		m_light_count = 0;
	}

	void Renderer2D::RenderQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		RenderQuad(transform, Ref<Texture2D>(nullptr), 1.0f, color);
	}

	void Renderer2D::RenderQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tiling_factor,
	                            const glm::vec4& color)
	{
		float texture_index = 0.0f;

		if (texture != nullptr)
		{
			for (uint32_t i = 1; i < m_texture_index; i++)
			{
				if (m_texture_array[i].get() == texture.get())
				{
					texture_index = i;
					break;
				}
			}

			if (texture_index == 0.0f)
			{
				texture_index = static_cast<float>(m_texture_index);
				m_texture_array[m_texture_index] = texture;
				m_texture_index++;
			}
		}

		constexpr size_t quad_vertex_count = 4;
		constexpr glm::vec2 texture_coords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

		for (size_t i = 0; i < quad_vertex_count; i++)
		{
			m_quad_data.quad_vertex_ptr_->position_ = transform * m_quad_position[i];
			m_quad_data.quad_vertex_ptr_->normal_ = normalize(transform * glm::vec4(
				cross(glm::vec3(m_quad_position[1] - m_quad_position[0]),
					glm::vec3(m_quad_position[2] - m_quad_position[0])), 1.0f));
			m_quad_data.quad_vertex_ptr_->color_ = color;
			m_quad_data.quad_vertex_ptr_->tex_coord_ = texture_coords[i];
			m_quad_data.quad_vertex_ptr_->tex_index_ = 0;
			m_quad_data.quad_vertex_ptr_->tiling_factor_ = tiling_factor;
			m_quad_data.quad_vertex_ptr_++;
			m_quad_vertex_count++;
		}
		m_quad_index_count += 6;
	}

	void Renderer2D::RenderQuad(const glm::mat4& transform, const Ref<SubTexture2D>& sub_texture, float tiling_factor,
	                            const glm::vec4& color)
	{
		const auto texture = sub_texture->GetTexture();
		float texture_index = 0.0f;

		for (size_t i = 0; i < m_texture_index; i++)
		{
			if (*m_texture_array[i] == *texture)
			{
				texture_index = i;
				break;
			}
		}

		if (texture_index == 0.0f)
		{
			texture_index = static_cast<float>(m_texture_index);
			m_texture_array[m_texture_index] = texture;
			m_texture_index++;
		}

		constexpr size_t quad_vertex_count = 4;
		constexpr glm::vec2 texture_coords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};


		for (size_t i = 0; i < quad_vertex_count; i++)
		{
			m_quad_data.quad_vertex_ptr_->position_ = transform * m_quad_position[i];
			m_quad_data.quad_vertex_ptr_->normal_ = normalize(transform * glm::vec4(
				cross(glm::vec3(m_quad_position[1] - m_quad_position[0]),
					glm::vec3(m_quad_position[2] - m_quad_position[0])), 1.0f));
			m_quad_data.quad_vertex_ptr_->color_ = color;
			m_quad_data.quad_vertex_ptr_->tex_coord_ = texture_coords[i];
			m_quad_data.quad_vertex_ptr_->tex_index_ = 0;
			m_quad_data.quad_vertex_ptr_->tiling_factor_ = tiling_factor;
			m_quad_data.quad_vertex_ptr_++;
			m_quad_vertex_count++;
		}
		m_quad_index_count += 6;
	}

	void Renderer2D::RenderQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
	                            float tiling_factor, const glm::vec4& color)
	{
		RenderQuad({position.x, position.y, 0.0f}, size, texture, tiling_factor, color);
	}

	void Renderer2D::RenderQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
	                            float tiling_factor, const glm::vec4& color)
	{
		constexpr size_t quad_vertex_count = 4;
		constexpr glm::vec2 texture_coords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

		const auto transform = translate(glm::mat4(1.0f), position) * scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});


		for (size_t i = 0; i < quad_vertex_count; i++)
		{
			m_quad_data.quad_vertex_ptr_->position_ = transform * m_quad_position[i];
			m_quad_data.quad_vertex_ptr_->normal_ = normalize(transform * glm::vec4(
				cross(glm::vec3(m_quad_position[1] - m_quad_position[0]),
					glm::vec3(m_quad_position[2] - m_quad_position[0])), 1.0f));
			m_quad_data.quad_vertex_ptr_->color_ = color;
			m_quad_data.quad_vertex_ptr_->tex_coord_ = texture_coords[i];
			m_quad_data.quad_vertex_ptr_->tex_index_ = 0;
			m_quad_data.quad_vertex_ptr_->tiling_factor_ = tiling_factor;
			m_quad_data.quad_vertex_ptr_++;
			m_quad_vertex_count++;
		}
		m_quad_index_count += 6;
	}


	void Renderer2D::RenderCircle(const glm::mat4& transform, const glm::vec4& color)
	{
		for (auto& i : m_quad_position)
		{
			CircleVertex circle_vertex{};
			circle_vertex.world_position_ = transform * i;
			circle_vertex.thickness_ = 1.0f;
			circle_vertex.local_position_ = i * 2.0f;
			circle_vertex.color_ = color;
			m_circle_data.circle_vertex_data_.emplace_back(circle_vertex);
		}
		m_circle_index_count += 6;
	}

	void Renderer2D::RenderText(const std::string& str, const glm::mat4& transform, const glm::vec4& color)
	{
	}

	void Renderer2D::RenderLine()
	{
	}

	void Renderer2D::RenderLight(const Light& light)
	{
		light_ubo.light[m_light_count] = light;
		++m_light_count;
	}
}
