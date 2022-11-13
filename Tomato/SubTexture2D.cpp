#include "tmtpch.h"

#include "SubTexture2D.h"

namespace Tomato{


	SubTexture2D::SubTexture2D(const std::shared_ptr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		:m_texture(texture)
	{
		m_texcoord[0] = { min.x, min.y };
		m_texcoord[1] = { max.x, min.y };
		m_texcoord[2] = { max.x, max.y };
		m_texcoord[3] = { min.x, max.y };
	}

	std::shared_ptr<Tomato::SubTexture2D> SubTexture2D::CreateSubtexture(const std::shared_ptr<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize, const glm::vec2& magSize)
	{
		glm::vec2  min = { (coords.x * spriteSize.x) / texture->GetWidth(), (coords.y * spriteSize.y) / texture->GetHeight() };
		glm::vec2  max = { ((coords.x + magSize.x) * spriteSize.x) / texture->GetWidth(), ((coords.y + magSize.y) *spriteSize.y) / texture->GetHeight() };
		return std::make_shared<SubTexture2D>(texture, min, max);
	}

}