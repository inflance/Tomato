#include "Editor.h"

#include <imgui.h>
#include <imgui_demo.cpp>
#include <ImGuizmo.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Tomato/Core/Math.h"
#include "Tomato/Core/File.h"
#include "Tomato/Input/Input.h"
#include "Tomato/Core/Timer.h"
#include "Tomato/Scene/ScriptableEntity.h"
#include "Tomato/Renderer/Mesh.h"
#include "Tomato/Renderer/Pipeline.h"
#include "Tomato/Renderer/RenderPass.h"
#include "Tomato/Renderer/ShaderFactory.h"

namespace Tomato
{
	Editor::Editor()
		: Layer("Editor"), m_cameraControler(1600.0f / 800.0f)
	{
	}

	void Editor::OnCreate()
	{
		m_texture = Texture2D::Create("PreCompile/Assets/Image/DefaultTexture.png");
		m_texture1 = Texture2D::Create("PreCompile/Assets/Image/tilemap_packed.png");
		m_subtexture = SubTexture2D::Create(m_texture1, { 0.0f, 0.0f }, { 73.0f, 73.0f }, { 1.0f, 0.71f });

	}

	void Editor::OnDestroy()
	{
	}

	void Editor::Tick(float deltaTime)
	{
		/*if (m_viewPortFocused)
			m_cameraControler.Tick(ts);*/

			//Renderer2D::ResetStats();
			//m_Scene->Tick(ts);
		m_frameBuffer->Bind();

		//RendererCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		//m_frameBuffer->ClearAttachment(1, -1);
		//m_editorCamera.Tick(deltaTime);
		//m_Scene->TickEditor(deltaTime, m_editorCamera);
		//m_Scene->Tick(ts);
		//Renderer::RenderQuad(m_command_buffer_, m_pipeline);
		//获取屏幕鼠标位置
		/*auto [mx, my] = ImGui::GetMousePos();
		mx -= m_viewportBounds[0].x;
		my -= m_viewportBounds[0].y;
		glm::vec2 viewportSize = m_viewportBounds[1] - m_viewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = mx;
		int mouseY = my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < static_cast<int>(viewportSize.x) && mouseY < static_cast<int>(
			viewportSize.y))
		{
			int pixelData = m_frameBuffer->ReadPixel(1, mouseX, mouseY);
			m_hoveredEntity = pixelData == -1 ? Entity() : Entity(static_cast<entt::entity>(pixelData), m_Scene.get());
		}

		m_frameBuffer->UnBind();*/
	}

