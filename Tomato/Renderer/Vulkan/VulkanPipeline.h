#pragma once

#include "Tomato/Renderer/Pipeline.h"
#include <vulkan/vulkan.h>

#include "VulkanShader.h"
#include "Tomato/Core/Macro.h"

namespace Tomato
{
	class UniformBuffer;

	class VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline(const PipelineProps& spec);
		virtual ~VulkanPipeline();

		virtual PipelineProps& GetSpecification() { return m_Specification; }
		virtual const PipelineProps& GetSpecification() const { return m_Specification; }

		virtual void Init() override;
		virtual void SetUniformBuffer(Ref<UniformBuffer> uniformBuffer, uint32_t binding, uint32_t set = 0);

		virtual void Bind() override;

		VkPipeline GetVulkanPipeline() { return m_VulkanPipeline; }
		VkPipelineLayout GetVulkanPipelineLayout() { return m_PipelineLayout; }
		VkDescriptorSet GetDescriptorSet(uint32_t set = 0)
		{
			ASSERT(m_DescriptorSets.DescriptorSets.size() > set);
			return m_DescriptorSets.DescriptorSets[set];
		}

		const std::vector<VkDescriptorSet>& GetDescriptorSets() const { return m_DescriptorSets.DescriptorSets; }

		void RT_SetUniformBuffer(Ref<UniformBuffer> uniformBuffer, uint32_t binding, uint32_t set = 0);
	private:
		PipelineProps m_Specification;

		VkPipelineLayout m_PipelineLayout = nullptr;
		VkPipeline m_VulkanPipeline = nullptr;
		VkPipelineCache m_PipelineCache = nullptr;
		VulkanShader::ShaderMaterialDescriptorSet m_DescriptorSets;
	};
}
