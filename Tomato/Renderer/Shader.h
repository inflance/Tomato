#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Tomato {

	class Shader
	{
	public:
		//Shader();
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;

		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;


		virtual void SetFloat1(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual const std::string& GetName() const = 0;

		static std::shared_ptr<Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath);
		static std::shared_ptr<Shader> Shader::Create(const std::string& filePath);
	};
}