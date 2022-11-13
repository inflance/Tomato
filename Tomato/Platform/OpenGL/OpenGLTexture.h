#pragma once

#include "Tomato/Renderer/Texture.h"

#include <glad/glad.h>

namespace Tomato{
	class OpenGLTexture : public Texture2D
	{
	public:
		OpenGLTexture(uint32_t width, uint32_t heigth);
		OpenGLTexture(const std::string& path);
		virtual ~OpenGLTexture();

		virtual uint32_t GetWidth() const override { return m_width; }
		virtual uint32_t GetHeight() const override { return m_height; }
		virtual uint32_t GetID() const override { return m_renderer_id; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_renderer_id == ((OpenGLTexture&)other).m_renderer_id;
		};

		virtual void Bind(uint32_t slot = 0) const override;
	private:
		uint32_t m_width;
		uint32_t m_height;
		unsigned int m_renderer_id;
		std::string m_path;

		GLenum m_internalformat, m_format;
	};
}