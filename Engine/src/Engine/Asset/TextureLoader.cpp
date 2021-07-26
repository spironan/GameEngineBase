#include "pch.h"
#include "TextureLoader.h"

#include "Engine/Core/Log.h"
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace engine
{

Texture TextureLoader::LoadFromFilePath(const std::string& fileName)
{
	Texture texture;
	texture.name = fileName;
	unsigned char* texData = stbi_load(fileName.c_str(), &texture.width, &texture.height, &texture.bitDepth, STBI_rgb_alpha);
	if (!texData)
	{
		LOG_ENGINE_ERROR("Failed to load image at filepath:{0}", fileName);
		texture.id = -1;
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


	stbi_image_free(texData);

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
