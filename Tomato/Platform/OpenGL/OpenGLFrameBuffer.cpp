#include "OpenGLFrameBuffer.h"

#include"Tomato/Core.h"

#include <glad/glad.h>

namespace Tomato {

	static const uint32_t MaxFrameSize = 8196;

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferProps& props)
		:m_frame_buffer_props(props)
	{
		Init();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_frame_buffer_props.Width, m_frame_buffer_props.Height);
	}

	void OpenGLFrameBuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Init()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_ColorAttach);
			glDeleteTextures(1, &m_DepthAttach);
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glCreateTextures(GL_TEXTURE_2D,1, &m_ColorAttach);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttach);

		/*glTextureStorage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_frame_buffer_props.Width, m_frame_buffer_props.Height);*/

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_frame_buffer_props.Width, m_frame_buffer_props.Height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttach, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttach);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttach);

		//glTextureStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_frame_buffer_props.Width, m_frame_buffer_props.Height);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_frame_buffer_props.Width, m_frame_buffer_props.Height,
			0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttach, 0);
		
		LOG_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,"Failed to compile FrameBuffer!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Delete()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttach);
		glDeleteTextures(1, &m_DepthAttach);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height) 
	{
		if (width == 0 || height == 0 || width > MaxFrameSize || height > MaxFrameSize)
		{
			LOG_WARN("Invilied Frame Size: width{0}, height:{1}", width, height);
			return;
		}
		m_frame_buffer_props.Width = width;
		m_frame_buffer_props.Height = height;

		Init();
	}

}