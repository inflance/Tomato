#pragma once

#include <glm/glm.hpp>

#include "Tomato/Core/Macro.h"

namespace Tomato
{
	enum class TextureFormat : uint32_t;
	class Texture2D;
	class FrameBuffer;

	struct ClearValue
	{
		glm::vec4 clear_color_{};
		glm::vec2 clear_depth_{};
	};

	struct RenderPassInfo
	{
		ClearValue clear_value{};
		uint32_t width_{}, height_{};
		std::optional<uint32_t> sample_count;
		std::vector<TextureFormat> format_;
		std::vector<Ref<Texture2D>> attachments_;
	};

	class RenderPass
	{
	public:
		virtual ~RenderPass()
		{
		}
		virtual void OnResize(uint32_t width, uint32_t height)  = 0;

		[[nodiscard]]virtual const RenderPassInfo& GetProps() const  = 0;
		virtual void SetProps( RenderPassInfo props) = 0;
		static Ref<RenderPass> Create( RenderPassInfo props);
	};
}
