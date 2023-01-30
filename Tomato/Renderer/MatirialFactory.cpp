#include "MatirialFactory.h"

namespace Tomato
{
	void MatirialFactory::Add(const std::string& path, const MatirialTextureData& data)
	{
		LOG_ASSERT(!Exist(path), "");
		m_matirial_factory[path] = data;
	}

	MatirialTextureData& MatirialFactory::Load(const std::string& path, MatirialTextureData& data)
	{
		data.Texture = Texture2D::Create(path);
		data.ID = data.Texture->GetID();
		Add(path, data);
		return m_matirial_factory[path];
	}

	MatirialTextureData& MatirialFactory::GetMatirial(const std::string& path)
	{
		LOG_ASSERT(Exist(path), "");
		return m_matirial_factory[path];
	}

	bool MatirialFactory::Exist(const std::string& path)
	{
		return m_matirial_factory.contains(path);
	}
}
