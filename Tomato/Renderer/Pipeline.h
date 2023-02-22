#pragma once

#include "Tomato/Core/Macro.h"
#include "BufferLayout.h"
#include "UniformBufferSet.h"

namespace Tomato
{
	class Shader;
	class RenderPass;
	class CommandBuffer;

	enum class PrimitiveType
	{
		None = 0,
		Points,
		Lines,
		Triangles,
		LineStrip,
		TriangleStrip,
		TriangleFan
	};

	enum class DepthCompareOperator
	{
		None = 0,
		Never,
		NotEqual,
		Less,
		LessOrEqual,
		Greater,
		GreaterOrEqual,
		Equal,
		Always,
	};

	struct PipelineInfo
	{
		Ref<Shader> shader_ = nullptr;
		Ref<RenderPass> render_pass_ = nullptr;
		PrimitiveType primitive_type_ = PrimitiveType::Triangles;
		VertexBufferLayout vertex_layout_;
		Ref<UniformBufferSet> uniform_buffer_set_;
		DepthCompareOperator depth_operator_ = DepthCompareOperator::GreaterOrEqual;
	};

	class Pipeline
	{
	public:
		virtual ~Pipeline() = default;
		virtual void Init() = 0;

		virtual void Bind() = 0;
		virtual const PipelineInfo& GetProps() = 0;

		static std::shared_ptr<Pipeline> Create(const PipelineInfo& pipeline_props);
	};
}
