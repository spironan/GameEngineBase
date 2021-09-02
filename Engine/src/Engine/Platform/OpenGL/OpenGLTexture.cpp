#include "pch.h"

#include "Engine/Core/Log.h"
#include "Engine/Platform/OpenGL/OpenGLImage.h"
#include "OpenGLTexture.h"
#include "stb_image.h"

engine::OpenGLTexture2D::OpenGLTexture2D(ImageFormat format, uint32_t width, uint32_t height, const void* data, TextureProperties properties)
	:m_width(width), m_height(height), m_properties(properties)
{
	ImageSpecification spec;
	spec.Format = format;
	spec.Width = width;
	spec.Height = height;
	m_image = Image2D::Create(spec);
	m_image->Generate();

	m_loaded = true;
}

engine::OpenGLTexture2D::OpenGLTexture2D(const std::string& path, TextureProperties properties)
	: m_properties(properties), m_filePath(path)
{
	int width{}, height{}, channels{};

	LOG_ENGINE_INFO("Loading texture {0}, srgb={1}", path, m_properties.SRGB? "RGB": "RGBA");

	stbi_uc* imageData = stbi_load(path.c_str(), &width, &height, &channels, (m_properties.SRGB ? STBI_rgb : STBI_rgb_alpha) );
	ImageFormat format = m_properties.SRGB ? ImageFormat::RGB : ImageFormat::RGBA;
	Buffer buffer(imageData, Utils::GetImageMemorySize(format, width, height));
	ImageSpecification spec;
	spec.Format = format;
	spec.Width = width;
	spec.Height = height;
	m_image = Image2D::Create(spec, buffer);

	std::shared_ptr<OpenGLImage2D> ogi = std::dynamic_pointer_cast<OpenGLImage2D>(m_image);
	ogi->CreateSampler(m_properties);

	m_width = width;
	m_height = height;
	m_loaded = true;

	m_image->Generate();
	stbi_image_free(m_image->GetBuffer().Data);
	m_image->GetBuffer() = Buffer();

}

engine::OpenGLTexture2D::~OpenGLTexture2D()
{
	m_image->Release();
}

void engine::OpenGLTexture2D::Bind(uint32_t slot) const
{
	glBindTextureUnit(slot, m_image->GetID());
}
