#pragma once

#include "Vulkan.h"
#include "Tomato/Renderer/Pipeline.h"

namespace Tomato
{
	class VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline(const PipelineInfo& props);
		void Init() override;
		const PipelineInfo& GetProps() override { return props; };

		void Bind() override;
		PipelineInfo props;
		vk::raii::Pipeline graphicsPipeline = nullptr;
		vk::raii::PipelineLayout pipelineLayout = nullptr;
		vk::raii::DescriptorPool descriptorPool = nullptr;
		std::vector<vk::raii::DescriptorSet> descriptorSet{};
	};
}
