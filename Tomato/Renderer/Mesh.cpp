#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Tomato/Core/Core.h>

#include <Tomato/Function/Timer.h>
#include "VertexArray.h"
#include "Buffer.h"
#include "MeshFactory.h"

namespace Tomato {

	void Mesh::Load(int entityID, const std::string& path)
	{
		Timer timer("load");
		if (!m_path.empty())
		{
			m_submeshs.clear();
		}
		m_path = path;
		m_ID = entityID;
		if (MeshFactory::Getnstance().Exist(m_path)) {
			*this = MeshFactory::Getnstance().GetMesh(m_path);
		}
		else {
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
			//¼ì²é´íÎó

			LOG_ASSERT(scene || !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || scene->mRootNode, "ERROR::ASSIMP::{0} ", importer.GetErrorString());
			// retrieve the directory path of the filepath
			m_directory = path.substr(0, path.find_last_of('/'));

			// process ASSIMP's root node recursively
			ProcessNode(scene->mRootNode, scene);
			MeshFactory::Getnstance().Add(m_path, *this);
		}
		
	}


	void Mesh::ProcessNode(aiNode* node, const aiScene* scene)
	{
		//Timer timer("ProcessNodes");
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			
			// walk through each of the mesh's vertices
			GetStaticMeshData(mesh, scene);
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	void Mesh::GetStaticMeshData(aiMesh* mesh, const aiScene* scene)
	{
		//Timer timer("GetStaticMeshData");
		std::vector<Vertex> veries;
		std::vector<uint32_t> indices;
		std::vector<MatirialTextureData> textures;
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			// positions
			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			// normals
			if (mesh->HasNormals())
			{
				vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
			}
			else {
				LogSystem::ConsoleLog("Mesh don`t have normal", LogType::Warn);
			}
			// texture coordinates
			if (mesh->mTextureCoords[0])
			{
				vertex.TexCoords = {
					mesh->mTextureCoords[0][i].x ,
					mesh->mTextureCoords[0][i].y
				};
				// tangent
				vertex.Tangent = {
					mesh->mTangents[i].x,
					mesh->mTangents[i].y,
					mesh->mTangents[i].z
				};
				// bitangent
				vertex.Bitangent = {
					mesh->mBitangents[i].x,
					mesh->mBitangents[i].y,
					mesh->mBitangents[i].z
				};
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			vertex.EntityID = m_ID;
			veries.push_back(vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		
		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<MatirialTextureData> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<MatirialTextureData> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<MatirialTextureData> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::Normal);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<MatirialTextureData> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::Ambient);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		m_submeshs.push_back(SubMesh(veries, indices, textures));
	}

	std::vector<MatirialTextureData> Mesh::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName)
	{
		//Timer timer("LoadMaterialTextures");
		std::vector<MatirialTextureData> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			std::string path = m_directory+ '/'+std::string(str.C_Str());
			LOG_ERROR(path);
			if (MatirialFactory::GetInstance().Exist(path))
			{
				//Timer timer("skip");
				textures.push_back(MatirialFactory::GetInstance().GetMatirial(path));
			}
			else {
				//Timer timer("!skip");
				MatirialTextureData texture;
				texture.Type = typeName;
				texture.Path = str.C_Str();
				texture = MatirialFactory::GetInstance().Load(path, texture);
				textures.push_back(texture);
			}
		}
		return textures;
	}


	SubMesh::SubMesh(const std::vector<Vertex>& verties, const std::vector<uint32_t>& indices)
		:m_Verties(verties), m_Indices(indices)
	{
		m_VertexArray = VertexArray::Create();

		m_VertexBuffer = VertexBuffer::Create(sizeof(Vertex) * m_Verties.size());
		m_VertexBuffer->SetLayout({
					{ ShaderDataType::Float3, "a_Position"},
					{ ShaderDataType::Float3, "a_Normal"},
					{ ShaderDataType::Float2, "a_TexCoord"},
					{ ShaderDataType::Float3, "a_Tangent"},
					{ ShaderDataType::Float3, "a_Bitangent"},
					{ ShaderDataType::Int,	  "a_EntityID"},
			});

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size());

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}


	SubMesh::SubMesh(const std::vector<Vertex>& verties, const std::vector<uint32_t>& indices, const std::vector<MatirialTextureData>& texture)
		:SubMesh(verties, indices)
	{
		m_texture = texture;
	}

}