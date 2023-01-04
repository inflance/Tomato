#pragma once

namespace Tomato {

	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureProps
	{
		FramebufferTextureProps() = default;
		FramebufferTextureProps(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentProps
	{
		FramebufferAttachmentProps() = default;
		FramebufferAttachmentProps(std::initializer_list<FramebufferTextureProps> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureProps> Attachments;
	};

	struct FrameBufferProps
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;
		FramebufferAttachmentProps Attachments;
		bool SwapChainTarget = false;
	};

	class FrameBuffer 
	{
	public:
		virtual ~FrameBuffer() = default;

		[[nodiscard]] virtual const FrameBufferProps& GetFrameBufferProps() const = 0;

		[[nodiscard]] virtual uint32_t GetID() const = 0;

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		[[nodiscard]] virtual uint32_t GetColorAttachmentRID(uint32_t index = 0) const = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		static std::shared_ptr<FrameBuffer> Create(FrameBufferProps& props);
	};
}

