#include "LevelPanel.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "glm/gtc/type_ptr.inl"
#include "Tomato/ECS/Components.h"
#include "Tomato/ECS/World/Level.hpp"
#include "ranges"
#include <math.h>

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
			DrawMenu<LightComponent>("Light", m_selected_entity);
			DrawMenu<SpriteComponent>("Sprite", m_selected_entity);
			DrawMenu<CameraComponent>("Camera", m_selected_entity);
			DrawMenu<RigidBody2DComponent>("Rigid Body 2D", m_selected_entity);
			DrawMenu<BoxCollider2DComponent>("Box Collider 2D", m_selected_entity);
			ImGui::EndPopup();
		}

		DrawComponents<TransformComponent>
		("Transform", entity, [&](TransformComponent& component)
		 {
			 auto& position = component.position_;
			 auto& scale = component.scale_;
			 ImGui::DragFloat3("Position", value_ptr(position), 0.1f);

			 glm::vec3 rotation = degrees(component.rotation_);
			 ImGui::DragFloat3("Rotation", value_ptr(rotation), 1.0f);
			 component.rotation_ = radians(rotation);

			 ImGui::DragFloat3("Scale", value_ptr(scale), 0.1f);
		 });

		DrawComponents<LightComponent>
		("Light", entity, [&](LightComponent& component)
		{
			auto& light = component.light_;
			auto cur_type_string = light_type_to_str[light.light_type_];
			if (ImGui::BeginCombo("Light Type", cur_type_string.c_str()))
			{
				for (auto type : light_type_to_str | std::views::keys)
				{
					const bool is_selected = cur_type_string == light_type_to_str[type];
					if (ImGui::Selectable(light_type_to_str[type].c_str(), is_selected))
					{
						cur_type_string = light_type_to_str[type];
						light.light_type_ = type;
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (light.light_type_ == LightType::DirectionLight)
			{
				auto& color = light.color_;
				ImGui::ColorEdit3("Color", value_ptr(color));

				auto& position = light.position;
				ImGui::DragFloat3("Position", value_ptr(position), 1.0f);

				auto& direction = light.direction_;
				ImGui::DragFloat3("Direction", value_ptr(direction), 1.0f);

				auto intensity = light.intensity_;
				ImGui::DragFloat("Intensity", &intensity, 0.1);
				light.intensity_ = intensity;
			}
			else if (light.light_type_ == LightType::PointLight)
			{
				auto& color = light.color_;
				ImGui::ColorEdit3("Color", value_ptr(color));

				auto& position = light.position;
				ImGui::DragFloat3("Position", value_ptr(position), 1.0f);

				auto& direction = light.direction_;
				ImGui::DragFloat3("Direction", value_ptr(direction), 1.0f);

				auto intensity = light.intensity_;
				ImGui::DragFloat("Intensity", &intensity);
				light.intensity_ = intensity;

				auto constant = light.constant_;
				ImGui::DragFloat("Constant", &constant);
				light.constant_ = constant;

				auto linear = light.linear_;
				ImGui::DragFloat("Linear", &linear);
				light.linear_ = linear;

				auto quadratic = light.quadratic_;
				ImGui::DragFloat("Quadratic", &quadratic);
				light.quadratic_ = quadratic;
			}
			else
			{
				auto& color = light.color_;
				ImGui::ColorEdit3("Color", value_ptr(color));

				auto& position = light.position;
				ImGui::DragFloat3("Position", value_ptr(position), 1.0f);

				auto& direction = light.direction_;
				ImGui::DragFloat3("Direction", value_ptr(direction), 1.0f);

				auto intensity = light.intensity_;
				ImGui::DragFloat("Intensity", &intensity);
				light.intensity_ = intensity;

				auto constant = light.constant_;
				ImGui::DragFloat("Constant", &constant);
				light.constant_ = constant;

				auto linear = light.linear_;
				ImGui::DragFloat("Linear", &linear);
				light.linear_ = linear;

				auto quadratic = light.quadratic_;
				ImGui::DragFloat("Quadratic", &quadratic);
				light.quadratic_ = quadratic;

				auto cut_off = glm::acos(light.cut_off) * 180 / glm::pi<float>();
				ImGui::DragFloat("CutOff", &cut_off, 1.0f, 0);
				light.cut_off = glm::cos(glm::radians(cut_off));

				auto outer_cut_off = glm::acos(light.outer_cut_off) * 180 / glm::pi<float>();
				ImGui::DragFloat("OuterCutOff", &outer_cut_off, 1.0f, 0);
				light.outer_cut_off = glm::cos(glm::radians(outer_cut_off));
			}
		});

		DrawComponents<CameraComponent>
		("Camera", entity, [](CameraComponent& component)
		{
			auto& camera = component.camera_;

			ImGui::Checkbox("Is Main", &component.is_main_camera_);

			auto& type_string = camera_type_to_str[camera.
				GetCameraType()];
			if (ImGui::BeginCombo("Camera Type", type_string.c_str()))
			{
				for (auto type : camera_type_to_str | std::views::keys)
				{
					const bool is_selected = type_string == camera_type_to_str[type];
					if (ImGui::Selectable(camera_type_to_str[type].c_str(), is_selected))
					{
						type_string = camera_type_to_str[type];
						camera.SetCameraType(type);
					}

					if (is_selected)
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

		DrawComponents<RigidBody2DComponent>
		("Rigid Body 2D", entity, [](RigidBody2DComponent& component)
		{
			auto type_string = body_type_to_str[component.type_];
			if (ImGui::BeginCombo("Body Type", type_string.c_str()))
			{
				for (auto type : body_type_to_str | std::views::keys)
				{
					const bool is_selected = type_string == body_type_to_str[type];
					if (ImGui::Selectable(body_type_to_str[type].c_str(), is_selected))
					{
						type_string = body_type_to_str[type];
						component.type_ = type;
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fixed Rotation", &component.fixed_rotation_);
		});

		DrawComponents<BoxCollider2DComponent>
		("Box Collider 2D", entity, [](BoxCollider2DComponent& component)
		{
			ImGui::DragFloat2("Offset", value_ptr(component.offset_));
			ImGui::DragFloat2("Size", value_ptr(component.size_));
			ImGui::DragFloat("Density", &component.density_, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.friction_, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.restitution_, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.restitution_threshold_, 0.01f, 0.0f);
		});

		DrawComponents<SpriteComponent>
		("Sprite", entity, [&](SpriteComponent& component)
		{
			auto cur_geom_type = geometry_type_to_str[component.geometry_type_];
			if (ImGui::BeginCombo("Body Type", cur_geom_type.c_str()))
			{
				for (auto type : geometry_type_to_str | std::views::keys)
				{
					const bool is_selected = cur_geom_type == geometry_type_to_str[type];
					if (ImGui::Selectable(geometry_type_to_str[type].c_str(), is_selected))
		 {
						cur_geom_type = geometry_type_to_str[type];
						component.geometry_type_ = type;
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
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

	void LevelPanel::DrawVector3(const std::string& label, glm::vec3& values, float speed,
	                             const glm::vec3& default_value) const
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 90.0f);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, (ImGui::GetColumnWidth() - 50.0f) / 2 * 1.3);

		const float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		const ImVec2 size = {line_height - 3, line_height - 3};

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{6, 6});
		if (ImGui::Button("X", size))
			values.x = default_value.x;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.7f, 0.15f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.8f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.7f, 0.15f, 1.0f});
		if (ImGui::Button("Y", size))
			values.y = default_value.y;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.5f, 0.85f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.6f, 0.9f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.5f, 0.85f, 1.0f});
		if (ImGui::Button("Z", size))
			values.z = default_value.z;
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::Columns(1);
		ImGui::PopStyleVar();

		ImGui::PopID();
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
		const auto& name = m_level_context->GetLevelRegistry()->Get().get<TagComponent>(entity).tag_;

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
	void LevelPanel::DrawMenu(const std::string& name, Entity entity) const
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
