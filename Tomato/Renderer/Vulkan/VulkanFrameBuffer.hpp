#pragma once
#include "../FrameBuffer.hpp"
#include "vulkan/vulkan_raii.hpp"


namespace Tomato
{
	class VulkanFrameBuffer : public FrameBuffer
	{
	public:
		VulkanFrameBuffer(const FrameBufferInfo& info);
		~VulkanFrameBuffer() override;

		void Init();
		void OnResize(uint32_t width, uint32_t height) override;
		[[nodiscard]] const FrameBufferInfo& GetFrameBufferInfo() const override { return m_info; }

		vk::raii::Framebuffer m_framebuffer = nullptr;
	private:
		FrameBufferInfo m_info;
		std::vector<Ref<Texture2D>> m_texture ;
	};
}
