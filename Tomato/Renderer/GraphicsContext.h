#pragma once

#include "SwapChain.h"

namespace Tomato
{
	class Window;

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void WaitIdle() = 0;

		static std::shared_ptr<GraphicsContext> Create(Window* window);
		[[nodiscard]] inline auto GetSwapChain()const { return m_swap_chain; }
	protected:
		std::shared_ptr<SwapChain> m_swap_chain = nullptr;
	};
}
