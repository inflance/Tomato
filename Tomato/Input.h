#pragma once

#include <glm/glm.hpp>

namespace Tomato {

	class Input
	{
	public:
		static bool IsKeyPressed(int key);
		static bool IsMouseButtonPressed(int button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}