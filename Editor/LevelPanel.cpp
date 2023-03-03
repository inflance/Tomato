#include "LevelPanel.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "glm/gtc/type_ptr.inl"
#include "Tomato/ECS/Components.h"
#include "Tomato/ECS/World/Level.hpp"

namespace Tomato
{
	LevelPanel::LevelPanel(const std::shared_ptr<Level>& context)
		: m_level_context(context)
	{
	}

	LevelPanel::~LevelPanel()
	{
	}

	void LevelPanel::DrewDetailPanel(const Entity& entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			std::string& name = m_level_context->GetLevelRegistry()->Get().get<TagComponent>(entity).tag_;
			char buffer[256]{};
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
			DrawMenu<TransformComponent>("Transform", m_selected_entity);
			DrawMenu<SpriteComponent>("Sprite", m_selected_entity);
			DrawMenu<CameraComponent>("Camera", m_selected_entity);
			DrawMenu<RigidBody2DComponent>("Rigid Body 2D", m_selected_entity);
			DrawMenu<BoxCollider2DComponent>("Box Collider 2D", m_selected_entity);
			ImGui::EndPopup();
		}

		DrawComponents<TransformComponent>
		("Transform", entity,
		 [&](TransformComponent& component)
		 {
			 auto& position = component.position_;
			 auto& scale = component.scale_;
			 ImGui::DragFloat3("Position", value_ptr(position), 0.1f);

			 glm::vec3 rotation = degrees(component.rotation_);
			 ImGui::DragFloat3("Rotation", value_ptr(rotation), 1.0f);
			 component.rotation_ = radians(rotation);

			 ImGui::DragFloat3("Scale", value_ptr(scale), 0.1f);
		 });
		DrawComponents<CameraComponent>("Camera", entity, [](CameraComponent& component)
		{
			auto& camera = component.camera_;

			ImGui::Checkbox("Main", &component.is_main_camera_);

			const char* projectionTypeStrings[] = {"Orthographic", "Perspective",};
			const char* currentProjectionTypeString = projectionTypeStrings[static_cast<int>(camera.GetCameraType())];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetCameraType(static_cast<CameraType>(i));
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetCameraType() == CameraType::Perspective)
			{
				float fov = glm::degrees(camera.GetFov());
				if (ImGui::DragFloat("Vertical FOV", &fov))
					camera.SetFov(glm::radians(fov));

				float near = camera.GetNear();
				if (ImGui::DragFloat("Near", &near))
					camera.SetNear(near);

				float far = camera.GetFar();
				if (ImGui::DragFloat("Far", &far))
					camera.SetFar(far);
				camera.UpdateProjectionMatrix();
			}
			if (camera.GetCameraType() == CameraType::Orthographic)
			{
				float zoom_level = camera.GetZoomLevel();
				if (ImGui::DragFloat("Size", &zoom_level))
					camera.SetZoomLevel(zoom_level);

				float near = camera.GetNear();
				if (ImGui::DragFloat("Near", &near))
					camera.SetNear(near);

				float far = camera.GetFar();
				if (ImGui::DragFloat("Far", &far))
					camera.SetFar(far);
				camera.UpdateProjectionMatrix();
			}
		});

		DrawComponents<RigidBody2DComponent>("Rigidbody 2D", entity, [](RigidBody2DComponent& component)
		{
			const char* bodyTypeStrings[] = {"Static", "Dynamic", "Kinematic"};
			const char* currentBodyTypeString = bodyTypeStrings[static_cast<int>(component.type_)];
			if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
					if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
					{
						currentBodyTypeString = bodyTypeStrings[i];
						component.type_ = static_cast<RigidBody2DComponent::BodyType>(i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fixed Rotation", &component.fixed_rotation_);
		});

		DrawComponents<BoxCollider2DComponent>("Box Collider 2D", entity, [](BoxCollider2DComponent& component)
		{
			ImGui::DragFloat2("Offset", value_ptr(component.offset_));
			ImGui::DragFloat2("Size", value_ptr(component.size_));
			ImGui::DragFloat("Density", &component.density_, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.friction_, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.restitution_, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.restitution_threshold_, 0.01f, 0.0f);
		});

		DrawComponents<SpriteComponent>
		("Sprite", entity,
		 [&](SpriteComponent& component)
		 {
			 auto& color = component.color_;
			 ImGui::Columns(2);
			 ImGui::SetColumnWidth(0, 120.0f);
			 ImGui::Text("Color");

			 ImGui::NextColumn();
			 ImGui::PushMultiItemsWidths(1, ImGui::GetContentRegionAvail().x);
			 ImGui::ColorEdit4("##Color", value_ptr(color));
			 ImGui::PopItemWidth();

			 ImGui::NextColumn();
			 ImGui::Text("Texture");

			 ImGui::NextColumn();
			 ImGui::PushMultiItemsWidths(1, ImGui::GetContentRegionAvail().x);
			 //if (component.texture_)
			 //{
			 // ImGui::ImageButton((void*)(component.texture_->GetID()),
			 //	 { 80, 80 }, { 0, 1 }, { 1, 0 });
			 //}
			 //else
			 //{
			 // //TODO add default texture here
			 //}

			 ImGui::PopItemWidth();

			 /*	if (ImGui::BeginDragDropTarget())
				 {
					 if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(
						 "Assets_Panel"))
					 {
						 auto path = static_cast<const wchar_t*>(payload->Data);
						 std::filesystem::path texturePath = std::filesystem::path(
							 g_asset_path) / path;
						 Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
						 component.texture_ = texture;
					 }
					 ImGui::EndDragDropTarget();
				 }*/
			 ImGui::NextColumn();
			 ImGui::Text("Tiling Factor");
			 ImGui::NextColumn();
			 ImGui::PushMultiItemsWidths(1, ImGui::GetContentRegionAvail().x);
			 ImGui::DragFloat("##Tiling Factor", &component.tiling_factor_, 0.1f);
			 ImGui::PopItemWidth();
		 });
	}

	void LevelPanel::Tick()
	{
		if (m_is_open)
		{
			ImGui::Begin("Level Panel");

			{
				auto& name = m_level_context->GetLevelName();
				char buffer[256]{};
				strcpy_s(buffer, sizeof(buffer), name.c_str());

				if (ImGui::InputText("##Scene Name", buffer, sizeof(buffer)))
				{
					m_level_context->SetLevelName(std::string(buffer));
				}
			}

			{
				auto ins = this;
				m_level_context->GetLevelRegistry()->Get().each(
					[ins](auto entity)
					{
						ins->DrawLevelPanel(Entity(entity, ins->m_level_context->GetLevelRegistry().get()));
					}
				);
			}

			{
				ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 8.0f);
				ImGuiPopupFlags flags = ImGuiPopupFlags_MouseButtonRight;
				if (ImGui::BeginPopupContextWindow("Enabled Entity##", flags))
				{
					if (ImGui::MenuItem("Create Empty Entity"))
					{
						m_level_context->CreateDefaultEntity("Empty Entity");
					}
					ImGui::EndPopup();
				}
				ImGui::PopStyleVar();
			}

			{
				ImGui::Begin("Detail");
				if (m_selected_entity)
				{
					DrewDetailPanel(m_selected_entity);
				}
				ImGui::End();
			}

			ImGui::End();
		}
	}

	void LevelPanel::DrawLevelPanel(Entity entity)
	{
		auto flag = m_selected_entity == entity;
		ImGuiTreeNodeFlags flags = (flag ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None) |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed;
		auto& name = m_level_context->GetLevelRegistry()->Get().get<TagComponent>(entity).tag_;

		if (ImGui::TreeNodeEx((void*)(static_cast<intptr_t>(static_cast<uint32_t>(entity))), flags,
		                      name.c_str()))
		{
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				m_selected_entity = entity;
				LOG_INFO("m_selected_entity {}", static_cast<uint32_t>(entity));
			}
			ImGui::TreePop();
		}
	}

	template <typename TComponent>
	void LevelPanel::DrawMenu(const std::string& name, Entity entity)
	{
		if (!entity.HasComponent<TComponent>())
		{
			if (ImGui::MenuItem(name.c_str()))
			{
				entity.AddComponent<TComponent>();
				ImGui::CloseCurrentPopup();
			}
		}
	}


	template <typename TComponent, typename Func>
	void LevelPanel::DrawComponents(const std::string& name, Entity Entity, Func uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (Entity.HasComponent<TComponent>())
		{
			auto context = ImGui::GetCurrentContext();
			auto& component = Entity.GetComponent<TComponent>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{20, 6});
			float lineHeight = 18.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(TComponent).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight);

			if (ImGui::Button("Setting", ImVec2{lineHeight, lineHeight}))
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
				Entity.RemoveComponent<TComponent>();
		}
	}
}
