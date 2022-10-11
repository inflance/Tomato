#include "Tomato/Input.h"
#include "Tomato/Engine.h"

#include <glfw/glfw3.h>


namespace Tomato {

	bool Input::IsKeyPressed(const int key)
	{
		auto* window = static_cast<GLFWwindow*>(TomatoEngine::GetInstance().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(const int button)
	{
		auto* window = static_cast<GLFWwindow*>(TomatoEngine::GetInstance().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		auto* window = static_cast<GLFWwindow*>(TomatoEngine::GetInstance().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return { (float)xPos, (float)yPos };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}
}