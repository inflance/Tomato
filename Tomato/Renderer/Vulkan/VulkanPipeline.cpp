#include "VulkanPipeline.h"

#include "VulkanShader.h"

#include "VulkanContext.h"
#include "VulkanUniformBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanFrameBuffer.h"

namespace Tomato
{
	namespace Utils
	{
		constexpr VkPrimitiveTopology GetVulkanTopology(PrimitiveType type)
		{
			switch (type)
			{
			case PrimitiveType::Points: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case PrimitiveType::Lines: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case PrimitiveType::Triangles: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case PrimitiveType::LineStrip: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case PrimitiveType::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			case PrimitiveType::TriangleFan: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
			}

			ASSERT(false, "Unknown toplogy");
			return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
		}

		constexpr VkCompareOp GetVulkanCompareOperator(const DepthCompareOperator compareOp)
		{
			switch (compareOp)
			{
			case DepthCompareOperator::Never: return VK_COMPARE_OP_NEVER;
			case DepthCompareOperator::NotEqual: return VK_COMPARE_OP_NOT_EQUAL;
			case DepthCompareOperator::Less: return VK_COMPARE_OP_LESS;
			case DepthCompareOperator::LessOrEqual: return VK_COMPARE_OP_LESS_OR_EQUAL;
			case DepthCompareOperator::Greater: return VK_COMPARE_OP_GREATER;
			case DepthCompareOperator::GreaterOrEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case DepthCompareOperator::Equal: return VK_COMPARE_OP_EQUAL;
			case DepthCompareOperator::Always: return VK_COMPARE_OP_ALWAYS;
			}
			ASSERT(false, "Unknown Operator");
			return VK_COMPARE_OP_MAX_ENUM;
		}
	}

