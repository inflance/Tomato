#pragma once

#include "Tomato/Renderer/FrameBuffer.h"
#include <vulkan/vulkan.h>

#include "Tomato/Core/Macro.h"

namespace Tomato
{
	class Texture2D;

	class VulkanFramebuffer : public Framebuffer
	{
	public:
		VulkanFramebuffer(const FramebufferProps& spec);
		virtual ~VulkanFramebuffer();

		virtual void Resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;
		virtual void AddResizeCallback(const std::function<void(Ref<Framebuffer>)>& func) override;

		virtual void Bind() const override {}
		virtual void Unbind() const override {}

		virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const override {}

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override{ return m_RendererID; }
		virtual uint32_t GetColorAttachmentRendererID() const { return 0; }
		virtual uint32_t GetDepthAttachmentRendererID() const { return 0; }

		virtual Ref<Texture2D> GetImage(uint32_t attachmentIndex = 0) const override { ASSERT(attachmentIndex < m_AttachmentImages.size()); return m_AttachmentImages[attachmentIndex]; }
		virtual Ref<Texture2D> GetDepthImage() const override { return m_DepthAttachmentImage; }
		size_t GetColorAttachmentCount() const { return m_Specification.SwapChainTarget ? 1 : m_AttachmentImages.size(); }
		bool HasDepthAttachment() const { return m_DepthAttachmentImage == nullptr; }
		VkRenderPass GetRenderPass() const { return m_RenderPass; }
		VkFramebuffer GetVulkanFramebuffer() const { return m_Framebuffer; }
		const std::vector<VkClearValue>& GetVulkanClearValues() const { return m_ClearValues; }

		virtual const FramebufferProps& GetSpecification() const override { return m_Specification; }

		void Invalidate();
		void RT_Invalidate();
		void Release();
	private:
		FramebufferProps m_Specification;
		uint32_t m_RendererID = 0;
		uint32_t m_Width = 0, m_Height = 0;

		std::vector<Ref<Texture2D>> m_AttachmentImages;
		Ref<Texture2D> m_DepthAttachmentImage;

		std::vector<VkClearValue> m_ClearValues;

		VkRenderPass m_RenderPass = nullptr;
		VkFramebuffer m_Framebuffer = nullptr;

		std::vector<std::function<void(Ref<Framebuffer>)>> m_ResizeCallbacks;
	};
}
