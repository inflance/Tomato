#pragma once

#include "Shader.h"

namespace Tomato {

	class ShaderFactory 
	{
	public:
		void Add(const std::shared_ptr<Shader>& shader);
		void Add(const std::string& name, const std::shared_ptr<Shader>& shader);

		std::shared_ptr<Shader> Load(const std::string& filePath);
		std::shared_ptr<Shader> Load(const std::string& name, const std::string& filePath);

		std::shared_ptr<Shader> GetShader(const std::string& name);
	
	private:
		bool Exists(const std::string& name);

	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_shader;
	};
}