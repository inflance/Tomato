#pragma once

#include "Tomato/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Tomato {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		OpenGLContext(void* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_window_handle;
	};
}