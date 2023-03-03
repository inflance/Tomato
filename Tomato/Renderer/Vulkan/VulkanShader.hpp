#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

#include "Tomato/Renderer/Shader.hpp"
#include "Vulkan.hpp"
#include <ranges>

#include "VulkanContext.hpp"

namespace Tomato
{
	class VulkanDevice;

	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const ShaderCreateInfo& createInfo);

		~VulkanShader() override;

		void Bind() const override;
		void UnBind() const override;

		[[nodiscard]] const std::string& GetName() const override { return m_create_info.Name; }

		void SetInt(const std::string& name, int value) override;
		void SetIntArray(const std::string& name, int* values, uint32_t count) override;

		void SetMat3(const std::string& name, const glm::mat3& value) override;
		void SetMat4(const std::string& name, const glm::mat4& value) override;

		void SetFloat1(const std::string& name, float value) override;
		void SetFloat2(const std::string& name, const glm::vec2& value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;

		[[nodiscard]] bool HasShaderModule(ShaderType type) const { return m_data_map.contains(type); }
		[[nodiscard]] bool HasShader(ShaderType type) const { return m_data_map.contains(type); }

		vk::raii::ShaderModule GetShaderModule(const vk::raii::Device& device, ShaderType type)
		{
			if (HasShaderModule(type))
				return device.createShaderModule(vk::ShaderModuleCreateInfo(
					vk::ShaderModuleCreateFlags(), m_data_map[type].Data.size() * sizeof(uint32_t),
					m_data_map[type].Data.data()));
			return nullptr;
		}

		const auto& CreateShaderModule()
		{
			const auto& device = VulkanContext::Get().device;
			for (auto& res : m_data_map | std::views::values)
			{
				m_modules.try_emplace(res.Type, GetShaderModule(device, res.Type));
			}
			return m_modules;
		}

		void DestroyShaderModule()
		{
			m_modules.clear();
		}

	private:
		void CompileGLSLToSPIRV();

	private:
		ShaderCreateInfo m_create_info;
		std::unordered_map<ShaderType, ShaderResource> m_data_map;
		std::map<ShaderType, vk::raii::ShaderModule> m_modules{};
	};
}
