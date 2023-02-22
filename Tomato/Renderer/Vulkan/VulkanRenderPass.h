#pragma once

#include "../RenderPass.h"
#include "Vulkan.h"

namespace Tomato
{
	class VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass(const RenderPassProps& props);
		~VulkanRenderPass() override = default;


		void SetProps(const RenderPassProps& props) override;

		RenderPassProps props;
		vk::raii::RenderPass renderPass = nullptr;
	};
}
