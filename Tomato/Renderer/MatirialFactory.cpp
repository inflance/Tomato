#include "MatirialFactory.hpp"

namespace Tomato
{
	void MatirialFactory::Add(const std::string& path, const MaterialTextureData& data)
	{
		LOG_ASSERT(!Exist(path), "");
		m_matirial_factory[path] = data;
	}

	MaterialTextureData& MatirialFactory::Load(const std::string& path, MaterialTextureData& data)
	{
		data.Texture = Texture2D::Create(path);
		Add(path, data);
		return m_matirial_factory[path];
	}

	MaterialTextureData& MatirialFactory::GetMatirial(const std::string& path)
	{
		LOG_ASSERT(Exist(path), "");
		return m_matirial_factory[path];
	}

	bool MatirialFactory::Exist(const std::string& path)
	{
		return m_matirial_factory.contains(path);
	}
}
