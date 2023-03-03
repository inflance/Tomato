#pragma once

#include <glm/glm.hpp>
#include <filesystem>

namespace Tomato
{
	enum class ShaderType : int32_t
	{
		None = -1,
		Vertex = 0,
		Fragment,
		Geometry,
		Compute,
		TesselationControl,
		TesselationEvaluation
	};

	enum class ShaderCreateFlag
	{
		ByPath,
		ByCode,
		ByData
	};

	struct ShaderResource
	{
		ShaderType Type = ShaderType::None;
		ShaderCreateFlag Flag = ShaderCreateFlag::ByCode;
		std::filesystem::path Path;
		std::string Code;
		std::vector<uint32_t> Data;
	};

	struct ShaderCreateInfo
	{
		std::string Name;
		std::vector<ShaderResource> Resource;
	};

	class Shader
	{
	public:
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

		static std::shared_ptr<Shader> Create(const ShaderCreateInfo& createInfo);
	};
}
