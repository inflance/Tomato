#pragma once

#include "Vulkan.h"
#include "Tomato/Renderer/UniformBuffer.h"

namespace Tomato
{
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer();
		~VulkanUniformBuffer() override;


		void SetData(const void* data, uint32_t size, uint32_t offset) override;
		VkDescriptorBufferInfo& GetDescriptorBufferInfo() { return m_descriptor_buffer_info; }
	private:
		VkDescriptorBufferInfo m_descriptor_buffer_info{};
	};
}
