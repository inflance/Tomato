#include "Pipeline.h"

#include "Renderer.h"
#include "Tomato/Core/Macro.h"
#include "Tomato/Renderer//Vulkan/VulkanPipeline.h"

namespace Tomato
{
	std::shared_ptr<Pipeline> Pipeline::Create(const PipelineInfo& pipeline_props)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanPipeline>(pipeline_props);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "δ֪API");
		return nullptr;
	}
}
