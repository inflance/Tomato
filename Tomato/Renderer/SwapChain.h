#pragma once
#include "CommandBuffer.h"
#include "Texture.h"
#include "Tomato/Core/Core.h"

namespace Tomato
{
	class Window;

	class SwapChain
	{
	public:
		virtual ~SwapChain() = default;
		virtual void Init(Window* window) = 0;
		virtual std::shared_ptr<Texture> GetCurrentImage() =0;
		virtual std::shared_ptr<Texture> GetImage(uint32_t index) =0;
		virtual uint32_t GetCurrentFrameIndex() const = 0;
		virtual uint32_t GetCurrentImageIndex() const =0;
		virtual size_t GetSwapChainBufferCount() const =0;
		virtual CommandBuffer* GetCurrentCommandBuffer() =0;
		virtual CommandBuffer* GetComputeCmdBuffer() =0;
		static Ref<SwapChain> Create(uint32_t width, uint32_t height);
	};
}
