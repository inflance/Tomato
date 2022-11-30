#pragma once

#include <glm/glm.hpp>

#include "Tomato/Renderer/Texture.h"

namespace Tomato {

	class SubTexture2D
	{
	public:
		SubTexture2D(const std::shared_ptr<Texture2D>& texture,const glm::vec2& min,const glm::vec2& max);
		
		const std::shared_ptr<Texture2D> GetTexture() const { return m_texture; }
		glm::vec2* GetTextcoord(){ return m_texcoord; }

		static std::shared_ptr<SubTexture2D> CreateSubtexture(const std::shared_ptr<Texture2D>& texture, const glm::vec2& coords,  const glm::vec2& spriteSize, const glm::vec2& magSize = {1.0f, 1.0f});
	
	private:
		std::shared_ptr<Texture2D> m_texture;
		glm::vec2 m_texcoord[4];
	};

}