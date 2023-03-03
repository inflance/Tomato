#pragma once

#include <unordered_map>

#include <glm/vec2.hpp>

namespace Tomato
{
	struct RigidBody2DComponent
	{
		enum class BodyType
		{
			Static = 0,
			Dynamic,
			Kinematic
		};

		BodyType type_ = BodyType::Static;
		bool fixed_rotation_ = false;

		void* body_ = nullptr;

		RigidBody2DComponent() = default;
		RigidBody2DComponent(const RigidBody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 offset_ = {0.0f, 0.0f};
		glm::vec2 size_ = {0.5f, 0.5f};

		float density_ = 1.0f;
		float friction_ = 0.5f;
		float restitution_ = 0.0f;
		float restitution_threshold_ = 0.5f;

		void* fixture_ = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	inline static std::unordered_map<RigidBody2DComponent::BodyType, std::string> body_type_to_str
	{
		{RigidBody2DComponent::BodyType::Static, "Static"},
		{RigidBody2DComponent::BodyType::Dynamic, "Dynamic"},
		{RigidBody2DComponent::BodyType::Kinematic, "Kinematic"},
	};

	inline static std::unordered_map<std::string, RigidBody2DComponent::BodyType> str_to_body_type
	{
		{"Static", RigidBody2DComponent::BodyType::Static},
		{"Dynamic", RigidBody2DComponent::BodyType::Dynamic},
		{"Kinematic", RigidBody2DComponent::BodyType::Kinematic},
	};
}
