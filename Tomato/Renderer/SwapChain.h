#pragma once

#include "Tomato/Core/Macro.h"

namespace Tomato
{
	class Window;

	class SwapChain
	{
	public:
		virtual ~SwapChain()
		{
		};
		virtual void InitSurface(const Window* window, uint32_t width, uint32_t height) = 0;

		virtual void InitSwapChain(uint32_t width, uint32_t height) = 0;

		static Ref<SwapChain> Create(Window* window, uint32_t width, uint32_t height);
	};
}
