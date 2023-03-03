#include "VulkanPipeline.hpp"

#include "VulkanContext.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanShader.hpp"
#include "VulkanTexture.hpp"
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

		vk::raii::ShaderModule vertexShaderModule = nullptr, fragmentShaderModule = nullptr;
		if (shader->HasShader(ShaderType::Vertex))
			vertexShaderModule = shader->GetShaderModule(device, ShaderType::Vertex);
		if (shader->HasShader(ShaderType::Fragment))
			fragmentShaderModule = shader->GetShaderModule(device, ShaderType::Fragment);


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

		for (int i = 0; i < descriptorSet.size(); i++)
		{
			Utils::updateDescriptorSets(
				device, descriptorSet[i], {
					bufferData
				}, imageData);
		}

		const auto& renderPass = As<VulkanRenderPass>(props.render_pass_)->renderPass;
		auto vi = VulkanVertexBuffer::GetVulkanVertexLayout(props.vertex_layout_);
		graphicsPipeline = Utils::makeGraphicsPipeline(device,
		                                               pipelineCache,
		                                               vertexShaderModule,
		                                               nullptr,
		                                               fragmentShaderModule,
		                                               nullptr,
		                                               props.vertex_layout_.GetStride(),
		                                               {
			                                               vi
		                                               },
		                                               vk::FrontFace::eClockwise,
		                                               true,
		                                               pipelineLayout,
		                                               renderPass);
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
