#pragma once

#include "Tomato/Core/Core.h"

#include "VertexArray.h"
#include "Buffer.h"
#include "Mesh.h"

namespace Tomato {

	struct BaseShapeData
	{
		std::vector<Vertex> Vertices;
		std::vector<Index> indices;
		int EntityID;
	};

	class BaseShape
	{
	public:
		BaseShape() = default;
		BaseShape(int entityID);
		void CreateCube(int entityID);

		void SetData();

		[[nodiscard]] Ref<VertexArray> GetVertexArray() const { return m_vertex_array; }
		[[nodiscard]] Ref<VertexBuffer> GetVertexBuffer() const { return m_vertex_buffer; }
		[[nodiscard]] Ref<IndexBuffer> GetIndexArray() const { return m_index_buffer; }
		[[nodiscard]] const BaseShapeData& GetBaseShape() const { return m_base_shape; }

	private:
		BaseShapeData m_base_shape;
		Ref<VertexArray> m_vertex_array;
		Ref<VertexBuffer> m_vertex_buffer;
		Ref<IndexBuffer> m_index_buffer;
	};

}