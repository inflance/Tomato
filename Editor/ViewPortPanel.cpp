#include "ViewPortPanel.hpp"

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"

namespace Tomato
{
	ViewPortPanel::ViewPortPanel(void* id, float width, float height, std::string name)
		: m_id(id), m_width(width), m_height(height), m_name(std::move(name))
	{
	}

	ViewPortPanel::~ViewPortPanel()
	{
		ImGui_ImplVulkan_RemoveTexture((VkDescriptorSet)(m_id));
	}

	void ViewPortPanel::Begin() const
	{
		if (m_id)
		{
			ImGui::PushStyleColor(1, {0, 0, 0, 0});

			ImGui::Begin(m_name.c_str());
		}
	}

	void ViewPortPanel::Tick()
	{
		if (m_id)
		{
			const ImVec2 view_port_size = ImGui::GetContentRegionAvail();
			if ((m_width != view_port_size.x || m_height != view_port_size.y) && view_port_size.x > 0.0f &&
				view_port_size.y > 0.0f)
			{
				m_width = view_port_size.x;
				m_height = view_port_size.y;

				for (auto& fn : m_fn)
				{
					fn(m_width, m_height);
				}
			}
			ImGui::Image(m_id, {m_width, m_height},{0,0},{1,1});
		}
	}

	void ViewPortPanel::End() const
	{
		if (m_id)
		{
			ImGui::PopStyleColor(1);
			ImGui::End();
		}
	}

	void ViewPortPanel::UpdateViewPort(float width, float height)
	{
		m_width = width;
		m_height = height;
	}

	void ViewPortPanel::UpdateName(std::string name)
	{
		m_name = std::move(name);
	}

	void ViewPortPanel::UpdateImage(void* id)
	{
		ImGui_ImplVulkan_RemoveTexture((VkDescriptorSet)(m_id));
		m_id = id;
	}

	void ViewPortPanel::SetEventTick(EventFn&& fn)
	{
		m_fn.emplace_back(fn);
	}

	std::shared_ptr<ViewPortPanel> ViewPortPanel::Create(void* id, float width, float height, std::string name)
	{
		return std::make_shared<ViewPortPanel>(id, width, height, std::move(name));
	}
}
