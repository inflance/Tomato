#include "OpenGLContext.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Tomato/Core/Core.h"

namespace Tomato {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_window_handle(windowHandle)
	{

	}

	OpenGLContext::OpenGLContext(void* windowHandle)
		:m_window_handle((GLFWwindow*)windowHandle)
	{

	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_window_handle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		LOG_ASSERT(status, "");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_window_handle);
	}

}