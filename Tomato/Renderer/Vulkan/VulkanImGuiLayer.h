#pragma once

#include "Tomato/ImGui/ImGuiLayer.h"

namespace Tomato
{
	class VulkanImGuiLayer : public ImGuiLayer
	{
	public:
		VulkanImGuiLayer()
			: ImGuiLayer()
		{
		}

		~VulkanImGuiLayer() override = default;

		void OnCreate() override;
		void OnDestroy() override;
		void OnEvent(Event& event) override;
		void OnImGuiRenderer() override;

		void Begin() override;
		void End() override;

		void SetDarkModeColor()override;

		void SetBlock(bool block) override { m_block = block; }

	};
}
