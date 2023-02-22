#include "OpenGLFrameBuffer.h"

#include"Tomato/Core/Core.h"

#include <glad/glad.h>

namespace Tomato {

	namespace Utils {
		
		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
			}
			return false;
		}

		static GLenum FBTextureFormatToGL(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
			case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}

			LOG_ASSERT(false, "");
			return 0;
		}

	}

	static constexpr uint32_t MaxFrameSize = 8196;

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferProps& props)
		:m_frame_buffer_props(props)
	{
		for (auto& props : m_frame_buffer_props.Attachments.Attachments) {
			if (!Utils::IsDepthFormat(props.TextureFormat))
				m_colorAttachmentProps.emplace_back(props);
			else
				m_depthAttachmentProps = props;
		}
		Init();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		Delete();
	}

	int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		LOG_ASSERT(attachmentIndex < m_colorAttachments.size(), "");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		const auto& spec = m_colorAttachmentProps[attachmentIndex];
		glClearTexImage(m_colorAttachments[attachmentIndex], 0,
			Utils::FBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
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
		if (m_RendererID){
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
			glDeleteTextures(1, &m_depthAttachment);

			m_colorAttachments.clear();
			m_depthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		//Attachment
		bool multi_sample = m_frame_buffer_props.Samples > 1;

		if (!m_colorAttachmentProps.empty())
		{
			m_colorAttachments.resize(m_colorAttachmentProps.size());
			CreateTextures(multi_sample, m_colorAttachments.data(), m_colorAttachments.size());

			for (size_t i = 0; i < m_colorAttachments.size(); i++)
			{
				BindTexture(multi_sample, m_colorAttachments[i]);
				switch (m_colorAttachmentProps[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					AttachColorTexture(m_colorAttachments[i], m_frame_buffer_props.Samples, GL_RGBA8, GL_RGBA, m_frame_buffer_props.Width, m_frame_buffer_props.Height, i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					AttachColorTexture(m_colorAttachments[i], m_frame_buffer_props.Samples, GL_R32I, GL_RED_INTEGER, m_frame_buffer_props.Width, m_frame_buffer_props.Height, i);
					break;
				}
			}

			if (m_depthAttachmentProps.TextureFormat != FramebufferTextureFormat::None)
			{
				CreateTextures(multi_sample, &m_depthAttachment, 1);
				BindTexture(multi_sample, m_depthAttachment);
				switch (m_depthAttachmentProps.TextureFormat)
				{
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					AttachDepthTexture(m_depthAttachment, m_frame_buffer_props.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_frame_buffer_props.Width, m_frame_buffer_props.Height);
					break;
				}
			}
		}

		if (m_colorAttachments.size() > 1)
		{
			LOG_ASSERT(m_colorAttachments.size() <= 4,"");
			constexpr GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_colorAttachments.size(), buffers);
		}
		else if (m_colorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}
		
		LOG_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,"Failed to compile FrameBuffer!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Delete() const
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
		glDeleteTextures(1, &m_depthAttachment);
	}

	GLenum OpenGLFrameBuffer::TextureTarget(bool multi_sampled)
	{
		return multi_sampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	void OpenGLFrameBuffer::CreateTextures(bool multi_sampled, uint32_t* id, uint32_t count)
	{
		glCreateTextures(TextureTarget(multi_sampled), count, id);
	}

	void OpenGLFrameBuffer::BindTexture(bool multi_sampled, uint32_t color_attachment)
	{
		glBindTexture(TextureTarget(multi_sampled), color_attachment);
	}

	void OpenGLFrameBuffer::AttachColorTexture(uint32_t id, int samples, GLenum internal_format, GLenum format, uint32_t width, uint32_t height, int index)
	{
		bool multi_sampled = samples > 1;
		if (multi_sampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internal_format, width, height, GL_FALSE);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multi_sampled), id, 0);
	}

	void OpenGLFrameBuffer::AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachment_type, uint32_t width, uint32_t height)
	{
		const bool multi_sampled = samples > 1;
		if (multi_sampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, TextureTarget(multi_sampled), id, 0);
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