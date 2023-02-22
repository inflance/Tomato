#include "VulkanShader.h"

#include <shaderc/shaderc.hpp>
#include <vulkan/vulkan_core.h>
#include <spirv_cross/spirv_cross.hpp>

#include "Tomato/Core/Log.h"
#include "Tomato/Function/Loder.h"
#include "Tomato/Core/FileSystem.h"
#include "Tomato/Core/Timer.h"

namespace Tomato
{
	constexpr size_t HashString(std::string_view string)
	{
		size_t code{0};
		for (auto& ch : string)
			code = (code << 6) ^ (code >> 26) ^ ch;
		return code;
	}

	constexpr std::string_view GetShaderSPIRVDirectory()
	{
		return "PreCompile/Shader/Spv";
	}

	inline ShaderType FileExtensionToShaderType(std::string_view extension)
	{
		switch (HashString(extension))
		{
		case HashString(".vert"): return ShaderType::Vertex;
		case HashString(".frag"): return ShaderType::Fragment;
		case HashString(".geom"): return ShaderType::Geometry;
		case HashString(".comp"): return ShaderType::Compute;
		case HashString(".tesc"): return ShaderType::TesselationControl;
		case HashString(".tese"): return ShaderType::TesselationEvaluation;
		default: break;
		}
		return ShaderType::None;
	}

	inline std::string ShaderTypeToFileExtension(ShaderType type)
	{
		switch (type)
		{
		case ShaderType::Vertex: return ".vert";
		case ShaderType::Fragment: return ".frag";
		case ShaderType::Geometry: return ".geom";
		case ShaderType::Compute: return ".comp";
		case ShaderType::TesselationControl: return ".tesc";
		case ShaderType::TesselationEvaluation: return ".tese";
		default: break;
		}
		return {};
	}

	inline std::string ShaderTypeToString(ShaderType type)
	{
		switch (type)
		{
		case ShaderType::Vertex: return "Vertex";
		case ShaderType::Fragment: return "Fragment";
		case ShaderType::Geometry: return "Geometry";
		case ShaderType::Compute: return "Compute";
		case ShaderType::TesselationControl: return "Tesselation Control";
		case ShaderType::TesselationEvaluation: return "Tesselation Evaluation";
		default: break;
		}
		return {};
	}

	inline std::optional<shaderc_shader_kind> ShaderTypeToShaderC(ShaderType type)
	{
		switch (type)
		{
		case ShaderType::Vertex: return shaderc_glsl_vertex_shader;
		case ShaderType::Fragment: return shaderc_glsl_fragment_shader;
		case ShaderType::Geometry: return shaderc_glsl_geometry_shader;
		case ShaderType::Compute: return shaderc_glsl_compute_shader;
		case ShaderType::TesselationControl: return shaderc_glsl_tess_control_shader;
		case ShaderType::TesselationEvaluation: return shaderc_glsl_tess_evaluation_shader;
		default: break;
		}
		LOG_ASSERT(false, "This type is not supported yet");
		return {};
	}

	inline std::optional<VkShaderStageFlagBits> ShaderTypeToVulkanStageFlagBits(ShaderType type)
	{
		switch (type)
		{
		case ShaderType::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
		case ShaderType::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
		case ShaderType::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
		case ShaderType::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
		case ShaderType::TesselationControl: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		case ShaderType::TesselationEvaluation: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		default: break;
		}
		LOG_ASSERT(false, "This type is not supported yet");
		return {};
	}



	VulkanShader::VulkanShader(const ShaderCreateInfo& createInfo)
		: m_create_info(createInfo)
	{
		for (auto& res : m_create_info.Resource)
		{
			if (res.Flag == ShaderCreateFlag::ByCode)
			{
				if (!HasShader(res.Type))
					m_data_map[res.Type] = res;
			}
			else if (res.Flag == ShaderCreateFlag::ByPath)
			{
				auto type = static_cast<uint32_t>(FileExtensionToShaderType(res.Path.extension().string()));
				if(res.Type != ShaderType::None)
				{
					type = static_cast<uint32_t>(res.Type);
				}
				if (type == static_cast<uint32_t>(ShaderType::Vertex)) //default use vertex shader paath name;
					m_create_info.Name = FileSystem::GetFileName(res.Path);
				if (m_data_map.contains(res.Type))
				{
					LOG_ERROR("The type of {0} shader already exists",
					          ShaderTypeToString(m_create_info.Resource[type].Type));
				}
				else
				{
					m_create_info.Resource[type].Code = Loader::Loader<std::string>(res.Path);
				}
				m_data_map[res.Type] = res;
			}
			else if (res.Flag == ShaderCreateFlag::ByData)
			{
				return;
			}
		}
		CompileGLSLToSPIRV();
	}

	void VulkanShader::Bind() const
	{
	}

	void VulkanShader::UnBind() const
	{
	}

	void VulkanShader::SetInt(const std::string& name, int value)
	{
	}

	void VulkanShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
	}

	void VulkanShader::SetMat3(const std::string& name, const glm::mat3& value)
	{
	}

	void VulkanShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
	}

	void VulkanShader::SetFloat1(const std::string& name, float value)
	{
	}

	void VulkanShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
	}

	void VulkanShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
	}

	void VulkanShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
	}

	void VulkanShader::CompileGLSLToSPIRV()
	{
		//Timer timer("CompileGLSLToSPIRV");
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

		const std::filesystem::path spv_dir = FileSystem::GetFullPath(std::string(GetShaderSPIRVDirectory()));

		//If this SPIR-v directory does not exist, create
		if (FileSystem::CreateDirections(spv_dir))
		{
		}
		//For each path, compile it into a SPIR - V binary
		for (auto& resource : m_data_map)
		{
			if (!resource.second.Code.empty())
			{
				Timer timer;
				std::filesystem::path path = resource.second.Path;
				std::filesystem::path spv_path = spv_dir / (m_create_info.Name +
					ShaderTypeToFileExtension(resource.second.Type) + std::string(".spv"));

				shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
					resource.second.Code, ShaderTypeToShaderC(resource.second.Type).value(),
					resource.second.Path.generic_string().c_str(),
					options);
				if (result.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					assert(false, "");
				}
				resource.second.Data = std::vector<uint32_t>{result.cbegin(), result.cend()};

				Loader::UnLoader(spv_path, resource.second.Data, sizeof(uint32_t));
				LOG_WARN("{} shader {} compile for {} ms", ShaderTypeToString(resource.second.Type), m_create_info.Name,
				         timer.CountTime());
			}
		}
	}
}
