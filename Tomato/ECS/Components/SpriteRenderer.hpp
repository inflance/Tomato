#pragma once

#include "glm/vec4.hpp"

namespace Tomato
{
	struct SpriteComponent
	{
		enum class Render2DType
		{
			Quad = 0,
			Circle,
		};

		Render2DType geometry_type_ = Render2DType::Quad;
		glm::vec4 color_{1.0f};
		float tiling_factor_ = 1.0f;
		//white texture
		//std::shared_ptr<Texture2D> texture_ = nullptr;

		SpriteComponent() = default;

		SpriteComponent(const glm::vec4& color)
			: color_(color)
		{
		}
		SpriteComponent(const SpriteComponent& other) = default;
	};

	inline static std::unordered_map<SpriteComponent::Render2DType, std::string> geometry_type_to_str
	{
		{SpriteComponent::Render2DType::Quad, "Quad"},
		{SpriteComponent::Render2DType::Circle, "Circle"},
	};

	inline static std::unordered_map<std::string, SpriteComponent::Render2DType> str_to_geometry_type
	{
		{"Quad", SpriteComponent::Render2DType::Quad},
		{"Circle", SpriteComponent::Render2DType::Circle},
	};
}
