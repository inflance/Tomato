#pragma once
#include "Tomato/tmtpch.h"
#include "OpenGLShader.h"
#include "Tomato/Core.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"

namespace Tomato {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if(type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		else if (type == "fragment" || type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}
		LOG_ASSERT(false, "δ֧�ִ�����");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		m_name = GetFileName(filePath);
		std::string shaderStr = ReadFile(filePath);
		//LOG_ASSERT("{0}", res);
		auto shaderSource = Preprocess(shaderStr);
		Compile(shaderSource);
	}

	OpenGLShader::OpenGLShader( const std::string& vertexPath, const std::string& fragmentPath)
	{
		m_name = GetFileName(vertexPath);
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ��֤ifstream��������׳��쳣

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			// ���ļ�
			vShaderFile.open(vertexPath.c_str());
			fShaderFile.open(fragmentPath.c_str());
			std::stringstream vShaderStream, fShaderStream;
			// ��ȡ�ļ��Ļ������ݵ���������
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// �ر��ļ�������
			vShaderFile.close();
			fShaderFile.close();
			// ת����������string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}


		std::unordered_map<GLenum, std::string> source;
		source[GL_VERTEX_SHADER] = vertexCode;
		source[GL_FRAGMENT_SHADER] = fragmentCode;

		Compile(source);
	}

	

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_shader_id);

	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(m_shader_id, name.c_str()), value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		glUniform1iv(glGetUniformLocation(m_shader_id, name.c_str()),count, values);
	}
	
	void OpenGLShader::CheckCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];

		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				LOG_ERROR("SHADER ERROR: Shader compilation error of type : {0}\nERROR INFO:{1} ", type, infoLog);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				LOG_ERROR("SHADER ERROR: Program linking error of type : {0}\nERROR INFO:{1} ", type, infoLog);
				glDeleteShader(shader);
			}
		}
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		GLint location = glGetUniformLocation(m_shader_id, name.c_str());

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));

	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_shader_id, name.c_str());

		glUniform4f(location, value.x, value.y, value.z, value.w);

	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value)
	{
		GLint location = glGetUniformLocation(m_shader_id, name.c_str());

		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetFloat1(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_shader_id, name.c_str());

		glUniform1f(location, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_shader_id, name.c_str());

		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_shader_id, name.c_str());

		glUniform3f(location, value.x, value.y, value.z);

	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSource)
	{
		GLuint program = glCreateProgram();
		std::vector<GLenum> shaderIDs;
		for (auto kv : shaderSource) {
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceStr = source.c_str();

			glShaderSource(shader, 1, &sourceStr, nullptr);

			glCompileShader(shader);
			if (type == GL_VERTEX_SHADER) {
				CheckCompileErrors(shader, "VERTEX");
			}else if (type == GL_FRAGMENT_SHADER) {
				CheckCompileErrors(shader, "FRAGMENT");
			}
			
			glAttachShader(program, shader);
			shaderIDs.push_back(shader);
		}

		m_shader_id = program;
		//������ɫ������
		glLinkProgram(m_shader_id);
		CheckCompileErrors(m_shader_id, "PROGRAM");

		//������ɣ�ɾ��ռ�õ���ɫ����Դ
		for (auto id : shaderIDs) {
			glDeleteShader(id);
		}
	}

	std::string OpenGLShader::ReadFile(const std::string& path)
	{
		std::string result;
		std::ifstream in(path, std::ios::in, std::ios::binary);
		//tellg() �������������л�ȡλ��
		//seekg(offset,) �����������������е�λ��
		//
		if (in) {
			//ֱ�������ļ���β
			in.seekg(0, std::ios::end);
			//��result����Ϊ�ļ��Ĵ�С
			result.resize(in.tellg());
			//����ָ�뵽�ļ���ͷ
			in.seekg(0, std::ios::beg);
			//���ļ�д���ַ���
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			LOG_ERROR("���ܴ��ļ���{0}", path);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::Preprocess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSource;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos) 
		{
			size_t eol = source.find_first_of("\r\n", pos);
			LOG_ASSERT(eol != std::string::npos, "�﷨����");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			LOG_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "��Ч��shader����");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSource[ShaderTypeFromString(type)] = source.substr(nextLinePos,
				pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

		}

		return shaderSource;
	}

	//���ļ�������ȡ����
	std::string OpenGLShader::GetFileName(const std::string& filePath)
	{
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;

		return filePath.substr(lastSlash, count);
	}



}