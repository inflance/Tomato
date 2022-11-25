#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "ScenePanel.h"
#include "Tomato/Scene/Components.h"
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

		//Scene Name
		{
			auto& name = m_context->m_sceneName;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), name.c_str());

			if (ImGui::InputText("##Scene Name", buffer, sizeof(buffer)))
			{
				name = std::string(buffer);
			}
		}
		
		m_context->m_Registry.each(
			[=](auto entity)
			{
				Entity Entity = { entity, m_context.get() };
				
				DrawScenePanel(Entity);
			
			}
		);
		
		//
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 8.0f);
		if (ImGui::BeginPopupContextWindow(0, 1, true))
		{
			if (!m_selectedEntity) {
				if (ImGui::MenuItem("Create Empty Entity"))
				{
					m_context->CreateEntity("Empty Entity");

				}
			}
			
			if (m_selectedEntity)
			{
				if (ImGui::MenuItem("Create Empty Entity"))
				{
					m_context->CreateEntity("Empty Entity");

				}
				if (ImGui::MenuItem("Delete Entity")) {
					m_context->DestroyEntity(m_selectedEntity);
					m_selectedEntity = {};
				}
			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();

		ImGui::End();

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_selectedEntity = {};

		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(40, 40));
		ImGui::Begin("Detail");
		if (m_selectedEntity)
		{
			DrewDetailPanel(m_selectedEntity);
		}
		ImGui::End();
		//ImGui::PopStyleVar();
		


	}

	template<typename T, typename Func>
	void Tomato::ScenePanel::DrawComponents(const std::string& name, Entity Entity, Func uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (Entity.HasComponent<T>())
		{
			auto context = ImGui::GetCurrentContext();
			auto& component = Entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 20, 8 });
			float lineHeight = 20.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.6f);

			uint32_t id = m_moreBtn.get()->GetID();
			if (ImGui::ImageButton((void*)id, ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				Entity.RemoveComponent<T>();
		}
	}


	void ScenePanel::DrawScenePanel(Entity Entity)
	{
		ImGuiTreeNodeFlags flags = ((Entity == m_selectedEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		
		auto& name = m_context->m_Registry.get<NameComponent>(Entity).Name;
		bool selected = Entity == m_selectedEntity;
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize,5.0f);
		ImGui::Selectable(name.c_str(), &selected, flags, { ImGui::GetContentRegionAvail().x, 28.0f});
		ImGui::PopStyleVar();
		if (ImGui::IsItemClicked())
		{
			m_selectedEntity = Entity;
			
		}
	}

	void ScenePanel::DrewDetailPanel(Entity Entity)
	{
		if (Entity.HasComponent<NameComponent>())
		{
			auto& name = m_context->m_Registry.get<NameComponent>(Entity).Name;
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

		if (ImGui::Button("+ Add Component"))
			ImGui::OpenPopup("+ AddComponent");

		if (ImGui::BeginPopup("+ AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				if (!m_selectedEntity.HasComponent<CameraComponent>())
					m_selectedEntity.AddComponent<CameraComponent>();
				else
					LOG_WARN("This entity already has the Camera Component!");
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite"))
			{
				if (!m_selectedEntity.HasComponent<SpriteComponent>())
					m_selectedEntity.AddComponent<SpriteComponent>();
				else
					LOG_WARN("This entity already has the Sprite Component!");
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		DrawComponents<TransformComponent>("Transform", Entity,
			[&](auto& component){
				auto& position = component.Position;
				auto& scale = component.Scale;
				DrawVector3("Position", position, 0.1f, dPosition);

				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVector3("Rotation", rotation, 1.0f, dRotation);
				component.Rotation = glm::radians(rotation);

				DrawVector3("Scale", scale, 0.1f, dScale);
			});

		DrawComponents<SpriteComponent>("Color", Entity,
			[](auto& component) {
				auto& color = component.Color;

				ImGui::ColorEdit4("Color", glm::value_ptr(color));
			});

		DrawComponents<CameraComponent>("Camera", Entity,
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
		m_moreBtn = Texture2D::Create("C:/Users/liyun/source/repos/Tomato/Tomato/Precompile/ImGuiImage/more.png");
		m_context = context;
		m_selectedEntity.Clear();
		m_selectedEntity = {};
	}

	void ScenePanel::DrawVector3(const std::string& label, glm::vec3& values, float speed, const glm::vec3& defaltValue)
	{
		
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0,90.0f);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::GetColumnWidth()/2);

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight-3 , lineHeight-3 };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 6, 6 });
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
		ImGui::PopStyleVar();

		ImGui::PopID();

	}

}


