#include "Shader.hpp"
#pragma once

#include "Shader.hpp"

#include "Renderer.hpp"
#include "Tomato/Core/Core.hpp"
#include "Tomato/Renderer/Vulkan/VulkanShader.hpp"

namespace Tomato
{

	std::shared_ptr<Shader> Shader::Create(const ShaderCreateInfo& createInfo)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanShader>(createInfo);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "δ֪API");
		return nullptr;
	}
}
