#include "VulkanVertexBuffer.h"

#include "VulkanContext.h"

namespace Tomato
{
	namespace Utils
	{
		static inline auto ShaderDataTypeToVulkanFormat(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float: return vk::Format::eR32Sfloat;
			case ShaderDataType::Float2: return vk::Format::eR32G32Sfloat;
			case ShaderDataType::Float3: return vk::Format::eR32G32B32Sfloat;
			case ShaderDataType::Float4: return vk::Format::eR32G32B32A32Sfloat;
			case ShaderDataType::Mat3: return vk::Format::eUndefined;
			case ShaderDataType::Mat4: return vk::Format::eAstc4x4SfloatBlock;
			case ShaderDataType::Int: return vk::Format::eR32Sint;
			case ShaderDataType::Int2: return vk::Format::eR32G32Sint;
			case ShaderDataType::Int3: return vk::Format::eR32G32B32Sint;
			case ShaderDataType::Int4: return vk::Format::eR32G32B32A32Sint;
			case ShaderDataType::Bool: return vk::Format::eR8Uint;
			case ShaderDataType::None: ;
			default: break;
			}
			assert(false);
			return vk::Format::eUndefined;
		}
	}

	VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size)
		: m_size(size)
	{
		const auto& device = VulkanContext::Get().device;
		const auto& physicalDevice = VulkanContext::Get().physicalDevice;
		const vk::MemoryPropertyFlags propertyFlags = vk::MemoryPropertyFlagBits::eHostVisible |
			vk::MemoryPropertyFlagBits::eHostCoherent;
		buffer = vk::raii::Buffer(device, vk::BufferCreateInfo({}, size, vk::BufferUsageFlagBits::eVertexBuffer));
		deviceMemory = vk::raii::DeviceMemory(Utils::allocateDeviceMemory(
			device, physicalDevice.getMemoryProperties(), buffer.getMemoryRequirements(), propertyFlags));
		buffer.bindMemory(*deviceMemory, 0);
	}

	VulkanVertexBuffer::VulkanVertexBuffer(float* vertices, uint32_t size)
	{
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
	}

	void VulkanVertexBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		assert(size <= m_size);
		auto deviceData = static_cast<uint8_t*>(deviceMemory.mapMemory(offset, size));

		memcpy(deviceData, (uint8_t*)(data) + offset, size);

		deviceMemory.unmapMemory();
	}

	std::vector<std::pair<vk::Format, uint32_t>> VulkanVertexBuffer::GetVulkanVertexLayout(
		const VertexBufferLayout& layout)
	{
		std::vector<std::pair<vk::Format, uint32_t>> res;
		assert(layout.GetElementCount());
		for (auto& lay : layout)
		{
			res.emplace_back(Utils::ShaderDataTypeToVulkanFormat(lay.Type), lay.Offset);
		}
		return res;
	}
}
