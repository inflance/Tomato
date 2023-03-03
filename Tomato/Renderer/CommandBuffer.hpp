#pragma once
#include "Tomato/Core/Macro.h"

namespace Tomato
{
	class Pipeline;
	class Framebuffer;
	class RenderPass;

	enum class CommandBufferType : uint32_t
	{
		Graphics,
		Compute,
		Raytracing
	};

	struct PipelineStatistics
	{
		uint64_t InputAssemblyVertices = 0;
		uint64_t InputAssemblyPrimitives = 0;
		uint64_t VertexShaderInvocations = 0;
		uint64_t ClippingInvocations = 0;
		uint64_t ClippingPrimitives = 0;
		uint64_t FragmentShaderInvocations = 0;
		uint64_t ComputeShaderInvocations = 0;

		// TODO(Yan): tesselation shader stats when we have them
	};

	class CommandBuffer
	{
	public:
		virtual ~CommandBuffer()
		{
		}

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Submit() = 0;

		virtual float GetExecutionGPUTime(uint32_t frameIndex, uint32_t queryIndex = 0) const = 0;
		virtual const PipelineStatistics& GetPipelineStatistics(uint32_t frameIndex) const = 0;

		virtual uint32_t BeginTimestampQuery() = 0;
		virtual void EndTimestampQuery(uint32_t queryID) = 0;

		static Ref<CommandBuffer> Create(uint32_t count = 0, const std::string& debugName = "");
		static Ref<CommandBuffer> CreateFromSwapChain(const std::string& debugName = "");
	};
}
