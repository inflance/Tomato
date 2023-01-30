#include "Tomato/Input/Input.h"

#include <glfw/glfw3.h>

#include "Tomato/Core/Engine.h"

namespace Tomato
{
	bool Input::IsKeyPressed(KeyCode key)
	{
		auto* window = static_cast<GLFWwindow*>(TomatoEngine::Get().GetWindow().GetNativeWindow());
		const auto state = glfwGetKey(window, key);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		auto* window = static_cast<GLFWwindow*>(TomatoEngine::Get().GetWindow().GetNativeWindow());
		const auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		auto* window = static_cast<GLFWwindow*>(TomatoEngine::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return {static_cast<float>(xPos), static_cast<float>(yPos)};
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
