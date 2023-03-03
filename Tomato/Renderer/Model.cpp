#include "Model.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Tomato/Core/Macro.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace std
{
	template <>
	struct hash<Tomato::Vertex>
	{
		size_t operator()(const Tomato::Vertex& vertex) const noexcept
		{
			return ((hash<glm::vec3>()(vertex.position_) ^ (hash<glm::vec3>()(vertex.normal_) ^
					(hash<glm::vec3>()(vertex.color_) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.tex_coord_) << 1));
		}
	};
}

namespace Tomato
{
	Model::Model(const std::string& path)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
		{
			LOG_ERROR(warn + err);
			ASSERT(false);
		}
		std::unordered_map<Vertex, uint32_t> uniqueVertices{};
		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};
				vertex.position_ = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				if (index.normal_index >= 0)
				{
					vertex.normal_ = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2]
					};
				}

				vertex.tex_coord_ = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0 - attrib.texcoords[2 * index.texcoord_index + 1]
				};

				vertex.color_ = {1.0f, 1.0f, 1.0f};

				if (!uniqueVertices.contains(vertex))
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(m_data.vertices_.size());
					m_data.vertices_.push_back(vertex);
				}

				m_data.indices_.push_back(uniqueVertices[vertex]);
			}
		}
	}
}
