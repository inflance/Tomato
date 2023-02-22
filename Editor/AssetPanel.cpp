#include "AssetPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Tomato/Core/File.h"
#include "Tomato/Scene/Scene.h"

namespace Tomato {

	const std::filesystem::path g_asset_path = "PreCompile/Assets";

	AssetPanel::AssetPanel()
		:m_cur_asset_path(g_asset_path)
	{
		m_dir_icon = Texture2D::Create("PreCompile/Assets/Icon/Directory.png");
		m_file_icon = Texture2D::Create("PreCompile/Assets/Icon/File.png");
	}

	void AssetPanel::OnImGuiRenderer()
	{
		ImGui::Begin("Assets");

		if (m_cur_asset_path != std::filesystem::path(g_asset_path))
		{
			if (ImGui::Button("<-"))
			{
				m_cur_asset_path = m_cur_asset_path.parent_path();
				m_cur_selected_dir = std::filesystem::directory_entry();
			}		
		}
		ImGui::SameLine();
		if (ImGui::Button("->") && m_cur_selected_dir.is_directory())
		{
			m_cur_asset_path /= m_cur_selected_dir.path().filename();
			m_cur_selected_dir = std::filesystem::directory_entry();
		}

		float cellSize = m_icon_padding + m_icon_size;

		float panelWidth = ImGui::GetContentRegionAvail().x;

		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_cur_asset_path))
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());
			std::shared_ptr<Texture2D> icon = directoryEntry.is_directory() ? m_dir_icon : m_file_icon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			bool selected = m_cur_selected_dir == directoryEntry;

			file_icon(filenameString.c_str(), selected, (ImTextureID)icon->GetID(), { m_icon_size, m_icon_size });

			if (!directoryEntry.is_directory() && ImGui::BeginDragDropSource())
			{
				auto relativePath = std::filesystem::relative(path, g_asset_path);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("Assets_Panel", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			
			if (ImGui::IsItemFocused() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
					m_cur_selected_dir = directoryEntry;

			}
			if (ImGui::IsItemHovered()&& ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory()){
						m_cur_asset_path /= path.filename();
						m_cur_selected_dir = std::filesystem::directory_entry();
				}
			}
			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::Columns(1);

		// TODO: status bar
		ImGui::End();

	}

	bool AssetPanel::file_icon(const char* label, bool isSelected, void* icon, glm::vec2 size, glm::vec4 layout /*= { 1, 1, 0, 0 }*/, bool hasPreview /*= false*/, int previewWidth /*= 0*/, int previewHeight /*= 0*/)
	{
			ImGuiStyle& style = ImGui::GetStyle();
			ImGuiContext& g = *GImGui;
			ImGuiWindow* window = g.CurrentWindow;

			float windowSpace = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
			ImVec2 pos = window->DC.CursorPos;
			bool ret = false;

			if (ImGui::InvisibleButton(label, { size.x, size.y }))
				ret = true;

			bool hovered = ImGui::IsItemHovered();
			bool active = ImGui::IsItemActive();
			bool doubleClick = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
			if (doubleClick && hovered)
				ret = true;


			float iconSize = size.y - g.FontSize * 2;
			float iconPosX = pos.x + (size.x - iconSize) / 2.0f;
			ImVec2 textSize = ImGui::CalcTextSize(label, 0, true, size.x);


			if (hovered || active || isSelected)
				window->DrawList->AddRectFilled(g.LastItemData.Rect.Min, g.LastItemData.Rect.Max, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[active ? ImGuiCol_HeaderActive : (isSelected ? ImGuiCol_Header : ImGuiCol_HeaderHovered)]));

			if (hasPreview) {
				ImVec2 availSize = ImVec2(size.x, iconSize);

				float scale = std::min<float>(availSize.x / previewWidth, availSize.y / previewHeight);
				availSize.x = previewWidth * scale;
				availSize.y = previewHeight * scale;

				float previewPosX = pos.x + (size.x - availSize.x) / 2.0f;
				float previewPosY = pos.y + (iconSize - availSize.y) / 2.0f;

				window->DrawList->AddImage(icon, ImVec2(previewPosX, previewPosY), ImVec2(previewPosX + availSize.x, previewPosY + availSize.y), { layout.x, layout.y }, { layout.z, layout.w });
			}
			else
				window->DrawList->AddImage(icon, ImVec2(iconPosX, pos.y), ImVec2(iconPosX + iconSize, pos.y + iconSize), { layout.x, layout.y }, { layout.z, layout.w });

			window->DrawList->AddText(g.Font, g.FontSize, ImVec2(pos.x + (size.x - textSize.x) / 2.0f, pos.y + iconSize), ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_Text]), label, 0, size.x);

			float lastButtomPos = ImGui::GetItemRectMax().x;
			float thisButtonPos = lastButtomPos + style.ItemSpacing.x + size.x; // Expected position if next button was on same line
			if (thisButtonPos < windowSpace)
				ImGui::SameLine();
			return ret;
	}

}



