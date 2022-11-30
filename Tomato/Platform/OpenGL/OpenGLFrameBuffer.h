#pragma once

#include "Tomato/Renderer/FrameBuffer.h"

namespace Tomato {

	using GLenum = uint32_t;

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferProps& props);

		virtual ~OpenGLFrameBuffer();

		const FrameBufferProps& GetFrameBufferProps() const override { return m_frame_buffer_props; }
		virtual uint32_t GetID() const override{ return m_RendererID; }
		virtual uint32_t GetColorAttachmentRID(uint32_t index = 0) const override{ return m_colorAttachments[index]; }

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		void Bind() const override;
		void UnBind() const override;

		void Resize(uint32_t width, uint32_t height) override;

	private:
		void Init();
		void Delete();
		GLenum TextureTarget(bool multisampled);
		void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count);
		void BindTexture(bool multisampled, uint32_t colorAttachment);
		void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index);
		void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height);
	private:
		uint32_t m_RendererID = 0;
		FrameBufferProps m_frame_buffer_props;

		std::vector<FramebufferTextureProps> m_colorAttachmentProps;
		FramebufferTextureProps m_depthAttachmentProps = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_colorAttachments;
		uint32_t m_depthAttachment = 0;
	};
}
