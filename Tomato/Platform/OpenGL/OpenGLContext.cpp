#include "OpenGLContext.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>


namespace Tomato {

	OpenGLContext::OpenGLContext(GLFWwindow* windowhandled)
		:m_window_handled(windowhandled)
	{

	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_window_handled);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_window_handled);
	}

}