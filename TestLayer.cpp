#include "TestLayer.h"

#include "Tomato/Tomato.h"
#include "Tomato/Renderer/RenderPass.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"
#include "ImGuizmo.h"
#include "glm/gtc/type_ptr.hpp"
#include "vulkan/vulkan.hpp"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include "Editor/LevelPanel.hpp"
#include "Tomato/Manager/AssetManager.hpp"
#include "Tomato/Renderer/FrameBuffer.hpp"
#include "Tomato/Function/Camera/EditorCamera.hpp"
#include "Tomato/Function/Camera/Camera.hpp"
#include "Tomato/Function/Controller/CameraControler.hpp"
#include "Tomato/Renderer/IndexBuffer.hpp"
#include "Tomato/Renderer/Model.hpp"
#include "Tomato/Renderer/Texture.hpp"
#include "Tomato/ECS/World/Level.hpp"
#include "Editor/ViewPortPanel.hpp"

namespace Tomato
{
	struct VertexData
	{
		glm::vec3 Position{};
		glm::vec3 Color{};
		glm::vec2 TexCoord{};
	};

	inline void* GetIDFormTexture(const Ref<Texture2D>& texture)
	{
		auto info = (vk::DescriptorImageInfo*)texture->GetDescriptorInfo();
		return ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView,
		                                   static_cast<VkImageLayout>(info->imageLayout));
	}

	void TestLayer::OnCreate()
	{
		//ubo.model = translate(glm::mat4(1.0f), {0.0, 0.0, 1.0}) * rotate(
		//		glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
		//	rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

#if 0
		m_camera = std::make_shared<Camera>();

		m_controller = CameraController(m_camera);
		m_controller.SetCameraTickFnc([](Ref<Camera>& camera, float deltaTime)
		{
			//LOG_INFO("{} {}", static_cast<int>(camera->GetCameraType()), deltaTime);
		});
#endif

		m_level = std::make_shared<Level>("default");
		auto entity = m_level->CreateDefaultEntity("hello");
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<SpriteComponent>();

		m_level_panel = std::make_shared<LevelPanel>(m_level);
		m_editor_camera = std::make_shared<EditorCamera>(glm::radians(45.0f), 800 / 800.0f, 0.001f, 1000.0f);
		m_editor_camera->SetCameraType(CameraType::Perspective);

		Renderer2D::Init();

		m_view_port_panel = ViewPortPanel::Create(GetIDFormTexture(
			                                          Renderer2D::GetUniformBuffer()->GetFrameBufferInfo().render_pass->
			                                          GetProps().attachments_[0]), 500, 500);
		auto ins = this;

		m_view_port_panel->SetEventTick([ins](float width, float height)
		{
			ins->m_editor_camera->SetViewportSize(width, height);
			ins->m_level->OnViewPortResize(width, height);
		});

		m_level->OnViewPortResize(Engine::Get().GetWindow().GetWidth(), Engine::Get().GetWindow().GetHeight());
	}

	void TestLayer::OnDestroy()
	{
	}

	void TestLayer::Tick(float delta_time)
	{
		if (m_editor_mode == EditorType::EditorMode)
		{
			m_editor_camera->Tick(delta_time);

			m_level->TickOnEditor(m_editor_camera, delta_time);
		}
		else if (m_editor_mode == EditorType::RuntimeMode)
		{
			m_level->LogicTick(delta_time);
			m_level->Tick(delta_time);
		}
	}

	void TestLayer::OnImGuiRenderer()
	{
		bool open = true;

		static bool showViewPort = true;

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 9.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 6.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.5f, 5.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6.5f, 12.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 6.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 7.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.00f, 0.5f));
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		// Begin Dock Space
		ImGui::Begin("DockSpace Demo", &open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", nullptr, &opt_fullscreen);
				ImGui::MenuItem("Padding", nullptr, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
				{
					dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
				}
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
				{
					dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
				}
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "",
				                    (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
				{
					dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
				}
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "",
				                    (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
				{
					dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
				}
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "",
				                    (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen))
				{
					dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
				}
				ImGui::Separator();

				if (ImGui::MenuItem("Close", nullptr, false, &open != nullptr))
					open = false;
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::ShowDemoWindow(&open);

		{
			if (!is_resize)
			{
				m_view_port_panel->Begin();
				static bool mode = true;
				ImGui::Checkbox("Editor", &mode);
				if (mode)
				{
					if (m_editor_mode != EditorType::EditorMode) m_level->OnRuntimeLevelEnd();
					m_editor_mode = EditorType::EditorMode;
				}
				else
				{
					if (m_editor_mode != EditorType::RuntimeMode) m_level->OnRuntimeLevelStart();
					m_editor_mode = EditorType::RuntimeMode;
				}
				m_view_port_panel->Tick();

				{
					Entity selected_entity = m_level_panel->GetSelectedEntity();

					if (selected_entity && m_editor_mode == EditorType::EditorMode)
					{
						ImGuizmo::SetOrthographic(
							m_editor_camera->GetCamera().GetCameraType() == CameraType::Orthographic);

						float width = ImGui::GetWindowWidth();
						float height = ImGui::GetWindowHeight();

						ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, width, height);

						auto& camera_view = m_editor_camera->GetViewMatrix();
						auto camera_projection = m_editor_camera->GetProjectionMatrix();
						camera_projection[1][1] *= -1.0f;
						auto& tc = selected_entity.GetComponent<TransformComponent>();
						auto transform = tc.GetTransform();
						ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
						Manipulate(value_ptr(camera_view), value_ptr(camera_projection),
						           ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::WORLD,
						           value_ptr(transform));

						if (ImGuizmo::IsUsing())
						{
							glm::vec3 position, rotation, scale;

							Math::DecomposeTransform(transform, position, rotation, scale);

							glm::vec3 deltaRotation = rotation - tc.rotation_;

							tc.position_ = position;
							tc.rotation_ += deltaRotation;
							tc.scale_ = scale;
						}
					}
				}
				m_view_port_panel->End();
			}

			m_level_panel->Tick();

			ImGui::Begin("Test");
			{
				//ImGui::SliderFloat("lod", &ubo.lod, 0, 10);
				auto& camera = m_editor_camera->GetCamera();
				auto type = camera.GetCameraType();
				{
					static const char* camera_str[] = {"Orthographic", "Perspective"};

					const char* currCameraType = camera_str[static_cast<int>(camera.
						GetCameraType())];

					if (ImGui::BeginCombo("Projection", currCameraType))
					{
						for (int i = 0; i < 2; i++)
						{
							bool selected = currCameraType == camera_str[i];
							if (ImGui::Selectable(camera_str[i], selected))
							{
								currCameraType = camera_str[i];
								m_editor_camera->UpdateCameraType(static_cast<CameraType>(i));
							}
							if (selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
				}

				auto [mov_speed, rot_speed] = m_editor_camera->GetCameraSpeed();
				ImGui::SliderFloat("editor camera move speed", &mov_speed, 0, 90);
				ImGui::SliderFloat("editor camera rot speed", &rot_speed, 0, 90);
				m_editor_camera->SetCameraSpeed({mov_speed, rot_speed});
				auto thre = m_editor_camera->Get3();
				ImGui::DragFloat3("position", value_ptr(std::get<0>(thre)), 1);
				ImGui::DragFloat3("front", value_ptr(std::get<1>(thre)), 1);
				ImGui::DragFloat3("up", value_ptr(std::get<2>(thre)), 1);
				m_editor_camera->Set3(thre);
			}
			ImGui::End();
		}

		if (opt_fullscreen)
		{
			ImGui::PopStyleVar(10);
		}
		ImGui::End(); // End Dock Space
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
		is_resize = true;

		Renderer2D::GetUniformBuffer()->OnResize(width, height);

		m_view_port_panel->UpdateImage(GetIDFormTexture(
			Renderer2D::GetUniformBuffer()->GetFrameBufferInfo().render_pass->
			                                GetProps().attachments_[0]));
		is_resize = false;
		return false;
	}
}
