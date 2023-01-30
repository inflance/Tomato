#pragma once

#include "Shader.h"
#include "Tomato/Core/Macro.h"
#include "Buffer.h"

namespace Tomato
{
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

	struct PipelineProps
	{
		std::shared_ptr<Shader> Shader;
		//defalt primitive type 
		PrimitiveType Type = PrimitiveType::Triangles;
		BufferLayout Layout;
		BufferLayout InstanceLayout;
		Ref<RenderPass> RenderPass;
		DepthCompareOperator DepthOperator = DepthCompareOperator::GreaterOrEqual;
		bool BackfaceCulling = true;
		bool DepthTest = true;
		bool DepthWrite = true;
		bool Wireframe = false;
		float LineWidth = 1.0f;

		std::string DebugName;
	};

	class Pipeline
	{
	public:
		virtual ~Pipeline() = default;
		virtual void Init() = 0;

		virtual void Bind() = 0;

		static std::shared_ptr<Pipeline> Create(const PipelineProps& pipeline_props);
	};
}
