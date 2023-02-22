#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Tomato {

	//VertexBuffer------------------------------------------------

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glGenBuffers(1, &m_renderer_id);//gen+Bind
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		glGenBuffers(1, &m_renderer_id);//gen+Bind
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_renderer_id);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	//IndexBuffer-------------------------------------------------

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		:m_count(count)
	{
		glGenBuffers(1, &m_renderer_id);//gen+Bind
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_count) * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_renderer_id);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
	}

	void OpenGLIndexBuffer::UnBind() const 
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}