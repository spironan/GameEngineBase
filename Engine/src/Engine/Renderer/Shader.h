/************************************************************************************//*!
\file          Shader.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 3, 2021
\brief         File contains a shader interface for creating abstracted shaders 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once
#include <memory>
#include "glm/glm.hpp"

namespace engine
{

// Represents a complete shader program stored in a single file.
class Shader
{
public:
	virtual ~Shader() = default;

	/*********************************************************************************//*!
	\brief    Parses and creates a shader object from a .glsl file containing both vertex and fragment shader.
			  The shader types must be defined in the .glsl file using a keyword "<#type> <fragment>" etc.
	 
	\param    filepath filepath to the shader file
	\return   
	*//**********************************************************************************/
	static std::shared_ptr<Shader> Create(const std::string& filepath);
	static std::shared_ptr<Shader> CreateFromString(const std::string& source);
};

}//end namespace engine

