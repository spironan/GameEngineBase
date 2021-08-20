#pragma once

#include "Engine/Asset/AssetTypes.h"
#include "Engine/Core/Assert.h"

#include "Engine/Core/Buffer.h"
#include "Engine/Renderer/Image.h"

#include "glad/glad.h"

namespace engine
{

	class OpenGLImage2D : public Image2D
	{
	public:
		OpenGLImage2D(ImageSpecification specification, Buffer buffer);
		OpenGLImage2D(ImageSpecification specification, const void* data = nullptr);
		virtual ~OpenGLImage2D();

		virtual void Generate() override; // releases if any
		virtual void Release() override;

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual float GetAspectRatio() const override { return (float)m_Specification.Width / (float)m_Specification.Height; }

		virtual ImageSpecification& GetSpecification() override { return m_Specification; }
		virtual const ImageSpecification& GetSpecification() const override { return m_Specification; }

		virtual Buffer GetBuffer() const override { return m_ImageData; }
		virtual Buffer& GetBuffer() override { return m_ImageData; }

		virtual void CreatePerLayerImageViews() override {}

		ooRendererID& GetRendererID() { return m_RendererID; }
		ooRendererID GetRendererID() const { return m_RendererID; }

		ooRendererID& GetSamplerRendererID() { return m_SamplerRendererID; }
		ooRendererID GetSamplerRendererID() const { return m_SamplerRendererID; }

		void CreateSampler(TextureProperties properties);

		virtual ooRendererID GetID() const override { return (ooRendererID)m_RendererID; }
	private:
		ImageSpecification m_Specification;
		uint32_t m_Width, m_Height;
		ooRendererID m_RendererID = 0;
		ooRendererID m_SamplerRendererID = 0;

		Buffer m_ImageData;
	};


	namespace Utils
	{

		inline GLenum OpenGLImageFormat(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RGB:     return GL_RGB;
				case ImageFormat::SRGB:    return GL_RGB;
				case ImageFormat::RGBA:
				case ImageFormat::RGBA16F:
				case ImageFormat::RGBA32F: return GL_RGBA;
			}
			ENGINE_ASSERT_MSG(false, "Unknown image format");
			return 0;
		}

		inline GLenum OpenGLImageInternalFormat(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RGB:             return GL_RGB8;
				case ImageFormat::SRGB:            return GL_SRGB8;
				case ImageFormat::RGBA:            return GL_RGBA8;
				case ImageFormat::RGBA16F:         return GL_RGBA16F;
				case ImageFormat::RGBA32F:         return GL_RGBA32F;
				case ImageFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
				case ImageFormat::DEPTH32F:        return GL_DEPTH_COMPONENT32F;
			}
			ENGINE_ASSERT_MSG(false, "Unknown image format");
			return 0;
		}

		inline GLenum OpenGLFormatDataType(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RGB:
				case ImageFormat::SRGB:
				case ImageFormat::RGBA:    return GL_UNSIGNED_BYTE;
				case ImageFormat::RGBA16F:
				case ImageFormat::RGBA32F: return GL_FLOAT;
			}
			ENGINE_ASSERT_MSG(false, "Unknown image format");
			return 0;
		}

		inline GLenum OpenGLSamplerWrap(TextureWrap wrap)
		{
			switch (wrap)
			{
				case TextureWrap::Clamp:   return GL_CLAMP_TO_EDGE;
				case TextureWrap::Repeat:  return GL_REPEAT;
			}
			ENGINE_ASSERT_MSG(false, "Unknown wrap mode");
			return 0;
		}

		// Note: should always be called with mipmap = false for magnification filtering
		inline GLenum OpenGLSamplerFilter(TextureFilter filter, bool mipmap)
		{
			switch (filter)
			{
				case TextureFilter::Linear:   return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
				case TextureFilter::Nearest:  return mipmap ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
			}
			ENGINE_ASSERT_MSG(false, "Unknown filter");
			return 0;
		}

	}

}
