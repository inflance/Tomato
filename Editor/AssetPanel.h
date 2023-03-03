#pragma once

#include "Tomato/Renderer/Texture.hpp"

#include <filesystem>
#include <glm/glm.hpp>

namespace Tomato
{
	class AssetPanel
	{
	public:
		AssetPanel();

		void OnImGuiRenderer();
		static bool file_icon(const char* label, bool isSelected, void* icon, glm::vec2 size,
		                      glm::vec4 layout = {1, 1, 0, 0}, bool hasPreview = false, int previewWidth = 0,
		                      int previewHeight = 0);

	private:
		std::filesystem::path m_cur_asset_path;
		std::filesystem::directory_entry m_cur_selected_dir;

		std::shared_ptr<Texture2D> m_dir_icon;
		std::shared_ptr<Texture2D> m_file_icon;

		float m_icon_padding = 16.0f;
		float m_icon_size = 88.0f;
	};
}
