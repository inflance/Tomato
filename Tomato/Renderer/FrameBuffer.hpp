#pragma once
#include "Texture.hpp"
#include "Tomato/Core/Macro.h"

namespace Tomato
{
	class RenderPass;
	class Framebuffer;
	class Texture2D;

	struct FrameBufferInfo
	{
		uint32_t width_;
		uint32_t height_;
		uint32_t layer_ = 1;
		std::shared_ptr<RenderPass> render_pass;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer()
		{
		}

		virtual void OnResize(uint32_t width, uint32_t height) = 0;
		[[nodiscard]]virtual const FrameBufferInfo& GetFrameBufferInfo() const = 0;
		static Ref<FrameBuffer> Create(const FrameBufferInfo& spec);
	};
}
