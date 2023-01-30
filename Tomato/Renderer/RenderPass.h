#pragma once

#include <glm/vec4.hpp>

#include "Tomato/Core/Macro.h"

namespace Tomato
{
	class Framebuffer;
	class Texture;
	class CommandBuffer;

	struct RenderPassProps
	{
		std::vector<std::shared_ptr<Texture>> Attachments;
		std::shared_ptr<Texture>              DepthTarget;
		bool                                  IsClear = true;
	};


	struct RenderPassSpecification
	{
		Ref<Framebuffer> TargetFramebuffer;
		std::string DebugName;
		glm::vec4 MarkerColor;
	};


	class RenderPass
	{
	public:
		virtual ~RenderPass() = default;

		virtual RenderPassSpecification& GetSpecification() = 0;
		virtual const RenderPassSpecification& GetSpecification() const = 0;

		static Ref<RenderPass> Create(const RenderPassSpecification& spec);
	};
}
