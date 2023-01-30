#pragma once

#include "VulkanSwapChain.h"
#include "Tomato/Core/Macro.h"
#include "Tomato/Renderer/Pipeline.h"
#include "Tomato/Renderer/RendererAPI.h"

namespace Tomato
{
	class VulkanRendererAPI : public RendererAPI
	{
	public:
		VulkanRendererAPI();

		void Init() override;

		void SetViewPort(int x, int y, uint32_t width, uint32_t height) override;
		void Clear() override;
		void SetColor(const glm::vec4& color) override;
		void DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count = 0) override;
		void DrawArray(const std::shared_ptr<VertexArray>& vertex_array, uint32_t count = 0) override;
		void Submit(const std::shared_ptr<Pipeline>& m_pipeline) override;
	private:
		Ref<VulkanSwapChain> m_swap_chain;
	};
}
