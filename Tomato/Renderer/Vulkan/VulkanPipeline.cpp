#include "VulkanPipeline.hpp"

#include "VulkanContext.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanShader.hpp"
#include "VulkanTexture2D.hpp"
#include "VulkanUniformBuffer.hpp"
#include "VulkanVertexBuffer.hpp"

namespace Tomato
{
	namespace Utils
	{
		static inline auto ToVulkanDescriptorType(DescriptorType type)
		{
			switch (type)
			{
			case DescriptorType::StorageBuffer: return vk::DescriptorType::eStorageBuffer;
			case DescriptorType::UniformBuffer: return vk::DescriptorType::eUniformBuffer;
			case DescriptorType::CombinedImageSample: return vk::DescriptorType::eCombinedImageSampler;
			}
			ASSERT(false);
			return vk::DescriptorType::eUniformBuffer;
		}

		static inline auto ToVulkanShaderStageFlag(ShaderType flags)
		{
			switch (flags)
			{
			case ShaderType::Vertex: return vk::ShaderStageFlagBits::eVertex;
			case ShaderType::Fragment: return vk::ShaderStageFlagBits::eFragment;
			}
			ASSERT(false);
			return vk::ShaderStageFlagBits::eVertex;
		}

		static inline auto ToVulkanShaderStageFlag(ShaderStageFlags flags)
		{
			switch (flags)
			{
			case ShaderStageFlags::Vertex: return vk::ShaderStageFlagBits::eVertex;
			case ShaderStageFlags::Fragment: return vk::ShaderStageFlagBits::eFragment;
			}
			ASSERT(false);
			return vk::ShaderStageFlagBits::eVertex;
		}
	}

	VulkanPipeline::VulkanPipeline(const PipelineInfo& props)
		: props(props)
	{
		const auto& device = VulkanContext::Get().device;
		const auto& pipelineCache = VulkanContext::Get().pipelineCache;
		const auto& shader = As<VulkanShader>(props.shader_);

		const auto& uniformBufferSet = props.uniform_buffer_set_;
		const auto& uniformLayout = uniformBufferSet->GetUniformBufferSetLayout();
		std::vector<std::tuple<vk::DescriptorType, uint32_t, vk::ShaderStageFlags>> bindData;
		std::vector<Ref<VulkanUniformBuffer>> uniformBufferData;
		std::vector<Ref<VulkanTexture2D>> textureData;
		std::vector<vk::DescriptorPoolSize> poolSizes{};

		for (auto& layout : uniformLayout)
		{
			bindData.emplace_back(Utils::ToVulkanDescriptorType(layout.Type), layout.Count,
			                      Utils::ToVulkanShaderStageFlag(layout.Flag));

			if (layout.Type == DescriptorType::UniformBuffer)
			{
				uniformBufferData.emplace_back(
					As<VulkanUniformBuffer>(uniformBufferSet->GetUniformBuffer(layout.Binding, layout.Set)));

				poolSizes.emplace_back(Utils::ToVulkanDescriptorType(layout.Type), layout.Count);
			}
			else if (layout.Type == DescriptorType::CombinedImageSample)
			{
				textureData.emplace_back(As<VulkanTexture2D>(uniformBufferSet->GetTexture(layout.Binding, layout.Set)));
				poolSizes.emplace_back(Utils::ToVulkanDescriptorType(layout.Type), layout.Count);
			}
		}

		vk::raii::DescriptorSetLayout descriptorSetLayout = Utils::makeDescriptorSetLayout(device, bindData);

		pipelineLayout = vk::raii::PipelineLayout(device, {{}, *descriptorSetLayout});

		assert(!poolSizes.empty());
		uint32_t maxSets = std::accumulate(
			poolSizes.begin(), poolSizes.end(), 0, [](uint32_t sum, const vk::DescriptorPoolSize& dps)
			{
				return sum + dps.descriptorCount;
			});
		assert(0 < maxSets);

		vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
		                                                      maxSets, poolSizes);

		descriptorPool = vk::raii::DescriptorPool(device, descriptorPoolCreateInfo);


		descriptorSet = vk::raii::DescriptorSets(device, {*descriptorPool, *descriptorSetLayout});
		//descriptorSet.resize(2);
		std::vector<std::tuple<vk::DescriptorType, const vk::raii::Buffer&, vk::DeviceSize, const vk::raii::BufferView
		                       *>> bufferData;
		for (int i = 0; i < uniformBufferData.size(); i++)
		{
			bufferData.emplace_back(std::get<0>(bindData[i]), uniformBufferData[i]->uniformDataBuffer,
			                        VK_WHOLE_SIZE, nullptr);
		}
		std::vector<std::pair<const vk::raii::Sampler&, const vk::raii::ImageView&>> imageData;
		for (int i = 0; i < textureData.size(); i++)
		{
			LOG_INFO(textureData[i]->GetWidth());
			imageData.emplace_back(textureData[i]->sampler, textureData[i]->imageView);
		}

