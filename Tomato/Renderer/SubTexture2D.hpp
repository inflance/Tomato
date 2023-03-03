#pragma once

#include <glm/glm.hpp>

namespace Tomato
{
	class Texture2D;

	class SubTexture2D
	{
	public:
		SubTexture2D(const std::shared_ptr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		void GenTexCoord();
		[[nodiscard]] std::shared_ptr<Texture2D> GetTexture() const { return m_texture; }
		[[nodiscard]] glm::vec2* GetTexCoord() { return m_tex_coord; }

		static std::shared_ptr<SubTexture2D> Create(const std::shared_ptr<Texture2D>& texture,
		                                            const glm::vec2& coords, const glm::vec2& sprite_size,
		                                            const glm::vec2& mag_size = {1.0f, 1.0f});

	private:
		std::shared_ptr<Texture2D> m_texture;
		glm::vec2 m_tex_coord[4];
		glm::vec2 m_min{}, m_max{};
	};
}
