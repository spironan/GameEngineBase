/************************************************************************************//*!
\file          OpenGLShader.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 3, 2021
\brief         This file contains API for a concrete implementation of OpenGL shader. 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Renderer/Shader.h"
#include "Utility/Hash.h"
#include <glad/glad.h>
#include <oom/oom.hpp>

namespace engine
{

class OpenGLShader : public Shader
{
public:
	OpenGLShader() = default;
	/*********************************************************************************//*!
	\brief    Constructs an OpenGLShader object using file path
	 
	\param    filePath absolute path to the file
	*//**********************************************************************************/
	OpenGLShader(const std::string& filePath);
	static std::shared_ptr<OpenGLShader> CreateFromString(const std::string& source);
	~OpenGLShader() = default;

	/*********************************************************************************//*!
	\brief    Binds the OpenGL shader to the pipeline
	 
	*//**********************************************************************************/
	void Bind();
	/*********************************************************************************//*!
	\brief    Unbinds any shader program bound to the OpenGL pipeline
	 
	*//**********************************************************************************/
	void Unbind();

	/*********************************************************************************//*!
	\brief    Returns the raw ID of the Shader program
	 
	\return   Raw ID
	*//**********************************************************************************/
	GLint getID()const	{return m_ShaderID;	}


	/*-----------------------------------------------------------------------------*/
	/* Uniform setter Functions                                                    */
	/*-----------------------------------------------------------------------------*/
	virtual void SetUniform(const std::string& fullname, float value);
	virtual void SetUniform(const std::string& fullname, uint32_t value);
	virtual void SetUniform(const std::string& fullname, int value);
	virtual void SetUniform(const std::string& fullname,uint32_t count, const int value[]);
	virtual void SetUniform(const std::string& fullname, const oom::ivec2& value);
	virtual void SetUniform(const std::string& fullname, const oom::ivec3& value);
	virtual void SetUniform(const std::string& fullname, const oom::ivec4& value);
	virtual void SetUniform(const std::string& fullname, const oom::vec2& value);
	virtual void SetUniform(const std::string& fullname, const oom::vec3& value);
	virtual void SetUniform(const std::string& fullname, const oom::vec4& value);
	virtual void SetUniform(const std::string& fullname, const oom::mat3& value);
	virtual void SetUniform(const std::string& fullname, const oom::mat4& value);

private:
	/*********************************************************************************//*!
	\brief    Reads the entire shader file and returns it as a std::string object
	 
	\param    filepath absolute path to the file
	\return   
	*//**********************************************************************************/
	std::string OpenGLShader::ReadShaderFromFile(const std::string& filepath) const;

	/*********************************************************************************//*!
	\brief    Loads the .glsl shader from std::string into memory, compiling linking and validating
	 
	\param    source single string of shader source code
	*//**********************************************************************************/
	void Load(const std::string& source);

	/*********************************************************************************//*!
	\brief    Helper function to get shader name as readable for printing messages
	 
	\param    type GLenum of the shader type
	\return   std::string of shader type
	*//**********************************************************************************/
	std::string GetGLShaderName(GLenum type);

	/*********************************************************************************//*!
	\brief    Helper function to get GLenum shader type from string. Useful for parsing shaders
	 
	\param    name name of the shader to find
	\return   GLenum shader type
	*//**********************************************************************************/
	GLenum GetGLShaderType(engine::utility::StringHash name);
	
private:
	uint32_t m_ShaderID = 0;
	std::string m_Name,m_filePath;

	std::unordered_map<GLenum, std::string> m_shaderSources;
};

}