	static VkFormat ShaderDataTypeToVulkanFormat(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return VK_FORMAT_R32_SFLOAT;
		case ShaderDataType::Float2: return VK_FORMAT_R32G32_SFLOAT;
		case ShaderDataType::Float3: return VK_FORMAT_R32G32B32_SFLOAT;
		case ShaderDataType::Float4: return VK_FORMAT_R32G32B32A32_SFLOAT;
		}
		ASSERT(false);
		return VK_FORMAT_UNDEFINED;
	}

	VulkanPipeline::VulkanPipeline(const PipelineProps& spec)
		: m_Specification(spec)
	{
		ASSERT(spec.Shader);
		ASSERT(spec.RenderPass);
		Init();
		Renderer::RegisterShaderDependency(spec.Shader, this);
	}

	VulkanPipeline::~VulkanPipeline()
	{
		Renderer::SubmitAndFree(
			[pipeline = m_VulkanPipeline, pipelineCache = m_PipelineCache, pipelineLayout = m_PipelineLayout]()
			{
				const auto vulkanDevice = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
				vkDestroyPipeline(vulkanDevice, pipeline, nullptr);
				vkDestroyPipelineCache(vulkanDevice, pipelineCache, nullptr);
				vkDestroyPipelineLayout(vulkanDevice, pipelineLayout, nullptr);
			});
	}

	void VulkanPipeline::Init()
	{
		VulkanPipeline* instance;
		Renderer::Submit([instance]() mutable
		{
			// HZ_CORE_WARN("[VulkanPipeline] Creating pipeline {0}", instance->m_Specification.DebugName);

			VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
			ASSERT(instance->m_Specification.Shader);
			auto vulkanShader = As<VulkanShader>(instance->m_Specification.Shader);
			Ref<VulkanFramebuffer> framebuffer = As<VulkanFramebuffer>(instance->m_Specification.RenderPass->GetSpecification().TargetFramebuffer);

			auto descriptorSetLayouts = vulkanShader->GetAllDescriptorSetLayouts();

			const auto& pushConstantRanges = vulkanShader->GetPushConstantRanges();

			// TODO: should come from shader
			std::vector<VkPushConstantRange> vulkanPushConstantRanges(pushConstantRanges.size());
			for (uint32_t i = 0; i < pushConstantRanges.size(); i++)
			{
				const auto& pushConstantRange = pushConstantRanges[i];
				auto& vulkanPushConstantRange = vulkanPushConstantRanges[i];

				vulkanPushConstantRange.stageFlags = pushConstantRange.stageFlags;
				vulkanPushConstantRange.offset = pushConstantRange.offset;
				vulkanPushConstantRange.size = pushConstantRange.size;
			}

			// Create the pipeline layout that is used to generate the rendering pipelines that are based on this descriptor set layout
			// In a more complex scenario you would have different pipeline layouts for different descriptor set layouts that could be reused
			VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
			pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pPipelineLayoutCreateInfo.pNext = nullptr;
			pPipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
			pPipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
			pPipelineLayoutCreateInfo.pushConstantRangeCount = static_cast<uint32_t>(vulkanPushConstantRanges.size());
			pPipelineLayoutCreateInfo.pPushConstantRanges = vulkanPushConstantRanges.data();

			VK_CHECK_RESULT(
				vkCreatePipelineLayout(device, &pPipelineLayoutCreateInfo, nullptr, &instance->m_PipelineLayout),"");

			// Create the graphics pipeline used in this example
			// Vulkan uses the concept of rendering pipelines to encapsulate fixed states, replacing OpenGL's complex state machine
			// A pipeline is then stored and hashed on the GPU making pipeline changes very fast
			// Note: There are still a few dynamic states that are not directly part of the pipeline (but the info that they are used is)

			VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
			pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			// The layout used for this pipeline (can be shared among multiple pipelines using the same layout)
			pipelineCreateInfo.layout = instance->m_PipelineLayout;
			// Renderpass this pipeline is attached to
			pipelineCreateInfo.renderPass = framebuffer->GetRenderPass();

			// Construct the differnent states making up the pipeline

			// Input assembly state describes how primitives are assembled
			// This pipeline will assemble vertex data as a triangle lists (though we only use one triangle)
			VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
			inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssemblyState.topology = Utils::GetVulkanTopology(instance->m_Specification.Type);

			// Rasterization state
			VkPipelineRasterizationStateCreateInfo rasterizationState = {};
			rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizationState.polygonMode = instance->m_Specification.Wireframe
				                                 ? VK_POLYGON_MODE_LINE
				                                 : VK_POLYGON_MODE_FILL;
			rasterizationState.cullMode = instance->m_Specification.BackfaceCulling
				                              ? VK_CULL_MODE_BACK_BIT
				                              : VK_CULL_MODE_NONE;
			rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
			rasterizationState.depthClampEnable = VK_FALSE;
			rasterizationState.rasterizerDiscardEnable = VK_FALSE;
			rasterizationState.depthBiasEnable = VK_FALSE;
			rasterizationState.lineWidth = instance->m_Specification.LineWidth; // this is dynamic

			// Color blend state describes how blend factors are calculated (if used)
			// We need one blend attachment state per color attachment (even if blending is not used)
			size_t colorAttachmentCount = framebuffer->GetSpecification().SwapChainTarget
				                              ? 1
				                              : framebuffer->GetColorAttachmentCount();
			std::vector<VkPipelineColorBlendAttachmentState> blendAttachmentStates(colorAttachmentCount);
			if (framebuffer->GetSpecification().SwapChainTarget)
			{
				blendAttachmentStates[0].colorWriteMask = 0xf;
				blendAttachmentStates[0].blendEnable = VK_TRUE;
				blendAttachmentStates[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
				blendAttachmentStates[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
				blendAttachmentStates[0].colorBlendOp = VK_BLEND_OP_ADD;
				blendAttachmentStates[0].alphaBlendOp = VK_BLEND_OP_ADD;
				blendAttachmentStates[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
				blendAttachmentStates[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			}
			else
			{
				for (size_t i = 0; i < colorAttachmentCount; i++)
				{
					if (!framebuffer->GetSpecification().Blend)
						break;

					blendAttachmentStates[i].colorWriteMask = 0xf;
					if (!framebuffer->GetSpecification().Blend)
						break;

					const auto& attachmentSpec = framebuffer->GetSpecification().Attachments.Attachments[i];
					FramebufferBlendMode blendMode = framebuffer->GetSpecification().BlendMode ==
					                                 FramebufferBlendMode::None
						                                 ? attachmentSpec.BlendMode
						                                 : framebuffer->GetSpecification().BlendMode;

					blendAttachmentStates[i].blendEnable = attachmentSpec.Blend ? VK_TRUE : VK_FALSE;

					blendAttachmentStates[i].colorBlendOp = VK_BLEND_OP_ADD;
					blendAttachmentStates[i].alphaBlendOp = VK_BLEND_OP_ADD;
					blendAttachmentStates[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
					blendAttachmentStates[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

					switch (blendMode)
					{
					case FramebufferBlendMode::SrcAlphaOneMinusSrcAlpha:
						blendAttachmentStates[i].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
						blendAttachmentStates[i].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
						blendAttachmentStates[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
						blendAttachmentStates[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
						break;
					case FramebufferBlendMode::OneZero:
						blendAttachmentStates[i].srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
						blendAttachmentStates[i].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
						break;
					case FramebufferBlendMode::Zero_SrcColor:
						blendAttachmentStates[i].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
						blendAttachmentStates[i].dstColorBlendFactor = VK_BLEND_FACTOR_SRC_COLOR;
						break;

					default:
						ASSERT(false);
					}
				}
			}

			VkPipelineColorBlendStateCreateInfo colorBlendState = {};
			colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlendState.attachmentCount = static_cast<uint32_t>(blendAttachmentStates.size());
			colorBlendState.pAttachments = blendAttachmentStates.data();

			// Viewport state sets the number of viewports and scissor used in this pipeline
			// Note: This is actually overriden by the dynamic states (see below)
			VkPipelineViewportStateCreateInfo viewportState = {};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.scissorCount = 1;

			// Enable dynamic states
			// Most states are baked into the pipeline, but there are still a few dynamic states that can be changed within a command buffer
			// To be able to change these we need do specify which dynamic states will be changed using this pipeline. Their actual states are set later on in the command buffer.
			// For this example we will set the viewport and scissor using dynamic states
			std::vector<VkDynamicState> dynamicStateEnables;
			dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);
			dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);
			if (instance->m_Specification.Type == PrimitiveType::Lines || instance->m_Specification.Type ==
				PrimitiveType::LineStrip || instance->m_Specification.Wireframe)
				dynamicStateEnables.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);

			VkPipelineDynamicStateCreateInfo dynamicState = {};
			dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicState.pDynamicStates = dynamicStateEnables.data();
			dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());

			// Depth and stencil state containing depth and stencil compare and test operations
			// We only use depth tests and want depth tests and writes to be enabled and compare with less or equal
			VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
			depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depthStencilState.depthTestEnable = instance->m_Specification.DepthTest ? VK_TRUE : VK_FALSE;
			depthStencilState.depthWriteEnable = instance->m_Specification.DepthWrite ? VK_TRUE : VK_FALSE;
			depthStencilState.depthCompareOp = Utils::GetVulkanCompareOperator(instance->m_Specification.DepthOperator);
			depthStencilState.depthBoundsTestEnable = VK_FALSE;
			depthStencilState.back.failOp = VK_STENCIL_OP_KEEP;
			depthStencilState.back.passOp = VK_STENCIL_OP_KEEP;
			depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;
			depthStencilState.stencilTestEnable = VK_FALSE;
			depthStencilState.front = depthStencilState.back;

			// Multi sampling state
			// This example does not make use fo multi sampling (for anti-aliasing), the state must still be set and passed to the pipeline
			VkPipelineMultisampleStateCreateInfo multisampleState = {};
			multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			multisampleState.pSampleMask = nullptr;

			// Vertex input descriptor
			BufferLayout& layout = instance->m_Specification.Layout;
			BufferLayout& instanceLayout = instance->m_Specification.InstanceLayout;

			std::vector<VkVertexInputBindingDescription> vertexInputBindingDescriptions;

			VkVertexInputBindingDescription& vertexInputBinding = vertexInputBindingDescriptions.emplace_back();
			vertexInputBinding.binding = 0;
			vertexInputBinding.stride = layout.GetStride();
			vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			if (instanceLayout.GetElementCount())
			{
				VkVertexInputBindingDescription& instanceInputBinding = vertexInputBindingDescriptions.emplace_back();
				instanceInputBinding.binding = 1;
				instanceInputBinding.stride = instanceLayout.GetStride();
				instanceInputBinding.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
			}

			// Inpute attribute bindings describe shader attribute locations and memory layouts
			std::vector<VkVertexInputAttributeDescription> vertexInputAttributes(
				layout.GetElementCount() + instanceLayout.GetElementCount());

			uint32_t location = 0;
			for (auto element : layout)
			{
				vertexInputAttributes[location].binding = 0;
				vertexInputAttributes[location].location = location;
				vertexInputAttributes[location].format = ShaderDataTypeToVulkanFormat(element.Type);
				vertexInputAttributes[location].offset = element.Offset;
				location++;
			}

			for (auto element : instanceLayout)
			{
				vertexInputAttributes[location].binding = 1;
				vertexInputAttributes[location].location = location;
				vertexInputAttributes[location].format = ShaderDataTypeToVulkanFormat(element.Type);
				vertexInputAttributes[location].offset = element.Offset;
				location++;
			}

			// Vertex input state used for pipeline creation
			VkPipelineVertexInputStateCreateInfo vertexInputState = {};
			vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputState.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInputBindingDescriptions.
				size());
			vertexInputState.pVertexBindingDescriptions = vertexInputBindingDescriptions.data();
			vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributes.size());
			vertexInputState.pVertexAttributeDescriptions = vertexInputAttributes.data();

			const auto& shaderStages = vulkanShader->GetStageInfos();

			// Set pipeline shader stage info
			pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
			pipelineCreateInfo.pStages = shaderStages.data();

			// Assign the pipeline states to the pipeline creation info structure
			pipelineCreateInfo.pVertexInputState = &vertexInputState;
			pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
			pipelineCreateInfo.pRasterizationState = &rasterizationState;
			pipelineCreateInfo.pColorBlendState = &colorBlendState;
			pipelineCreateInfo.pMultisampleState = &multisampleState;
			pipelineCreateInfo.pViewportState = &viewportState;
			pipelineCreateInfo.pDepthStencilState = &depthStencilState;
			pipelineCreateInfo.renderPass = framebuffer->GetRenderPass();
			pipelineCreateInfo.pDynamicState = &dynamicState;

			// NOTE(Yan): we can use pipeline caches to serialize pipelines to disk for faster loading
			VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
			pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
			VK_CHECK_RESULT(
				vkCreatePipelineCache(device, &pipelineCacheCreateInfo, nullptr, &instance->m_PipelineCache),"");

			// Create rendering pipeline using the specified states
			VK_CHECK_RESULT(
				vkCreateGraphicsPipelines(device, instance->m_PipelineCache, 1, &pipelineCreateInfo, nullptr, &instance
					->m_VulkanPipeline),"");
			Utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_PIPELINE, instance->m_Specification.DebugName,
			                               instance->m_VulkanPipeline);

			// Shader modules are no longer needed once the graphics pipeline has been created
			// vkDestroyShaderModule(device, shaderStages[0].module, nullptr);
			// vkDestroyShaderModule(device, shaderStages[1].module, nullptr);

			// instance->m_DescriptorSets = vulkanShader->AllocateDescriptorSets();
		});
	}

	void VulkanPipeline::SetUniformBuffer(Ref<UniformBuffer> uniformBuffer, uint32_t binding, uint32_t set)
	{
		VulkanPipeline* instance = this;
		Renderer::Submit([instance, uniformBuffer, binding, set]() mutable
		{
			instance->RT_SetUniformBuffer(uniformBuffer, binding, set);
		});
	}

	void VulkanPipeline::Bind()
	{
	}

	void VulkanPipeline::RT_SetUniformBuffer(Ref<UniformBuffer> uniformBuffer, uint32_t binding, uint32_t set)
	{
		auto vulkanShader = As<VulkanShader>(m_Specification.Shader);
		Ref<VulkanUniformBuffer> vulkanUniformBuffer = As<VulkanUniformBuffer>(uniformBuffer);

		ASSERT(m_DescriptorSets.DescriptorSets.size() > set);

		VkWriteDescriptorSet writeDescriptorSet = {};
		writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet.descriptorCount = 1;
		writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeDescriptorSet.pBufferInfo = &vulkanUniformBuffer->GetDescriptorBufferInfo();
		writeDescriptorSet.dstBinding = binding;
		writeDescriptorSet.dstSet = m_DescriptorSets.DescriptorSets[set];

		LOG_WARN("VulkanPipeline - Updating descriptor set (VulkanPipeline::SetUniformBuffer)");
		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
		vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);
	}
}
