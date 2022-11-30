#pragma once

#include "Tomato/Core/Window.h"

namespace Tomato {

	class GraphicsContext 
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static std::shared_ptr<GraphicsContext> Create(void* window);
	};
}