#pragma once

#include "Tomato/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Tomato {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowhandled);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_window_handled;
	};
}