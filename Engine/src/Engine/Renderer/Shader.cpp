/************************************************************************************//*!
\file          Shader.cpp
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 3, 2021
\brief         File contains a interface selector for shaders

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "Shader.h"

#include "Engine/Platform/OpenGL/OpenGLShader.h"

namespace engine
{


std::shared_ptr<Shader> Shader::Create(const std::string& filepath)
{
	// can check if we are on vulkan/opengl here before returning
	return std::make_shared<OpenGLShader>(OpenGLShader(std::forward<const std::string&>(filepath)));
}

}