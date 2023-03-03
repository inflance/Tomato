#pragma once

#include "VulkanUtils.hpp"
#include "Tomato/Renderer/BufferLayout.hpp"
#include "Tomato/Renderer/VertexBuffer.hpp"

namespace Tomato
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(uint32_t size);
		VulkanVertexBuffer(float* vertices, uint32_t size);
		~VulkanVertexBuffer() override;

		void Bind() const override
		{
		}

		void UnBind() const override
		{
		}

		void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

		[[nodiscard]] const VertexBufferLayout& GetLayout() const override { return m_layout; }
		void SetLayout(const VertexBufferLayout& layout) override { m_layout = layout; }
		static std::vector<std::pair<vk::Format, uint32_t>> GetVulkanVertexLayout(const VertexBufferLayout& layout);

	public:
		vk::raii::Buffer buffer = nullptr;
		vk::raii::DeviceMemory deviceMemory = nullptr;

	private:
		VertexBufferLayout m_layout;
		uint32_t m_renderer_id{};
		uint32_t m_size{};
#if !defined( NDEBUG )
		vk::BufferUsageFlags m_usage;
		vk::MemoryPropertyFlags m_propertyFlags;
#endif
	};
}
