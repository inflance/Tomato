#include "VulkanUniformBuffer.h"

#include "VulkanContext.h"

namespace Tomato
{
	VulkanUniformBuffer::VulkanUniformBuffer(uint32_t size, uint32_t binding)
		: m_size(size), binding(binding)
	{
		const auto& device = VulkanContext::Get().device;
		const auto& physicalDevice = VulkanContext::Get().physicalDevice;
		const vk::MemoryPropertyFlags propertyFlags = vk::MemoryPropertyFlagBits::eHostVisible |
			vk::MemoryPropertyFlagBits::eHostCoherent;
		uniformDataBuffer = vk::raii::Buffer(
			device, vk::BufferCreateInfo({}, size, vk::BufferUsageFlagBits::eUniformBuffer));
		uniformDataMemory = vk::raii::DeviceMemory(Utils::allocateDeviceMemory(
			device, physicalDevice.getMemoryProperties(), uniformDataBuffer.getMemoryRequirements(), propertyFlags));
		uniformDataBuffer.bindMemory(*uniformDataMemory, 0);
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
	}

	void VulkanUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		assert(size <= m_size);
		auto pData = static_cast<uint8_t*>(uniformDataMemory.mapMemory(offset, size));
		memcpy(pData, (uint8_t*)(data) + offset, size);
		uniformDataMemory.unmapMemory();

		//uniformDataBuffer.bindMemory(*uniformDataMemory, 0);
	}
}
