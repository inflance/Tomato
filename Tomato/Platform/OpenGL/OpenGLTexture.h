#pragma once

#include <glad/glad.h>

#include "Tomato/Renderer/Texture.h"

namespace Tomato{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t heigth);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual const std::string& GetPath() const override { return m_path; }
		virtual uint32_t GetWidth() const override { return m_width; }
		virtual uint32_t GetHeight() const override { return m_height; }
		virtual uint32_t GetID() const override { return m_renderer_id; }

		void SetWhiteTexture();

		virtual void SetData(void* data, uint32_t size) override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_renderer_id == ((OpenGLTexture2D&)other).m_renderer_id;
		};

		virtual void BindUnit(uint32_t slot = 0) const override;
		virtual void Bind() const override;
	private:
		uint32_t m_width;
		uint32_t m_height;
		unsigned int m_renderer_id;
		std::string m_path;

		GLenum m_internalformat, m_format;
	};

	class OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube(uint32_t width, uint32_t heigth);
		OpenGLTextureCube(const std::vector<std::string>& paths);
		virtual ~OpenGLTextureCube();

		virtual const std::string& GetPath() const override { return m_path; }
		virtual uint32_t GetWidth() const override { return m_width; }
		virtual uint32_t GetHeight() const override { return m_height; }
		virtual uint32_t GetID() const override { return m_renderer_id; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_renderer_id == ((OpenGLTextureCube&)other).m_renderer_id;
		};

		virtual void BindUnit(uint32_t slot = 0) const override;
		virtual void Bind() const override;
	private:
		uint32_t m_width;
		uint32_t m_height;
		unsigned int m_renderer_id;
		std::string m_path;

		GLenum m_internalformat, m_format;
	};
}