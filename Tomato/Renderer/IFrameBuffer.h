#pragma once

namespace Tomato {

	struct FrameBufferProps
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class IFrameBuffer 
	{
	public:
		virtual ~IFrameBuffer() = default;

		virtual const FrameBufferProps& GetFrameBufferProps() const = 0;

		virtual uint32_t GetID() const = 0;
		virtual uint32_t GetColorAttach() const = 0;
		//virtual void SetFrameBufferProps(FrameBufferProps& props) const = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		static std::shared_ptr<IFrameBuffer> Create(FrameBufferProps& props);
	};
}

