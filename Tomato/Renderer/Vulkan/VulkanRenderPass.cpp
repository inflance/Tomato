#include "Tomato/pch.h"
#include "VulkanRenderPass.h"

#include "VulkanContext.h"
#include "VulkanSwapChain.h"

namespace Tomato
{
	VulkanRenderPass::VulkanRenderPass(const RenderPassProps& props)
		: props(props)
	{
		const auto& device = VulkanContext::Get().device;
		const auto& physicalDevice = VulkanContext::Get().physicalDevice;
		const auto& swapChain = VulkanContext::Get().swapChain;
		const auto& depthFormat = VulkanContext::Get().m_depth_buffer_data.format;

		const vk::Format colorFormat = Utils::pickSurfaceFormat(
				physicalDevice.getSurfaceFormatsKHR(*swapChain->surface)).
			format;

		std::vector<vk::AttachmentDescription> attachmentDescriptions;

		vk::AttachmentDescription colorAttach{};
		colorAttach.setFormat(colorFormat)
		           .setSamples(vk::SampleCountFlagBits::e1)
		           .setLoadOp(vk::AttachmentLoadOp::eClear)
		           .setStoreOp(vk::AttachmentStoreOp::eStore)
		           .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
		           .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
		           .setInitialLayout(vk::ImageLayout::eUndefined)
		           .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

		vk::AttachmentDescription depthAttach{};
		depthAttach.setFormat(depthFormat)
		           .setSamples(vk::SampleCountFlagBits::e1)
		           .setLoadOp(vk::AttachmentLoadOp::eClear)
		           .setStoreOp(vk::AttachmentStoreOp::eDontCare)
		           .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
		           .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
		           .setInitialLayout(vk::ImageLayout::eUndefined)
		           .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

		attachmentDescriptions.emplace_back(colorAttach);
		attachmentDescriptions.emplace_back(depthAttach);

		vk::RenderPassCreateInfo renderPassCreateInfo{};
		renderPassCreateInfo.setAttachments(attachmentDescriptions);

		//attachment is the index of the AttachmentDescription array;
		vk::AttachmentReference colorReference{};
		colorReference.setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);
		vk::AttachmentReference depthReference{};
		depthReference.setAttachment(1).setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

		vk::SubpassDependency dependency{};
		dependency.setSrcSubpass(VK_SUBPASS_EXTERNAL)
		          .setDstSubpass(0)
		          .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
		          .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
		          .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
		vk::SubpassDescription subpass{};
		subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
		       .setColorAttachments(colorReference)
		       .setColorAttachmentCount(1)
		       .setPDepthStencilAttachment(&depthReference);

		renderPassCreateInfo
			.setDependencies(dependency)
			.setDependencyCount(1)
			.setSubpasses(subpass)
			.setSubpassCount(1);
		renderPass = vk::raii::RenderPass(device, renderPassCreateInfo);
	}

	void VulkanRenderPass::SetProps(const RenderPassProps& props)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}
}
