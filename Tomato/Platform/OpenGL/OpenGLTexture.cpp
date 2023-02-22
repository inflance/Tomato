#include "OpenGLTexture.h"

#include <stb_image.h>

#include "Tomato/Core/Core.h"

namespace Tomato {
	
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		:m_width(width), m_height(height)
	{
		SetWhiteTexture();	
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_path(path)
	{
		if (m_path.empty())
		{
			m_width = 1, m_height = 1;
			SetWhiteTexture();
			uint32_t whiteTextureData = 0xffffffff;
			SetData(&whiteTextureData, sizeof(whiteTextureData));
		}
		else {
			int width, height, channels;

			//��ͼƬ��ת
			//stbi_set_flip_vertically_on_load(false);

			stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
			m_width = width;
			m_height = height;

			LOG_ASSERT(data, path + " ·���µ�ͼƬ����ʧ��");

			GLenum internalformat = 0, format = 0;
			if (channels == 1) {
				internalformat = GL_R8;
				format = GL_RED;
			}
			if (channels == 4) {
				internalformat = GL_RGBA8;
				format = GL_RGBA;
			}
			else if (channels == 3) {
				internalformat = GL_RGB8;
				format = GL_RGB;
			}
			m_internal_format = internalformat;
			m_format = format;

			LOG_ASSERT(internalformat && format, "��ʽ����֧��");

			glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
			glTextureStorage2D(m_renderer_id, 1, m_internal_format, m_width, m_height);
			// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, m_format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
	}


	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_renderer_id);
	}

	void OpenGLTexture2D::BindUnit(uint32_t slot /*= 0*/) const
	{
		glBindTextureUnit(slot, m_renderer_id);
	}

	void OpenGLTexture2D::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_renderer_id);
		
	}

	void OpenGLTexture2D::SetWhiteTexture()
	{
		m_internal_format = GL_RGBA8;
		m_format = GL_RGBA;
		LOG_ASSERT(m_internal_format && m_format, "��ʽ����֧��");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
		glTextureStorage2D(m_renderer_id, 1, m_internal_format, m_width, m_height);
		// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glGenerateMipmap(m_renderer_id);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		//byte per pixel
		uint32_t bpp = m_format == GL_RGBA ? 4 : 3;
		LOG_ASSERT(size == m_width * m_height * bpp, "���ݱ�����ʵ�ʵĲ���");
		glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, m_format, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTextureCube::OpenGLTextureCube(uint32_t width, uint32_t heigth)
	{
		m_internalformat = GL_RGBA8;
		m_format = GL_RGBA;

		LOG_ASSERT(m_internalformat && m_format, "��ʽ����֧��");

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_renderer_id);
		glTextureStorage3D(m_renderer_id, 1, m_internalformat, m_width, m_height, m_height);
		// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glGenerateMipmap(m_renderer_id);
	}

	OpenGLTextureCube::OpenGLTextureCube(const std::vector<std::string>& paths)
	{
		int width, height, channels;

		//��ͼƬ��ת
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = nullptr;
		for (int i = 0; i < paths.size(); i++)
		{
			data = stbi_load(paths[i].c_str(), &width, &height, &channels, 0);
			LOG_ASSERT(data, paths[i] + " ·���µ�ͼƬ����ʧ��");
		}
		
		m_width = width;
		m_height = height;

		GLenum internalformat = 0, format = 0;
		if (channels == 4) {
			internalformat = GL_RGBA8;
			format = GL_RGBA;
		}
		else if (channels == 3) {
			internalformat = GL_RGB8;
			format = GL_RGB;
		}
		m_internalformat = internalformat;
		m_format = format;

		LOG_ASSERT(internalformat && format, "��ʽ����֧��");

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_renderer_id);
		glTextureStorage3D(m_renderer_id, 1, m_internalformat, m_width, m_height, m_height);
		// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTextureSubImage3D(m_renderer_id, 0, 0, 0, 0, m_width, m_height, m_height, m_format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(m_renderer_id);
		stbi_image_free(data);
	}

	OpenGLTextureCube::~OpenGLTextureCube()
	{
		glDeleteTextures(1, &m_renderer_id);
	}

	void OpenGLTextureCube::SetData(void* data, uint32_t size)
	{
		//byte per pixel
		uint32_t bpp = m_format == GL_RGBA ? 4 : 3;
		LOG_ASSERT(size == m_width * m_height * bpp, "���ݱ�����ʵ�ʵĲ���");
		glTextureSubImage3D(m_renderer_id, 0, 0, 0, 0, m_width, m_height, m_height, m_format, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTextureCube::BindUnit(uint32_t slot /*= 0*/) const
	{
		glBindTextureUnit(slot, m_renderer_id);
	}

	void OpenGLTextureCube::Bind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_renderer_id);
	}

}