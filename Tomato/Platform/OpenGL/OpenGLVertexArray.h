#pragma once

#include "Tomato/Renderer/VertexArray.h"

namespace Tomato {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;

		void Bind() const override;
		void UnBind() const override;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		[[nodiscard]] std::shared_ptr<IndexBuffer> GetIndexBuffer() const override{ return m_index_buffer; }
		[[nodiscard]] std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffer() const override{ return m_vertex_buffer; }
	private:
		uint32_t m_renderer_id;
		uint32_t m_vertex_buffer_Index = 0;

		std::vector<std::shared_ptr<VertexBuffer>> m_vertex_buffer;
		std::shared_ptr<IndexBuffer> m_index_buffer;

	};
}