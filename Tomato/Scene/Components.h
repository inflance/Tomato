#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Light.h"
#include "ScriptableEntity.h"
#include "SceneCamera.h"
#include "Tomato/Core/Math.h"
#include "Tomato/Function/Camera/EditorCamera.h"
#include "Tomato/Renderer/Texture.h"
#include "Tomato/Renderer/Mesh.h"
#include "Tomato/Renderer/BaseShape.h"
#include "Materiral.h"

namespace Tomato {

	struct TransformComponent
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		TransformComponent() = default;
		TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
			:Position(position), Rotation(rotation), Scale(scale) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		void SetTransform(const glm::mat4& transform)
		{
			Math::DecomposeTransform(transform, Position, Rotation, Scale);
		}
	};

	struct CameraComponent
	{
		SceneCamera Camera;

		bool IsMain = true;
		bool IsResize = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) = default;
	};

	struct NameComponent
	{
		std::string Name = std::string();

		NameComponent() = default;
		NameComponent(const std::string& name)
			:Name(name) {}
		NameComponent(const NameComponent& other) = default;
	};

	struct SpriteComponent
	{
		glm::vec4 Color{ 1.0f };
		//white texture
		std::shared_ptr<Texture2D> Texture = Texture2D::Create(std::string());
		float TilingFactor = 1.0f;

		SpriteComponent() = default;
		SpriteComponent(const glm::vec4& color)
			:Color(color) {}
		SpriteComponent(const SpriteComponent& other) = default;
	};

	struct MatirialComponent
	{
		Matirial matiral;

		MatirialComponent() = default;
		MatirialComponent(const MatirialComponent& other) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nativeScriptComponent) { delete nativeScriptComponent->Instance; nativeScriptComponent->Instance = nullptr; };
		}
	};

	struct LightComponent {
		Light Light;

		LightComponent() = default;
		LightComponent(LightType lightType, float intensity, const glm::vec4& color, const glm::vec3& direction)
			:Light(lightType, intensity, color, direction) {}
	};

	struct StaticMeshComponent {
		Mesh StaticMesh;

		StaticMeshComponent() = default;
		StaticMeshComponent(StaticMeshComponent& other)
			:StaticMesh(other.StaticMesh) {}
	};

	struct BaseShapeComponent {
		BaseShape BaseShape;

		BaseShapeComponent() = default;
		BaseShapeComponent(int entityID)
		{
			BaseShape.CreateCube(entityID);
		}
		BaseShapeComponent(BaseShapeComponent& other)
			:BaseShape(other.BaseShape) {}
	};
}