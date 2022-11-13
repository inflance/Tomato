#include "ScenePanel.h"
#include "Tomato/Scene/Components.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include "Tomato/Renderer/Texture.h"
#include "Tomato/Platform/OpenGL/OpenGLTexture.h"


namespace Tomato {

	ScenePanel::ScenePanel(const std::shared_ptr<Scene>& context)
	{
		SetContex(context);
	}

	void ScenePanel::OnImGuiRenderer()
	{
		ImGui::Begin("Scene Panel");
		bool open = true;
		m_Context->m_Registry.each(
			[=](auto entity)
			{
				GameObject GO = { entity, m_Context.get() };
				
				DrawScenePanel(GO);
			
			}
		);
		
		if (ImGui::BeginPopupContextWindow(0, 1, true))
		{
			if (ImGui::MenuItem("Create Empty GameObject"))
				m_Context->CreateGameObject("Empty GameObject");

			if (ImGui::MenuItem("Delete GameObject") && m_SelectedGO)
			{
				m_Context->DestroyEntity(m_SelectedGO);
				m_SelectedGO.Clear();
			}

			ImGui::EndPopup();
		}


		ImGui::End();

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedGO.Clear();

		ImGui::Begin("Detail");
		if (m_SelectedGO)
		{
			DrewDetailPanel(m_SelectedGO);
		}
		ImGui::End();


	}

	template<typename T, typename Func>
	void Tomato::ScenePanel::DrawComponents(const std::string& name, GameObject GO, Func uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (GO.HasComponent<T>())
		{
			auto context = ImGui::GetCurrentContext();
			auto& component = GO.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = 20.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.7f);

			uint32_t id = m_moreBtn.get()->GetID();
			if (ImGui::ImageButton((void*)id, ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				GO.RemoveComponent<T>();
		}
	}


	void ScenePanel::DrawScenePanel(GameObject GO)
	{
		ImGuiTreeNodeFlags flags = ((GO == m_SelectedGO) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		auto& name = m_Context->m_Registry.get<NameComponent>(GO).Name;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)GO, flags, name.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectedGO = GO;
		}
		if (opened)
		{
			ImGui::TreePop();
			auto& name = m_Context->m_Registry.get<NameComponent>(GO).Name;
			ImGui::Text("%s", name.c_str());
		}

	}

	void ScenePanel::DrewDetailPanel(GameObject GO)
	{
		if (GO.HasComponent<NameComponent>())
		{
			auto& name = m_Context->m_Registry.get<NameComponent>(GO).Name;
			

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), name.c_str());

			if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
			{
				name = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				if (!m_SelectedGO.HasComponent<CameraComponent>())
					m_SelectedGO.AddComponent<CameraComponent>();
				else
					LOG_WARN("This entity already has the Camera Component!");
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Renderer"))
			{
				if (!m_SelectedGO.HasComponent<ColorComponent>())
					m_SelectedGO.AddComponent<ColorComponent>();
				else
					LOG_WARN("This entity already has the Sprite Renderer Component!");
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		DrawComponents<TransformComponent>("Transform", GO,
			[&](auto& component){
				auto& position = component.Position;
				auto& scale = component.Scale;
				DrawVector3("Position", position, 0.1f, dPosition);

				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVector3("Rotation", rotation, 1.0f, dRotation);
				component.Rotation = glm::radians(rotation);

				DrawVector3("Scale", scale, 0.1f, dScale);
			});

		DrawComponents<ColorComponent>("Color", GO,
			[](auto& component) {
				auto& color = component.Color;

				ImGui::ColorEdit4("Color", glm::value_ptr(color));
			});

		DrawComponents<CameraComponent>("Camera", GO,
			[](auto& component)
			{
				auto& cameraComponent = component;

				auto& camera = cameraComponent.Camera;

				const char* cameraTypeString[] = { "Orthographic", "Perspective" };

				const char* currCameraType = cameraTypeString[(int)camera.GetSceneCameraType()];

				ImGui::Checkbox("IsMainCamera", &cameraComponent.IsMain);

				if (ImGui::BeginCombo("Projection", currCameraType))
				{
					for (int i = 0; i < 2; i++)
					{
						bool selected = currCameraType == cameraTypeString[i];
						if (ImGui::Selectable(cameraTypeString[i], selected))
						{
							currCameraType = cameraTypeString[i];
							camera.SetSceneCameraType((SceneCameraType)i);
						}
						if (selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (camera.GetSceneCameraType() == SceneCameraType::Orthographic)
				{
					float zoomLevel = camera.GetOrthoZoomLevel();
					if (ImGui::DragFloat("ZoomLevel", &zoomLevel, 0.1f))
						camera.SetOrthoZoomLevel(zoomLevel);

					float orthoNear = camera.GetOrthoNear();
					if (ImGui::DragFloat("Near", &orthoNear, 0.1f))
						camera.SetOrthoNear(orthoNear);

					float orthoFar = camera.GetOrthoFar();
					if (ImGui::DragFloat("Far", &orthoFar, 0.1f))
						camera.SetOrthoFar(orthoFar);
				}
				if (camera.GetSceneCameraType() == SceneCameraType::Perspective)
				{
					float FOV = glm::degrees(camera.GetPerspFOV());
					if (ImGui::DragFloat("FOV", &FOV, 0.1f))
						camera.SetPerspFOV(glm::radians(FOV));

					float perspNear = camera.GetPerspNear();
					if (ImGui::DragFloat("Near", &perspNear, 0.1f))
						camera.SetPerspNear(perspNear);

					float perspFar = camera.GetPerspFar();
					if (ImGui::DragFloat("Far", &perspFar, 0.1f))
						camera.SetPerspFar(perspFar);
				}
			});
	}

	void ScenePanel::SetContex(const std::shared_ptr<Scene>& context)
	{
		m_moreBtn = Texture2D::Create("C:/Users/liyunlo2000/source/repos/Tomato/Tomato/Precompile/ImGuiImage/more.png");
		m_Context = context;
		m_SelectedGO .Clear();
	}

	void ScenePanel::DrawVector3(const std::string& label, glm::vec3& values, float speed, const glm::vec3& defaltValue)
	{
		
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 70.0f);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = defaltValue.x;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.7f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{  0.2f, 0.8f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.7f, 0.15f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = defaltValue.y;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.5f, 0.85f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{  0.2f, 0.6f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.5f, 0.85f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = defaltValue.z;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::Columns(1);

		ImGui::PopID();

	}

}


