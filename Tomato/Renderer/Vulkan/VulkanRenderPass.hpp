#pragma once

#include "../RenderPass.hpp"
#include "Vulkan.hpp"

namespace Tomato
{
	class VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass(RenderPassInfo props);
		~VulkanRenderPass() override = default;

		void Init();
		void InitRenderPass();
		void SetProps(RenderPassInfo props) override { m_props = props; };
		const RenderPassInfo& GetProps() const override { return m_props; };
		const auto& GetHandle() const { return renderPass; };

		void OnResize(uint32_t width, uint32_t height) override;

	private:

		RenderPassInfo m_props;
	public:
		vk::raii::RenderPass renderPass = nullptr;
	};
}
