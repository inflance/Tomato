#pragma once
#include "Tomato/Core/Macro.h"

namespace Tomato
{
	class Framebuffer;
	class Texture2D;

	enum class FramebufferBlendMode
	{
		None = 0,
		OneZero,
		SrcAlphaOneMinusSrcAlpha,
		Additive,
		Zero_SrcColor
	};


	enum class FramebufferTextureFormat
	{
		None = 0,

		// color_
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
			: TextureFormat(format)
		{
		}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
		FramebufferBlendMode BlendMode = FramebufferBlendMode::None;
		bool Blend = false;
	};

	struct FramebufferAttachmentProps
	{
		FramebufferAttachmentProps() = default;

		FramebufferAttachmentProps(std::initializer_list<FramebufferTextureProps> attachments)
			: Attachments(attachments)
		{
		}

		std::vector<FramebufferTextureProps> Attachments;
	};

	struct FramebufferProps
	{
		std::string debug_name_{};
		uint32_t Width, Height;
		uint32_t Samples = 1;
		FramebufferAttachmentProps Attachments;
		bool SwapChainTarget = false;
		FramebufferBlendMode BlendMode = FramebufferBlendMode::None;
		bool Blend = false;
		Ref<Framebuffer> ExistingFramebuffer{};
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer()
		{
		}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height, bool forceRecreate = false) = 0;
		virtual void AddResizeCallback(const std::function<void(Ref<Framebuffer>)>& func) = 0;

		virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual uint32_t GetRendererID() const = 0;

		virtual Ref<Texture2D> GetImage(uint32_t attachmentIndex = 0) const = 0;
		virtual Ref<Texture2D> GetDepthImage() const = 0;

		virtual const FramebufferProps& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferProps& spec);
	};
}
