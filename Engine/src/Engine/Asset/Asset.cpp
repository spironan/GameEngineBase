#include "pch.h"
#include "Asset.h"
#include "Engine/Platform/OpenGL/OpenGLTexture.h"

namespace engine
{
std::shared_ptr<Texture2D> engine::Texture2D::Create(ImageFormat format, uint32_t width, uint32_t height, const void* data, TextureProperties properties)
{
    return std::make_shared<OpenGLTexture2D>(format, width, height, data, properties);
}

std::shared_ptr<Texture2D> engine::Texture2D::Create(const std::string& path, TextureProperties properties)
{
    return std::make_shared<OpenGLTexture2D>(path, properties);
}
}// End namespace engine