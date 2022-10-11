#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Tomato {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:	return GL_FLOAT;
		case ShaderDataType::Float2:return GL_FLOAT;
		case ShaderDataType::Float3:return GL_FLOAT;
		case ShaderDataType::Float4:return GL_FLOAT;
		case ShaderDataType::Mat3:	return GL_FLOAT;
		case ShaderDataType::Mat4:	return GL_FLOAT;
		case ShaderDataType::Int:	return GL_INT;
		case ShaderDataType::Int2:	return GL_INT;
		case ShaderDataType::Int3:	return GL_INT;
		case ShaderDataType::Int4:	return GL_INT;
		case ShaderDataType::Bool:	return GL_BOOL;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_renderer_id);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_renderer_id);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_renderer_id);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_renderer_id);
		vertexBuffer->Bind();

		uint32_t index = 0;
		for (const auto& element : vertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(index);

			glVertexAttribPointer(index,
				element.getComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				vertexBuffer->GetLayout().GetStride(),
				(const void*)element.Offset);

			index++;
		}
		m_vertex_buffer.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_renderer_id);
		indexBuffer->Bind();

		m_index_buffer = indexBuffer;
	}

}