#include "MeshFactory.hpp"

#include "Tomato/Core/Core.hpp"

namespace Tomato
{
	void MeshFactory::Add(const std::string& path, const Mesh& data)
	{
		LOG_ASSERT(!Exist(path), "");
		m_mesh_factory[path] = data;
	}

	Mesh& MeshFactory::Load(const std::string& path, Mesh& data)
	{
		Add(path, data);
		return m_mesh_factory[path];
	}

	Mesh& MeshFactory::GetMesh(const std::string& path)
	{
		LOG_ASSERT(Exist(path), "");
		return m_mesh_factory[path];
	}

	bool MeshFactory::Exist(const std::string& path)
	{
		return m_mesh_factory.contains(path);
	}
}