	void Editor::OnImGuiRenderer()
	{
		bool open = true;

		static bool showViewPort = true;

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		//static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
//		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			//ImGui::SetNextWindowViewport(viewport->ID);
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
			//window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove;
			//window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			//dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handled the pass-thru hole, so we ask Begin() to not render a background.
		//if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		//window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		//ImGui::Begin("DockSpace Demo", &open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		//if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			//ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				/* ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				 ImGui::MenuItem("Padding", NULL, &opt_padding);*/

				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					CreateNewScene();
				}

				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					OpenScene();
				}

				if (ImGui::MenuItem("Save as", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

#if 0
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
#endif
				ImGui::Separator();

				if (ImGui::MenuItem("Close", nullptr, false, open != NULL))
					Engine::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
			ImGui::ShowDemoWindow(&open);
		}
		if (showViewPort)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 2.0f));
			ImGui::Begin("ViewPort");

			//set event block
			{
				m_viewPortFocused = ImGui::IsWindowFocused();
				m_viewPortHovered = ImGui::IsWindowHovered();

				if (!m_viewPortFocused && !m_viewPortHovered)
				{
					Engine::Get().GetImGuiLayer()->SetBlock(true);
				}
				else
				{
					Engine::Get().GetImGuiLayer()->SetBlock(false);
				}
			}

			//0, 20
			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();

			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();

			//获取当前窗口相对于屏幕左上角的位置
			auto viewportOffset = ImGui::GetWindowPos();
			m_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
			m_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
			//set window resize
			{
				ImVec2 viewPortSize = ImGui::GetContentRegionAvail();

				if (m_viewPortSize != *(glm::vec2*)&viewPortSize && viewPortSize.x > 0.0f && viewPortSize.y > 0.0f)
				{
					m_viewPortSize = { viewPortSize.x, viewPortSize.y };
					//LOG_WARN("{0}{1}", m_viewPortSize.x, m_viewPortSize.y);

					m_frameBuffer->Resize(m_viewPortSize.x, m_viewPortSize.y);
					m_Scene->SetViewPortResize(m_viewPortSize.x, m_viewPortSize.y);
					m_editorCamera.SetViewportSize(m_viewPortSize.x, m_viewPortSize.y);
				}
			}

			uint32_t id = m_frameBuffer->GetRendererID();

			ImGui::Image((void*)(id), ImVec2{ m_viewPortSize.x, m_viewPortSize.y }, ImVec2{ 0, 1 },
				ImVec2{ 1, 0 });

			{
				Entity SelectedEntity = m_ScenePanel.GetSelectedEntity();

				if (SelectedEntity && (m_zgmoMode != -1))
				{
					ImGuizmo::SetOrthographic(false);
					ImGuizmo::SetDrawlist();

					float width = ImGui::GetWindowWidth();
					float height = ImGui::GetWindowHeight();

					ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, width, height);

					/*auto& cameraEntity = m_Scene->GetMainCameraEntity();

					const auto& cc = cameraEntity.GetComponent<CameraComponent>().Camera;
					auto cameraProjection = cc.GetProjection();
					auto cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());*/

					//auto cameraProjection = m_editorCamera.GetProjection();
					auto cameraView = m_editorCamera.GetViewMatrix();

					auto& tc = SelectedEntity.GetComponent<TransformComponent>();
					auto transform = tc.GetTransform();

					/*Manipulate(value_ptr(cameraView), value_ptr(cameraProjection),
					static_cast<ImGuizmo::OPERATION>(m_zgmoMode), ImGuizmo::LOCAL,
						value_ptr(transform)); */

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

			ImGui::End();
			ImGui::PopStyleVar();
		}
		//ImGui::Image((ImTextureID)m_texture2->GetID(), {120,120 });
		ImGui::End();
		m_ScenePanel.OnImGuiRenderer();
		m_asset_panel.OnImGuiRenderer();
		ImGui::PopStyleVar(10);

		DeBugInfoPanel();
	}

	void Editor::OnEvent(Event& event)
	{
		m_editorCamera.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(Editor::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNC(Editor::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FUNC(Editor::OnMouseButtonReleased));
	}

	void Editor::CreateNewScene()
	{
		m_Scene = std::make_shared<Scene>();
		m_Scene->SetViewPortResize(m_viewPortSize.x, m_viewPortSize.y);
		m_ScenePanel.SetContex(m_Scene);
	}

	void Editor::OpenScene()
	{
		const std::string& filePath = File::Diolog::OpenFile("Tomato Scene (*.json)\0*.json\0");
		if (!filePath.empty())
		{
			auto newScene = std::make_shared<Scene>();

			SceneSerializater m_SceneSerializater(newScene);
			if (m_SceneSerializater.DeSerialization(filePath))
			{
				m_Scene = newScene;
				m_Scene->SetViewPortResize(m_viewPortSize.x, m_viewPortSize.y);
				m_ScenePanel.SetContex(m_Scene);
			}
			else
			{
				//LOG_ERROR("Failed To Open Scene");
			}
		}
	}

	void Editor::SaveSceneAs()
	{
		const std::string& filePath = File::Diolog::SaveFile("Tomato Scene (*.json)\0*.json\0");
		if (!filePath.empty())
		{
			SceneSerializater m_SceneSerializater(m_Scene);
			if (m_SceneSerializater.Serialization(filePath))
			{
				//LOG_INFO("Success To Save Scene");
			}
			else
			{
				//LOG_ERROR("Failed To Save Scene");
			}
		}
	}

	bool Editor::OnKeyPressed(KeyPressedEvent& e)
	{
		bool ctrl = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool alt = Input::IsKeyPressed(Key::LeftAlt) || Input::IsKeyPressed(Key::RightAlt);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
		case Key::N:
			if (ctrl)
			{
				CreateNewScene();
			}
			break;
		case Key::O:
			if (ctrl)
			{
				OpenScene();
			}
			break;
		case Key::S:
			if (shift && ctrl)
			{
				SaveSceneAs();
			}
			break;
		}

		if (m_editorMode == EditorMode::DefaultMode || m_editorMode == EditorMode::GizmoMode)
		{
			switch (e.GetKeyCode())
			{
			case Key::Q:
				m_zgmoMode = -1;
				m_editorMode = EditorMode::DefaultMode;
				break;

			case Key::W:
				m_zgmoMode = ImGuizmo::OPERATION::TRANSLATE;
				m_editorMode = EditorMode::GizmoMode;
				break;

			case Key::E:
				m_zgmoMode = ImGuizmo::OPERATION::ROTATE;
				m_editorMode = EditorMode::GizmoMode;
				break;

			case Key::R:
				m_zgmoMode = ImGuizmo::OPERATION::SCALE;
				m_editorMode = EditorMode::GizmoMode;
				break;
			}
		}
		return false;
	}

	bool Editor::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (m_viewPortHovered && !ImGuizmo::IsOver() && e.GetMouseButton() != Mouse::ButtonRight)
			m_ScenePanel.SetSelectedEntity(m_hoveredEntity);

		switch (e.GetMouseButton())
		{
		case Mouse::ButtonRight:
			m_editorMode = EditorMode::CameraMode;
			break;

		default:
			m_editorMode = EditorMode::DefaultMode;
			break;
		}
		return false;
	}

	void Editor::DeBugInfoPanel()
	{
		ImGui::Begin("setting");
		{
			const auto stats = Renderer2D::GetStats();
			ImGui::Text("DrawCalls:%d ", stats.DrawCalls);
			ImGui::Text("Quad: %d", stats.QuadCount);
			ImGui::Text("TotalIndex: %d", stats.GetTotalIndexCount());
			ImGui::Text("TotalVertex: %d", stats.GetTotalVertexCount());

			ImGui::Text("fps: %u", Engine::Get().GetFPS());
		}
		ImGui::End();
	}

	bool Editor::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		switch (e.GetMouseButton())
		{
		default:
			m_editorMode = EditorMode::DefaultMode;
			break;
		}
		return false;
	}
}