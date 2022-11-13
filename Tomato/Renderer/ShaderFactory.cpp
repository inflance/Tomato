#include "Tomato/tmtpch.h"

#include "Tomato/Core.h"

#include "ShaderFactory.h"


namespace Tomato {


	void ShaderFactory::Add(const std::string& name, const std::shared_ptr<Shader>& shader)
	{
		LOG_ASSERT(!Exists(name), "Shader “—¥Ê‘⁄");
		m_shader[name] = shader;
	}

	void ShaderFactory::Add(const std::shared_ptr<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	std::shared_ptr<Tomato::Shader> ShaderFactory::Load(const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	std::shared_ptr<Tomato::Shader> ShaderFactory::Load(const std::string& name, const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	std::shared_ptr<Tomato::Shader> ShaderFactory::GetShader(const std::string& name)
	{
		LOG_ASSERT(Exists(name), "Shader Œ¥’“µΩ");
		return m_shader[name];
	}

	bool ShaderFactory::Exists(const std::string& name)
	{
		return m_shader.find(name) != m_shader.end();
	}

}