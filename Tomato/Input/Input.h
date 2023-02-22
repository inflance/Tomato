#pragma once

#include <glm/glm.hpp>

#include "Tomato/Input/Codes.h"

namespace Tomato
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);

		static float GetMouseX();
		static float GetMouseY();
		static glm::vec2 GetMousePosition();
	};
}
