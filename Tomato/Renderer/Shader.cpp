#pragma once

#include "Shader.h"

#include "Renderer.h"
#include "Tomato/Core/Core.h"
#include "Tomato/Renderer/Vulkan/VulkanShader.h"

namespace Tomato
{

	std::shared_ptr<Shader> Shader::Create(const std::string& filePath)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanShader>(filePath);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "δ֪API");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::Create(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
	{

		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanShader>(vertexPath, fragmentPath);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "δ֪API");
		return nullptr;
	}

}
