#pragma once

#include "Tomato/Renderer/Buffer.h"

namespace Tomato
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(uint32_t size);
		VulkanVertexBuffer(float* vertices, uint32_t size);
		~VulkanVertexBuffer() override;

		void Bind() const override;
		void UnBind() const override;

		void SetData(const void* data, uint32_t size) override;

		[[nodiscard]] const BufferLayout& GetLayout() const override { return mLayout; }
		void SetLayout(const BufferLayout& layout) override { mLayout = layout; }

	private:
		uint32_t m_renderer_id;
		BufferLayout mLayout;
	};

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(uint32_t* indices, uint32_t count);
		~VulkanIndexBuffer() override;

		void Bind() const override;
		void UnBind() const override;
		[[nodiscard]] uint32_t GetCount() const override { return m_count; }

	private:
		uint32_t m_renderer_id;
		uint32_t m_count;
	};
}
