#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Tomato/Scene/Light.h"
#include "Tomato/Scene/ScriptableEntity.h"
#include "Tomato/Scene/SceneCamera.h"
#include "Tomato/Core/Math.h"
#include "Tomato/Function/Camera/EditorCamera.h"
#include "Tomato/Renderer/Texture.h"


namespace Tomato
{
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

	struct SpriteComponent
	{
		glm::vec4 color_{1.0f};
		float tiling_factor_ = 1.0f;
		//white texture
		std::shared_ptr<Texture2D> texture_ = Texture2D::Create(std::string());

		SpriteComponent() = default;

		SpriteComponent(const glm::vec4& color)
			: color_(color)
		{
		}

		SpriteComponent(const SpriteComponent& other) = default;
	};


	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template <typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nativeScriptComponent)
			{
				delete nativeScriptComponent->Instance;
				nativeScriptComponent->Instance = nullptr;
			};
		}
	};

	struct LightComponent
	{
		Light light_{};

		LightComponent() = default;

		LightComponent(LightType lightType, float intensity, const glm::vec4& color, const glm::vec3& direction)
		{
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
