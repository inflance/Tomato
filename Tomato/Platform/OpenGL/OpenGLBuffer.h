#pragma once

#include "Tomato/Renderer/Buffer.h"

namespace Tomato {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override { return m_layout; };
		virtual void SetLayout(const BufferLayout& layout) override { m_layout = layout; };
	private:
		uint32_t m_renderer_id;
		BufferLayout m_layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual uint32_t GetCount() const override { return m_count; };
	private:
		uint32_t m_renderer_id;
		uint32_t m_count;
	};
}