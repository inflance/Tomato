#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "SceneCamera.h"
#include "Tomato/EditorCamera.h"
#include "ScriptableGO.h"

namespace Tomato {

	struct ComponentBase
	{
		ComponentBase() = default;
		virtual ~ComponentBase() = default;
	};

	struct TransformComponent : ComponentBase
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		TransformComponent() = default;
		TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
			:Position(position), Rotation(rotation), Scale(scale)
		{
		}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

	};

	struct CameraComponent : ComponentBase
	{
		SceneCamera Camera;

		bool IsMain = true;
		bool IsResize = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) = default;

	};

	struct NameComponent : ComponentBase
	{
		std::string Name = std::string();

		NameComponent() = default;
		NameComponent(const std::string& name)
			:Name(name)
		{
		}
		NameComponent(const NameComponent& other) = default;
	};

	struct ColorComponent : ComponentBase
	{
		glm::vec4 Color{1.0f};

		ColorComponent() = default;
		ColorComponent(const glm::vec4& color)
			:Color(color)
		{
		}
		ColorComponent(const ColorComponent& other) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableGO* Instance = nullptr;

		ScriptableGO* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableGO*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nativeScriptComponent) { delete nativeScriptComponent->Instance; nativeScriptComponent->Instance = nullptr; };
		}
	};

}