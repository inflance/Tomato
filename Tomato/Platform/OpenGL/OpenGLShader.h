#pragma once

#include <glm/glm.hpp>

#include "Tomato/Renderer/Shader.h"

namespace Tomato {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertex_path, const std::string& fragment_path);
		OpenGLShader(const std::string& file_path);

		~OpenGLShader() override{}

		void Bind() const override;
		void UnBind() const override;

		[[nodiscard]]const std::string& GetName() const override { return m_name; }

		void SetInt(const std::string& name, int value) override;
		void SetIntArray(const std::string& name, int* values, uint32_t count) override;

		void SetMat3(const std::string& name, const glm::mat3& value) override;
		void SetMat4(const std::string& name, const glm::mat4& value) override;

		void SetFloat1(const std::string& name,  float value) override;
		void SetFloat2(const std::string& name, const glm::vec2& value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;

	private:
		//检查shader编译或者连接错误
		void CheckCompileErrors(unsigned int shader, const std::string& type);
		void Compile(const std::unordered_map<unsigned int, std::string>& shader_map);
		std::string ReadFile(const std::string& path);
		std::unordered_map<unsigned int, std::string> Preprocess(const std::string& source);
		std::string GetFileName(const std::string& file_path);
	private:
		uint32_t m_shader_id;
		std::string m_name;
	};
}