		for (auto& i : descriptorSet)
		{
			Utils::updateDescriptorSets(
				device, i, {
					bufferData
				}, imageData);
		}

		const auto& renderPass = As<VulkanRenderPass>(props.render_pass_)->renderPass;
		auto vi = VulkanVertexBuffer::GetVulkanVertexLayout(props.vertex_layout_);

		std::vector<vk::PipelineShaderStageCreateInfo> pipeline_shader_stage_create_infos;

		for (auto& shader_module : shader->CreateShaderModule())
		{
			vk::PipelineShaderStageCreateInfo cf;
			cf.setPName("main").setStage(Utils::ToVulkanShaderStageFlag(shader_module.first))
			  .setFlags({}).setModule(*shader_module.second).setPSpecializationInfo(nullptr);
			pipeline_shader_stage_create_infos.emplace_back(cf);
		}

		auto vertex_stride = props.vertex_layout_.GetStride();

		std::vector<vk::VertexInputAttributeDescription> vertex_attribute_descriptions;
		vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
		vk::VertexInputBindingDescription vertexInputBindingDescription(0, props.vertex_layout_.GetStride());

		if (0 < vertex_stride)
		{
			vertex_attribute_descriptions.reserve(vi.size());
			for (uint32_t i = 0; i < vi.size(); i++)
			{
				vertex_attribute_descriptions.emplace_back(i, 0, vi[i].first,
				                                           vi[i].second);
			}
			pipelineVertexInputStateCreateInfo.setVertexBindingDescriptions(vertexInputBindingDescription);
			pipelineVertexInputStateCreateInfo.setVertexAttributeDescriptions(vertex_attribute_descriptions);
		}

		vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(
			vk::PipelineInputAssemblyStateCreateFlags(),
			vk::PrimitiveTopology::eTriangleList);

		vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(
			vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr);

		vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(
			vk::PipelineRasterizationStateCreateFlags(),
			false,
			false,
			vk::PolygonMode::eFill,
			vk::CullModeFlagBits::eNone,
			vk::FrontFace::eClockwise,
			false,
			0.0f,
			0.0f,
			0.0f,
			1.0f);
		
		vk::PipelineMultisampleStateCreateInfo pipeline_multisample_state_create_info({}, vk::SampleCountFlagBits::e1);

		vk::StencilOpState stencilOpState(vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep,
		                                  vk::CompareOp::eAlways);
		vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(
			vk::PipelineDepthStencilStateCreateFlags(), true, true, vk::CompareOp::eLessOrEqual,
			false, false, stencilOpState, stencilOpState);

		//Set Blend
		/*
		 *	new_rgb = (src_factor * src_rgb) op (dst_factor  * dst_rgb)
		 *	new_a = (src_factor * src_a) op (dst_factor  * dst_a)
		 *
		 *	new_rgb = 1 * src_rgb + (1 - srcA) * dst_rgb
		 *	new_a = src_a === 1 * src_a + 0 * dst_a;
		 */
		//one way blend
		vk::ColorComponentFlags color_write_mask(
			vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB |
			vk::ColorComponentFlagBits::eA);

		vk::PipelineColorBlendAttachmentState color_blend_attachment_state;
		color_blend_attachment_state.setBlendEnable(true)
		.setSrcColorBlendFactor(vk::BlendFactor::eOne)
		.setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
		.setColorBlendOp(vk::BlendOp::eAdd)
		.setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
		.setDstAlphaBlendFactor(vk::BlendFactor::eZero)
		.setAlphaBlendOp(vk::BlendOp::eAdd)
		.setColorWriteMask(color_write_mask);

		//by bits
		vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(
			vk::PipelineColorBlendStateCreateFlags(), false, vk::LogicOp::eNoOp, color_blend_attachment_state,
			{{1.0f, 1.0f, 1.0f, 1.0f}});
		//Set Blend

		std::array<vk::DynamicState, 2> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
		vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(
			vk::PipelineDynamicStateCreateFlags(), dynamicStates);

		vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo(vk::PipelineCreateFlags(),
		                                                          pipeline_shader_stage_create_infos,
		                                                          &pipelineVertexInputStateCreateInfo,
		                                                          &pipelineInputAssemblyStateCreateInfo,
		                                                          nullptr,
		                                                          &pipelineViewportStateCreateInfo,
		                                                          &pipelineRasterizationStateCreateInfo,
		                                                          &pipeline_multisample_state_create_info,
		                                                          &pipelineDepthStencilStateCreateInfo,
		                                                          &pipelineColorBlendStateCreateInfo,
		                                                          &pipelineDynamicStateCreateInfo,
		                                                          *pipelineLayout,
		                                                          *renderPass);

		graphicsPipeline = vk::raii::Pipeline(device, pipelineCache, graphicsPipelineCreateInfo);
	}

	void VulkanPipeline::Init()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void VulkanPipeline::Bind()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}
}
