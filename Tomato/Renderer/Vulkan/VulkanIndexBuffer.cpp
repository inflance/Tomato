#include "VulkanIndexBuffer.h"

#include "VulkanContext.h"
#include "VulkanUtils.h"

namespace Tomato
{
	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices, uint32_t count)
		: m_count(count)
	{
		const auto& device = VulkanContext::Get().device;
		const auto& physicalDevice = VulkanContext::Get().physicalDevice;
		const vk::MemoryPropertyFlags propertyFlags = vk::MemoryPropertyFlagBits::eHostVisible |
			vk::MemoryPropertyFlagBits::eHostCoherent;
		uint32_t size = sizeof(uint32_t) * count;
		m_index_buffer =
			vk::raii::Buffer(device, vk::BufferCreateInfo({}, size, vk::BufferUsageFlagBits::eIndexBuffer));
		m_device_memory = vk::raii::DeviceMemory(Utils::allocateDeviceMemory(
			device, physicalDevice.getMemoryProperties(), m_index_buffer.getMemoryRequirements(), propertyFlags));
		m_index_buffer.bindMemory(*m_device_memory, 0);
		uint32_t* pData = static_cast<uint32_t*>(m_device_memory.mapMemory(0, size));
		memcpy(pData, indices, size);
		m_device_memory.unmapMemory();
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
	}
}
