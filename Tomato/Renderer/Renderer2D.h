#pragma once

#include "Tomato/ECS/Components.h"
#include "Tomato/Function/Camera/Camera.h"
#include "Tomato/Function/Camera/EditorCamera.h"
#include "Tomato/Renderer/Texture.h"
#include "Tomato/Renderer/SubTexture2D.h"

namespace Tomato
{
	struct Vertex2D
	{
		glm::vec3 position_;
		glm::vec4 color_;
		glm::vec2 tex_coord_;
		float tex_index_;
		float tiling_factor_;
	};

	struct TextVertex
	{
		glm::vec3 position_;
		glm::vec4 color_;
		glm::vec2 tex_coord_;
		float tex_index_;
	};

	struct LineVertex
	{
		glm::vec3 position_;
		glm::vec4 color_;
	};

	struct CircleVertex
	{
		glm::vec3 world_position_;
		float thickness_;
		glm::vec2 local_position_;
		glm::vec4 color_;
	};

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void RenderQuad(const glm::mat4& transform, const glm::vec4& color);
		static void RenderQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void RenderCircle();
		static void RenderLine();
	};
}
