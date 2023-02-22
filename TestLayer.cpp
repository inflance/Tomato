#include "TestLayer.h"

#include "Tomato/Tomato.h"
#include "Tomato/Renderer/Pipeline.h"
#include "Tomato/Renderer/RenderPass.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Tomato/Renderer/VertexBuffer.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "Tomato/Manager/AssetManager.h"

namespace Tomato
{
	struct VertexData
	{
		glm::vec3 Position{};
		glm::vec3 Color{};
		glm::vec2 TexCoord{};
	};


	VertexData vertices[] = {
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

		{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
	};

	VertexData vertices1[] = {
		{{-0.5f, -0.5f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, 0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

		{{-0.5f, -0.5f, -1.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, -1.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, 0.5f, -1.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, -1.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
	};


	std::vector<uint32_t> indices = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};

	struct UniformBufferObject
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
		float lod = 1.0f;
	} ubo, ubo1;


	void TestLayer::OnCreate()
	{
		ubo.model = translate(glm::mat4(1.0f), {0.0, 0.0, 1.0}) * rotate(
				glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
			rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		////ubo.view = lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		////ubo.proj = glm::perspective(glm::radians(45.0f), 1600 / 800.0f, 0.1f, 10.0f);


		m_camera = std::make_shared<Camera>();

		m_controller = CameraController(m_camera);
		m_controller.SetCameraTickFnc([](Ref<Camera>& camera, float deltaTime)
		{
			//LOG_INFO("{} {}", static_cast<int>(camera->GetCameraType()), deltaTime);
		});

		m_editor_camera = std::make_shared<EditorCamera>(glm::radians(45.0f), 1600 / 800.0f, 0.1f, 10.0f);
		m_editor_camera->SetCameraType(CameraType::Perspective);

		m_model = std::make_shared<Model>(R"(C:\Users\liyun\source\repos\Tomato\PreCompile\Assets\Mesh\cube.obj)");

		const Ref<Shader> shader = (Shader::Create({
			"Triangle",
			{
				{
					ShaderType::Vertex, ShaderCreateFlag::ByPath, {"PreCompile/Assets/Shader/BasicTexture.vert"}, {},
					{}
				},
				{
					ShaderType::Fragment, ShaderCreateFlag::ByPath, "PreCompile/Assets/Shader/BasicTexture.frag", {},
					{}
				},
			}
		}));


		vertex_buffer = (VertexBuffer::Create(sizeof(Vertex) * m_model->GetMeshData().vertices_.size()));
		vertex_buffer->SetData(m_model->GetMeshData().vertices_.data(),
		                       sizeof(Vertex) * m_model->GetMeshData().vertices_.size());
		vertex_buffer->SetLayout({
			{ShaderDataType::Float3, "inPosition"},
			{ShaderDataType::Float3, "inNormal"},

			{ShaderDataType::Float3, "inColor"},
			{ShaderDataType::Float2, "inTexCoord"},
		});


		RenderPassProps renderPassProps;
		renderPassProps.ClearValue = ClearValue();
		Ref<RenderPass> renderPass = RenderPass::Create(renderPassProps);

		uniformBuffer = UniformBuffer::Create(sizeof(ubo), 0);
		uniformBuffer->SetData(&ubo, sizeof(ubo));
		//	Ref<UniformBuffer> uniform = UniformBuffer::Create(sizeof(ubo), 0);
		texture = AssetManager::LoadWhiteTexture();
		/*	uint8_t pixel[] = { 0xff, 0xff, 0xff, 0xff };
			texture = Texture2D::Create(pixel, 1, 1);*/

		PipelineInfo props;
		props.vertex_layout_ = vertex_buffer->GetLayout();
		props.shader_ = shader;
		props.render_pass_ = renderPass;
		props.uniform_buffer_set_ = std::make_shared<UniformBufferSet>();
		props.uniform_buffer_set_->SetUniformBuffer(uniformBuffer);
		props.uniform_buffer_set_->SetTexture(texture, 1);
		props.uniform_buffer_set_->SetUniformBufferSetLayout({
			{DescriptorType::UniformBuffer, 1, ShaderStageFlags::Vertex, 0, 0},
			{DescriptorType::CombinedImageSample, 1, ShaderStageFlags::Fragment, 1, 0}
		});
		pipeline = (Pipeline::Create(props));

		index_buffer = IndexBuffer::Create(m_model->GetMeshData().indices_.data(),
		                                   m_model->GetMeshData().indices_.size());
	}

	void TestLayer::OnDestroy()
	{
	}

	void TestLayer::Tick(float delta_time)
	{
		auto instance = this;
		m_editor_camera->Tick(delta_time);
		ubo.proj = m_editor_camera->GetProjectionMatrix();
		Renderer::Submit([instance, delta_time]
			{
				//ubo.model = rotate(ubo.model, delta_time * glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				ubo.view = instance->m_editor_camera->GetViewMatrix();

				instance->uniformBuffer->SetData(&ubo, sizeof(ubo));
				Renderer::RenderQuad(nullptr, instance->pipeline, instance->vertex_buffer, instance->index_buffer);
				//uniformBuffer->SetData(&ubo1, sizeof(ubo1));
				//Renderer::RenderQuad(nullptr, instance->pipeline, instance->vertex_buffer1, instance->index_buffer);
			}
		);
	}

	void TestLayer::OnImGuiRenderer()
	{
		ImGui::SliderFloat("lod", &ubo.lod, 0, 10);

		auto [mov_speed,rot_speed] = m_editor_camera->GetCameraSpeed();
		ImGui::SliderFloat("editor camera move speed", &mov_speed, 0, 90);
		ImGui::SliderFloat("editor camera rot speed", &rot_speed, 0, 90);
		m_editor_camera->SetCameraSpeed({mov_speed, rot_speed});
		auto thre = m_editor_camera->Get3();
		ImGui::SliderFloat3("position", value_ptr(std::get<0>(thre)), 0, 10.0);
		ImGui::SliderFloat3("front", value_ptr(std::get<1>(thre)), 0, 10.0);
		ImGui::SliderFloat3("up", value_ptr(std::get<2>(thre)), 0, 10.0);
		m_editor_camera->Set3(thre);
	}

	void TestLayer::OnEvent(Event& event)
	{
		m_editor_camera->OnEvent(event);
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(TestLayer::OnWindowResize));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(TestLayer::OnKeyPressed));
	}


	bool TestLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case Key::C:
			//vertex_buffer->SetData(vertices1, sizeof(vertices1), 0);
			break;

		default:
			break;
		}
		return false;
	}

	bool TestLayer::OnWindowResize(WindowResizeEvent& e)
	{
		const uint32_t width = e.GetWidth(), height = e.GetHeight();

		if (width == 0 || height == 0)
		{
			return false;
		}
		m_editor_camera->SetViewportSize(width, height);
		return false;
	}
}
