#pragma once

#include "Tomato/Renderer/Shader.h"
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Tomato {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath);
		OpenGLShader(const std::string& filePath);

		virtual ~OpenGLShader() = default;

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual const std::string& GetName() const override { return m_name; };

		virtual void SetInt(const std::string& name, int value);
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;

		virtual void SetMat3(const std::string& name, const glm::mat3& value);
		virtual void SetMat4(const std::string& name, const glm::mat4& value);


		virtual void SetFloat1(const std::string& name,  float value);
		virtual void SetFloat2(const std::string& name, const glm::vec2& value);
		virtual void SetFloat3(const std::string& name, const glm::vec3& value);
		virtual void SetFloat4(const std::string& name, const glm::vec4& value);

	private:
		//检查shader编译或者连接错误
		void CheckCompileErrors(unsigned int shader, std::string type);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSource);
		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
		std::string GetFileName(const std::string& filePath);
	private:
		uint32_t m_shader_id;
		std::string m_name;
	};
}