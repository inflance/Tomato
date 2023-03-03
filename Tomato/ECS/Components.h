#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "ScriptableEntity.hpp"
#include "Tomato/ECS/Components/Light.hpp"
#include "Tomato/Core/Math.h"
#include "Tomato/Renderer/Texture.hpp"
#include "Tomato/ECS/Components/RigidBody2D.hpp"
#include "Tomato/ECS/Components/SpriteRenderer.hpp"
#include "Tomato/Renderer/Model.hpp"
#include "Tomato/Function/Camera/Camera.hpp"


namespace Tomato
{
	class ScriptableEntity;

	struct TransformComponent
	{
		glm::vec3 position_ = glm::vec3(0.0f);
		glm::vec3 rotation_ = glm::vec3(0.0f);
		glm::vec3 scale_ = glm::vec3(1.0f);

		TransformComponent() = default;

		TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
			: position_(position), rotation_(rotation), scale_(scale)
		{
		}

		[[nodiscard]] glm::mat4 GetTransform() const noexcept
		{
			const glm::mat4 rotation = toMat4(glm::quat(rotation_));

			return translate(glm::mat4(1.0f), position_)
				* rotation
				* scale(glm::mat4(1.0f), scale_);
		}

		void SetTransform(const glm::mat4& transform)
		{
			Math::DecomposeTransform(transform, position_, rotation_, scale_);
		}
	};

	struct CameraComponent
	{
		Camera camera_;

		bool is_main_camera_ = true;
		bool is_resize_ = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) = default;
	};

	struct TagComponent
	{
		std::string tag_ = std::string();

		TagComponent() = default;

		TagComponent(std::string tag)
			: tag_(std::move(tag))
		{
		}

		TagComponent(const TagComponent& other) = default;
	};


	struct NativeScriptComponent
	{
		ScriptableEntity* instance_ = nullptr;

		ScriptableEntity* (*instantiate_script_)();
		void (*destroy_script)(NativeScriptComponent*);

		NativeScriptComponent() = default;
		~NativeScriptComponent();

		template <typename T>
		void Bind()
		{
			instantiate_script_ = []() { return static_cast<ScriptableEntity*>(new T()); };
			if (!instance_) {
				instance_ = instantiate_script_();
				instance_->OnCreate();
			}
			destroy_script = [](NativeScriptComponent* native_script_component)
			{
				native_script_component->instance_->OnDestroy();
				delete native_script_component->instance_;
				native_script_component->instance_ = nullptr;
			};
		}
	};

	inline NativeScriptComponent::~NativeScriptComponent()
	{
		destroy_script(this);
	}


	template <typename T>
	struct ResetComponent
	{
		T component_;

		ResetComponent() = default;
		ResetComponent(const ResetComponent&) = default;

		ResetComponent(const T& component)
		{
			component_ = component;
		}
	};

#if 0

	struct MatirialComponent
	{
		Matirial matiral;

		MatirialComponent() = default;
		MatirialComponent(const MatirialComponent& other) = default;
	};
	struct StaticMeshComponent
	{
		Mesh StaticMesh;

		StaticMeshComponent() = default;

		StaticMeshComponent(StaticMeshComponent& other)
			: StaticMesh(other.StaticMesh)
		{
		}
	};

	struct BaseShapeComponent
	{
		BaseShape BaseShape;

		BaseShapeComponent() = default;

		BaseShapeComponent(int entityID)
		{
			BaseShape.CreateCube(entityID);
		}

		BaseShapeComponent(BaseShapeComponent& other)
			: BaseShape(other.BaseShape)
		{
		}
	};
#endif
}
