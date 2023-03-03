#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>


#include "Material.hpp"
#include "MatirialFactory.hpp"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace Tomato
{
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Shader;

	using Index = uint32_t;

	struct Vertex1
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;

		int EntityID = -1;
	};

	struct SkeletonVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;

		int IDs[4] = {0};
		float Weights[4] = {0.0f};

		int EntityID = -1;
	};

	class SubMesh
	{
	public:
		SubMesh() = default;
		SubMesh(std::vector<Vertex1> vertices, std::vector<uint32_t> indices);
		SubMesh(const std::vector<Vertex1>& vertices, const std::vector<uint32_t>& indices,
		        const std::vector<MaterialTextureData>& m_texture);

		template <typename Func>
		void Draw(const Func& func) const
		{
			func(m_VertexArray, m_VertexBuffer, m_vertices, m_texture);
		}

	private:
		std::vector<Vertex1> m_vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<MaterialTextureData> m_texture;

		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};

	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(const Mesh&) = default;

		void Load(int entityID = -1, const std::string& path = "PreCompile/Assets/Mesh/bbl/bbl.pmx");
		const std::vector<SubMesh>& GetMesh() { return m_submeshs; };
		std::string& GetPath() { return m_path; };

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		void GetStaticMeshData(aiMesh* mesh, const aiScene* scene);
		std::vector<MaterialTextureData>
		LoadMaterialTextures(aiMaterial* mat, aiTextureType type, PBRTextureType typeName);

	private:
		int m_id;
		std::string m_directory;
		std::string m_path;
		std::vector<SubMesh> m_submeshs;
	};
}
