#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "Matirial.h"

#include "MatirialFactory.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace Tomato {

	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Shader;

	using Index = uint32_t;
	
	struct Vertex {

		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;

		int EntityID = -1;
	};

	struct SkeletonVertex {
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
		SubMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
		SubMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<MatirialTextureData>& m_texture);

		template<typename Func>
		void Draw(const Func& func) const {
			func(m_VertexArray, m_VertexBuffer, m_vertices, m_texture);
		}
	
	private:
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<MatirialTextureData> m_texture;

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
		const std::vector<SubMesh>& GetMesh(){ return m_submeshs; };
		[[nodiscard]] AssetID GetID() const { return m_id; };
		std::string& GetPath() { return m_path; };

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		void GetStaticMeshData(aiMesh* mesh,  const aiScene* scene);
		std::vector<MatirialTextureData> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
		
	private:
		int m_id;
		std::string m_directory;
		std::string m_path;
		std::vector<SubMesh> m_submeshs;
	};

	
}