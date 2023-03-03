#pragma once

#include "Tomato/ImGui/ImGuiLayer.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace Tomato
{
	class VulkanImGuiLayer : public ImGuiLayer
	{
	public:
		VulkanImGuiLayer()
		{
		}

		~VulkanImGuiLayer() override = default;

		void OnCreate() override;
		void OnDestroy() override;
		void OnEvent(Event& event) override;
		void OnImGuiRenderer() override;
		void Begin() override;
		void End() override;

		void SetDarkModeColor() override;

		void SetBlock(bool block) override { m_block = block; }

	private:
		vk::raii::DescriptorPool descPool = nullptr;
	};
}
