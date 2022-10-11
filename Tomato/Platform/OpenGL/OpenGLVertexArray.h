#pragma once

#include "Tomato/Renderer/VertexArray.h"

namespace Tomato {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const override{ return m_index_buffer; }
		virtual std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffer() const override{ return m_vertex_buffer; }
	private:
		uint32_t m_renderer_id;

		std::vector<std::shared_ptr<VertexBuffer>> m_vertex_buffer;
		std::shared_ptr<IndexBuffer> m_index_buffer;
	};
}