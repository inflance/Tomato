#pragma once
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

namespace Tomato
{
	struct Vertex
	{
		glm::vec3 position_{};
		glm::vec3 normal_{};
		glm::vec3 color_{};
		glm::vec2 tex_coord_{};

		bool operator==(const Vertex& other) const
		{
			return position_ == other.position_ && color_ == other.color_ && tex_coord_ == other.tex_coord_ && normal_ == other.normal_;
		}
	};

	struct MeshData
	{
		std::vector<Vertex> vertices_;
		std::vector<uint32_t> indices_;
	};


	class Model
	{
	public:
		Model(const std::string& path);

		~Model() = default;

		auto& GetMeshData() { return m_data; }

	private:
		MeshData m_data;
	};

	struct MeshRenderComponent
	{
		Model model;
	};
}
