#pragma once

#include <unordered_map>

#include "Mesh.h"

namespace Tomato
{
	using AssetID = UUID;

	class MeshFactory
	{
	public:
		static MeshFactory& Get()
		{
			static MeshFactory instance;
			return instance;
		}

		void Add(const std::string& path, const Mesh& data);

		Mesh& Load(const std::string& path, Mesh& data);

		Mesh& GetMesh(const std::string& path);

		bool Exist(const std::string& path);

	private:
		std::unordered_map<std::string, Mesh> m_mesh_factory;
	};
}
