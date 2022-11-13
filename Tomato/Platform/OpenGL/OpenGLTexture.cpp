#include "OpenGLTexture.h"

#include "Tomato/Core.h"
#include "stb_image.h"

namespace Tomato {
	

	OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height)
		:m_width(width), m_height(height)
	{

		m_internalformat = GL_RGBA8;
		m_format = GL_RGBA;
		LOG_ASSERT(m_internalformat & m_format, "��ʽ����֧��");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
		glTextureStorage2D(m_renderer_id, 1, m_internalformat, m_width, m_height);
		// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glGenerateMipmap(m_renderer_id);
		
	}

	OpenGLTexture::OpenGLTexture(const std::string& path)
		:m_path(path)
	{
		int width, height, channels;

		//��ͼƬ��ת
		stbi_set_flip_vertically_on_load(true);

		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		m_width = width;
		m_height = height;

		LOG_ASSERT(data, path+" ·���µ�ͼƬ����ʧ��");

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

		LOG_ASSERT(internalformat & format, "��ʽ����֧��");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
		glTextureStorage2D(m_renderer_id, 1, m_internalformat, m_width, m_height);
			// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, m_format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(m_renderer_id);
		stbi_image_free(data);
	}


	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_renderer_id);
	}

	void OpenGLTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_renderer_id);
	}

	void OpenGLTexture::SetData(void* data, uint32_t size)
	{
		//byte per pixel
		uint32_t bpp = m_format == GL_RGBA ? 4 : 3;
		LOG_ASSERT(size == m_width * m_height * bpp, "���ݱ�����ʵ�ʵĲ���");
		glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, m_format, GL_UNSIGNED_BYTE, data);
	}

}