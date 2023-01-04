#pragma once

#include "Tomato/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Tomato {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		OpenGLContext(void* windowHandle);

		void Init() override;
		void SwapBuffers() override;

	private:
		GLFWwindow* m_window_handle;
	};
}