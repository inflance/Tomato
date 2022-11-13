#pragma once

#include "Tomato/Renderer/IFrameBuffer.h"

namespace Tomato {

	class OpenGLFrameBuffer : public IFrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferProps& props);

		virtual ~OpenGLFrameBuffer();

		const FrameBufferProps& GetFrameBufferProps() const override { return m_frame_buffer_props; }
		virtual uint32_t GetID() const override{ return m_RendererID; }
		virtual uint32_t GetColorAttach() const override{ return m_ColorAttach; }

		void Bind() const override;
		void UnBind() const override;

		void Resize(uint32_t width, uint32_t height) override;

	private:
		void Init();
		void Delete();

	private:
		FrameBufferProps m_frame_buffer_props;
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttach = 0;
		uint32_t m_DepthAttach = 0;
	};
}
