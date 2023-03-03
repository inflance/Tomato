#include "SubTexture2D.hpp"
#include "Tomato/Renderer/Texture.hpp"

namespace Tomato
{
	SubTexture2D::SubTexture2D(const std::shared_ptr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_texture(texture), m_min(min), m_max(max)
	{
		GenTexCoord();
	}

	void SubTexture2D::GenTexCoord()
	{
		m_tex_coord[0] = {m_min.x, m_min.y};
		m_tex_coord[1] = {m_max.x, m_min.y};
		m_tex_coord[2] = {m_max.x, m_max.y};
		m_tex_coord[3] = {m_min.x, m_max.y};
	}

	std::shared_ptr<SubTexture2D> SubTexture2D::Create(const std::shared_ptr<Texture2D>& texture,
	                                                   const glm::vec2& coords, const glm::vec2& sprite_size,
	                                                   const glm::vec2& mag_size)
	{
		glm::vec2 min = {
			(coords.x * sprite_size.x) / texture->GetWidth(), (coords.y * sprite_size.y) / texture->GetHeight()
		};
		glm::vec2 max = {
			((coords.x + mag_size.x) * sprite_size.x) / texture->GetWidth(),
			((coords.y + mag_size.y) * sprite_size.y) / texture->GetHeight()
		};
		return std::make_shared<SubTexture2D>(texture, min, max);
	}
}
