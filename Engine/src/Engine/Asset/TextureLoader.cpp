/************************************************************************************//*!
\file          TextureLoader.cpp
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 1, 2021
\brief         This file contains implementation of loading textures from files to rendering system


Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "TextureLoader.h"

#include "Engine/Core/Log.h"
#include <glad/glad.h>

// This ignores all warnings raised inside External headers
namespace stbi_local
{

#pragma warning(push, 0)
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma warning(pop, 0)
}

namespace engine
{

Texture TextureLoader::LoadFromFilePath(const std::string& fileName)
{
	Texture texture;
	texture.name = fileName;
	unsigned char* texData = stbi_local::stbi_load(fileName.c_str(), &texture.width, &texture.height, &texture.bitDepth, stbi_local::STBI_rgb_alpha);
	if (!texData)
	{
		LOG_ENGINE_ERROR("Failed to load image at filepath:{0}", fileName);
		texture.id = engine::ooTexID{ 0 };
		return texture;
	}


	{
		glCreateTextures(GL_TEXTURE_2D, 1, &texture.id);
		// allocate GPU storage for texture image data loaded from file
		glTextureStorage2D(texture.id, 1, GL_RGBA8, texture.width, texture.height);
		// copy image data from client memory to GPU texture buffer memory
		glTextureSubImage2D(texture.id, 0, 0, 0, texture.width, texture.height,
							GL_RGBA, GL_UNSIGNED_BYTE, texData);

		glTextureParameteri(texture.id, GL_TEXTURE_WRAP_S, GL_REPEAT); // default to repeat
		glTextureParameteri(texture.id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}


	stbi_local::stbi_image_free(texData);

	return texture;
}

void engine::TextureLoader::DestroyTexture(ooTexID id)
{
	glDeleteTextures(1, &id);
}

void engine::TextureLoader::DestroyTexture(Texture texture)
{
	glDeleteTextures(1, &texture.id);
}

}
