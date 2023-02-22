#pragma once

#include <glm/glm.hpp>

#include "Tomato/Core/Macro.h"

namespace Tomato
{
	struct ClearValue
	{
		glm::vec4 ClearColor{};
		glm::vec2 ClearDepth{};
	};

	struct RenderPassProps
	{
		ClearValue ClearValue{};
	};

	class RenderPass
	{
	public:
		virtual ~RenderPass()
		{
		}

		virtual void SetProps(const RenderPassProps& props) = 0;
		static Ref<RenderPass> Create(const RenderPassProps& props);
	};
}
