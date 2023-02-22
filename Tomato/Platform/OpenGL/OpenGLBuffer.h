#pragma once

#include "Tomato/Renderer/Buffer.h"

namespace Tomato {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		~OpenGLVertexBuffer() override;

		void Bind() const override;
		void UnBind() const override;

		void SetData(const void* data, uint32_t size) override;

		[[nodiscard]] const BufferLayout& GetLayout() const override { return m_layout; }
		void SetLayout(const BufferLayout& layout) override { m_layout = layout; }
	private:
		uint32_t m_renderer_id;
		BufferLayout m_layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer() override;

		void Bind() const override;
		void UnBind() const override;
		[[nodiscard]] uint32_t GetCount() const override { return m_count; }
	private:
		uint32_t m_renderer_id;
		uint32_t m_count;
	};
}