/************************************************************************************//*!
\file          OpenGLShader.cpp
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 3, 2021
\brief         This file contains a concrete implementation of an OpenGL shader.
				Functions include setting of uniforms

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "OpenGLShader.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Assert.h"

#include "glm/gtc/type_ptr.hpp"

namespace engine
{

OpenGLShader::OpenGLShader(const std::string& filePath)
	:m_filePath{filePath}
{
	//get the name from the filepath
	size_t found = filePath.find_last_of("/\\");
	m_Name = found != std::string::npos ? filePath.substr(found + 1) : filePath;
	found = m_Name.find_last_of(".");
	m_Name = found != std::string::npos ? m_Name.substr(0, found) : m_Name;

	//Reload()
	std::string source = ReadShaderFromFile(m_filePath);
	Load(source);
}

void OpenGLShader::Bind()
{
	glUseProgram(m_ShaderID);
}

void OpenGLShader::Unbind()
{
	glUseProgram(0);
}

void OpenGLShader::SetUniform(const std::string& fullname, float value)
{
	GLint location = glGetUniformLocation(m_ShaderID, fullname.c_str());
	ENGINE_ASSERT_MSG(location != -1, "Cannot find uniform '{0}' in {1} shader ", fullname,m_Name);
	glUniform1f(location, value);
}

void OpenGLShader::SetUniform(const std::string& fullname, uint32_t value)
{
	GLint location = glGetUniformLocation(m_ShaderID, fullname.c_str());
	ENGINE_ASSERT_MSG(location != -1, "Cannot find uniform '{0}' in {1} shader ", fullname, m_Name);
	glUniform1ui(location, value);
}

void OpenGLShader::SetUniform(const std::string& fullname, int value)
{
	GLint location = glGetUniformLocation(m_ShaderID, fullname.c_str());
	ENGINE_ASSERT_MSG(location != -1, "Cannot find uniform '{0}' in {1} shader ", fullname, m_Name);
	glUniform1i(location, value);
}

void OpenGLShader::SetUniform(const std::string& fullname, uint32_t count, const int value[])
{
	GLint location = glGetUniformLocation(m_ShaderID, fullname.c_str());
	ENGINE_ASSERT_MSG(location != -1, "Cannot find uniform '{0}' in {1} shader ", fullname, m_Name);
	glUniform1iv(location, count, value);
}

void OpenGLShader::SetUniform(const std::string& fullname, const glm::ivec2& value)
{
	GLint location = glGetUniformLocation(m_ShaderID, fullname.c_str());
	ENGINE_ASSERT_MSG(location != -1, "Cannot find uniform '{0}' in {1} shader ", fullname, m_Name);
	glUniform2i(location, value.x,value.y);
}

void OpenGLShader::SetUniform(const std::string& fullname, const glm::ivec3& value)
{
	GLint location = glGetUniformLocation(m_ShaderID, fullname.c_str());
	ENGINE_ASSERT_MSG(location != -1, "Cannot find uniform '{0}' in {1} shader ", fullname, m_Name);
	glUniform3i(location, value.x, value.y,value.z);
}

void OpenGLShader::SetUniform(const std::string& fullname, const glm::ivec4& value)
{
	GLint location = glGetUniformLocation(m_ShaderID, fullname.c_str());
	ENGINE_ASSERT_MSG(location != -1, "Cannot find uniform '{0}' in {1} shader ", fullname, m_Name);
	glUniform4i(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::SetUniform(const std::string& fullname, const glm::vec2& value)
{
	GLint location = glGetUniformLocation(m_ShaderID, fullname.c_str());
	ENGINE_ASSERT_MSG(location != -1, "Cannot find uniform '{0}' in {1} shader ", fullname, m_Name);
	glUniform2fv(location, 1,glm::value_ptr(value));
}

void OpenGLShader::SetUniform(const std::string& fullname, const glm::vec3& value)
{
	GLint location = glGetUniformLocation(m_ShaderID, fullname.c_str());
	ENGINE_ASSERT_MSG(location != -1, "Cannot find uniform '{0}' in {1} shader ", fullname, m_Name);
	glUniform3fv(location, 1, glm::value_ptr(value));
}

void OpenGLShader::SetUniform(const std::string& fullname, const glm::vec4& value)
{
	GLint location = glGetUniformLocation(m_ShaderID, fullname.c_str());
	ENGINE_ASSERT_MSG(location != -1, "Cannot find uniform '{0}' in {1} shader ", fullname, m_Name);
	glUniform4fv(location, 1, glm::value_ptr(value));
}

void OpenGLShader::SetUniform(const std::string& fullname, const glm::mat3& value)
{
	GLint location = glGetUniformLocation(m_ShaderID, fullname.c_str());
	ENGINE_ASSERT_MSG(location != -1, "Cannot find uniform '{0}' in {1} shader ", fullname, m_Name);
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::SetUniform(const std::string& fullname, const glm::mat4& value)
{
	GLint location = glGetUniformLocation(m_ShaderID, fullname.c_str());
	ENGINE_ASSERT_MSG(location != -1, "Cannot find uniform '{1}' in {2} shader ", fullname, m_Name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

std::string OpenGLShader::ReadShaderFromFile(const std::string& filepath) const
{
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
		}
		else
		{
			ENGINE_ASSERT_MSG(false, "Could not load shader!");
		}
		in.close();
		return result;
	}
}

void OpenGLShader::Load(const std::string& source)
{
	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);
	while (pos != std::string::npos)
	{
		size_t eol = source.find_first_of("\r\n", pos);
		ENGINE_ASSERT_MSG(eol != std::string::npos, "Syntax error");
		size_t begin = pos + typeTokenLength + 1;
		std::string type = source.substr(begin, eol - begin);
		ENGINE_ASSERT_MSG(type == "vertex" || type == "fragment" || type == "pixel" || type == "compute", "Invalid shader type specified");

		size_t nextLinePos = source.find_first_not_of("\r\n", eol);
		pos = source.find(typeToken, nextLinePos);
		auto shaderType = GetGLShaderType(type.c_str());
		m_shaderSources[shaderType] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

		// Compute shaders cannot contain other types
		//if (shaderType == GL_COMPUTE_SHADER)
		//{
		//	m_IsCompute = true;
		//	break;
		//}
	}


	if (m_ShaderID)
		glDeleteProgram(m_ShaderID);

	GLuint program = glCreateProgram();
	m_ShaderID = program;

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	for (auto i : m_shaderSources)
	{
		GLenum shaderType = i.first;
		std::string& data = i.second;
		const GLchar* ptr = (data.c_str());
		const GLint size = static_cast<GLint>(data.size());

		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &ptr, &size);
		glCompileShader(shader);

		{
			result = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
			if (!result)
			{
				glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
				LOG_ENGINE_ERROR("SHADER: Error compiling {0} \n\t : {1} shader '{2}'",m_Name, GetGLShaderName(shaderType), eLog);
				return;
			}
		}
		glAttachShader(m_ShaderID, shader);
	}

	//link program
	glLinkProgram(m_ShaderID);
	{
		result = 0;
		glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(m_ShaderID, sizeof(eLog), NULL, eLog);
			LOG_ENGINE_ERROR("Error linking {0} shader : '{1}'", m_Name, eLog);
			return;
		}
	}

	//validate program
	glValidateProgram(m_ShaderID);
	{
		result = 0;
		glGetProgramiv(m_ShaderID, GL_VALIDATE_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(m_ShaderID, sizeof(eLog), NULL, eLog);
			LOG_ENGINE_ERROR("Error validating {0} shader : '{1}'", m_Name, eLog);
			return;
		}
	}

}

std::string OpenGLShader::GetGLShaderName(GLenum type)
{
	switch (type)
	{
		case GL_FRAGMENT_SHADER:return "Fragment";
		case GL_VERTEX_SHADER:	return "Vertex";
		case GL_COMPUTE_SHADER: return "Compute";

		default:
			return "Unknown type";
			break;
	}
}

GLenum OpenGLShader::GetGLShaderType(engine::utility::StringHash name)
{
	switch (name)
	{
		case engine::utility::StringHash("vertex"):		return GL_VERTEX_SHADER;
		case engine::utility::StringHash("fragment"):	return GL_FRAGMENT_SHADER;
		case engine::utility::StringHash("compute"):	return GL_COMPUTE_SHADER;
		default:
			return GL_NONE;
			break;
	}
}

}// end namespace engine

