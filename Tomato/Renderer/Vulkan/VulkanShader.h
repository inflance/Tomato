#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

#include "Tomato/Renderer/Shader.h"

namespace Tomato
{
	class VulkanDevice;

	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path);
		VulkanShader(const std::initializer_list<std::filesystem::path>& file_paths);
		VulkanShader(const std::string& file_paths);

		~VulkanShader() override{}

		struct ShaderMaterialDescriptorSet
		{
			VkDescriptorPool Pool = nullptr;
			std::vector<VkDescriptorSet> DescriptorSets;
		};

		void Bind() const override;
		void UnBind() const override;

		[[nodiscard]] const std::string& GetName() const override { return m_name; }

		void SetInt(const std::string& name, int value) override;
		void SetIntArray(const std::string& name, int* values, uint32_t count) override;

		void SetMat3(const std::string& name, const glm::mat3& value) override;
		void SetMat4(const std::string& name, const glm::mat4& value) override;

		void SetFloat1(const std::string& name, float value) override;
		void SetFloat2(const std::string& name, const glm::vec2& value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;

		void CreatePipelineShaderModules(std::shared_ptr<VulkanDevice> device);
		void DestroyPipelineShaderModules();
		void CreatePipelineShaderStage();
		VkDescriptorSet GetDescriptorSet() { return m_DescriptorSet; }
		VkDescriptorSetLayout GetDescriptorSetLayout(uint32_t set) { return m_DescriptorSetLayouts.at(set); }
		std::vector<VkDescriptorSetLayout> GetAllDescriptorSetLayouts() { return m_DescriptorSetLayouts; };
		std::vector<VkPushConstantRange> GetPushConstantRanges() { return m_ranges; };

		std::vector<ShaderResource>& GetResources() { return m_shader_resources; }
		std::vector<VkPipelineShaderStageCreateInfo>& GetStageInfos() { return m_infos; }
		std::vector<VkShaderModule>& GetModules() { return m_modules; }

	private:
		void CompileGLSLToSPIRV();

	private:
		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
		VkDescriptorSet m_DescriptorSet;
		std::string m_name;
		std::vector<ShaderResource> m_shader_resources;
		std::vector<VkPipelineShaderStageCreateInfo> m_infos;
		std::vector<VkShaderModule > m_modules;
		std::shared_ptr<VulkanDevice> m_device;
		std::vector<VkPushConstantRange> m_ranges;
	};
}
