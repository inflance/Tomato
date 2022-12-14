#pragma once

#include "Shader.h"

#include "Renderer.h"
#include "Tomato/Core/Core.h"
#include "Tomato/Platform/OpenGL/OpenGLShader.h"

namespace Tomato {

	std::shared_ptr<Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(vertexPath, fragmentPath);
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "δ֪API");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& filePath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(filePath);
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "δ֪API");
		return nullptr;
	}

}