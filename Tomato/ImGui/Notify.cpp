#include "Notify.hpp"

#include "imgui.h"
#include "ThirdParty/imgui-notify/src/imgui_notify.h"
#include "ThirdParty/imgui-notify/tahoma.h"

namespace Tomato
{
	inline static std::unordered_map<Notify::NotifyLevel, ImGuiToastType_> level_change{
		{Notify::NotifyLevel::Success, ImGuiToastType_Success},
		{Notify::NotifyLevel::Error, ImGuiToastType_Error},
		{Notify::NotifyLevel::Info, ImGuiToastType_Info},
		{Notify::NotifyLevel::Warning, ImGuiToastType_Warning},
	};

	void Notify::Init()
	{
		ImGuiIO* io = &ImGui::GetIO();
		ImFontConfig font_cfg;
		font_cfg.FontDataOwnedByAtlas = false;
		io->Fonts->AddFontFromMemoryTTF((void*)tahoma, sizeof(tahoma), 17.f, &font_cfg);

		// Initialize notify
		ImGui::MergeIconsWithLatestFont(16.f, false);
	}

	void Notify::Insert(NotifyLevel level, int time, std::string_view format, std::string_view str)
	{
		ImGui::InsertNotification({
			level_change[level], time, format.data(), str.data()
		});
	}

	void Notify::Render()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f); // Round borders
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
		// Background color
		ImGui::RenderNotifications(); // <-- Here we render all notifications
		ImGui::PopStyleVar(1); // Don't forget to Pop()
		ImGui::PopStyleColor(1);
	}
}
