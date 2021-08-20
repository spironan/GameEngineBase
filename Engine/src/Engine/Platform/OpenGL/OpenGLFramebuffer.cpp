#include "pch.h"

#include "Engine/Platform/OpenGL/OpenGLFramebuffer.h"
#include "Engine/Platform/OpenGL/OpenGLImage.h"

#include "Engine/Core/Application.h"

#include "glad/glad.h"

namespace engine
{

namespace Utils
{
	static GLenum TextureTarget(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void CreateTextures(bool multisampled, ooRendererID* outID, uint32_t count)
	{
		glCreateTextures(TextureTarget(multisampled), 1, outID);
	}

	static void BindTexture(bool multisampled, ooRendererID id)
	{
		glBindTexture(TextureTarget(multisampled), id);
	}

	static GLenum DataType(GLenum format)
	{
		switch (format)
		{
			case GL_RGBA8:             return GL_UNSIGNED_BYTE;
			case GL_RG16F:
			case GL_RG32F:
			case GL_RGBA16F:
			case GL_RGBA32F:           return GL_FLOAT;
			case GL_DEPTH24_STENCIL8:  return GL_UNSIGNED_INT_24_8;
		}

		ENGINE_ASSERT_MSG(false, "Unknown format");
		return 0;
	}

	static GLenum DepthAttachmentType(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::DEPTH32F:        return GL_DEPTH_ATTACHMENT;
			case ImageFormat::DEPTH24STENCIL8: return GL_DEPTH_STENCIL_ATTACHMENT;
		}
		ENGINE_ASSERT_MSG(false, "Unknown format");
		return 0;
	}

	static std::shared_ptr<Image2D> CreateAndAttachColorAttachment(int samples, ImageFormat format, uint32_t width, uint32_t height, int index)
	{
		bool multisampled = samples > 1;
		std::shared_ptr<Image2D> image;
		if (multisampled)
		{
			//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			ImageSpecification spec;
			spec.Format = format;
			spec.Width = width;
			spec.Height = height;
			image = Image2D::Create(spec);
			image->Generate();
		}

		std::shared_ptr<OpenGLImage2D> glImage = std::dynamic_pointer_cast<OpenGLImage2D>(image);
		glImage->CreateSampler(TextureProperties());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), glImage->GetRendererID(), 0);
		return image;
	}

	static std::shared_ptr<Image2D> AttachDepthTexture(int samples, ImageFormat format, uint32_t width, uint32_t height)
	{
#if 0
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(TextureTarget(multisampled), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(TextureTarget(multisampled), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(TextureTarget(multisampled), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(TextureTarget(multisampled), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
#endif
		bool multisampled = samples > 1;
		std::shared_ptr<Image2D> image;
		if (multisampled)
		{
			//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			ImageSpecification spec;
			spec.Format = format;
			spec.Width = width;
			spec.Height = height;
			image = Image2D::Create(spec);

			image->Generate();
		}

		std::shared_ptr<OpenGLImage2D> glImage = std::dynamic_pointer_cast<OpenGLImage2D>(image);
		glImage->CreateSampler(TextureProperties());
		glFramebufferTexture2D(GL_FRAMEBUFFER, Utils::DepthAttachmentType(format), TextureTarget(multisampled), glImage->GetRendererID(), 0);
		return image;

	}

}

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
	:m_Specification(spec), m_Width(spec.Width), m_Height(spec.Height)
{
	ENGINE_ASSERT(spec.Attachments.Attachments.size());

	for (auto format : m_Specification.Attachments.Attachments)
	{
		if (!Utils::IsDepthFormat(format.Format))
			m_ColorAttachmentFormats.emplace_back(format.Format);
		else
			m_DepthAttachmentFormat = format.Format;
	}

	uint32_t width = spec.Width;
	uint32_t height = spec.Height;
	if (width == 0)
	{
		engine::Window& x = engine::Application::Get().GetWindow();
		width = x.GetSize().first;
		height = x.GetSize().second;
	}
	Resize(width, height, true);
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
}

void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height, bool forceRecreate)
{
	if (!forceRecreate && (m_Width == width && m_Height == height))
		return;

	m_Width = width;
	m_Height = height;

	if (m_RendererID)
	{
		glDeleteFramebuffers(1, &m_RendererID);
		m_ColorAttachments.clear();
	}

	glCreateFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	if (m_ColorAttachmentFormats.size())
	{
		m_ColorAttachments.resize(m_ColorAttachmentFormats.size());

		// Create color attachments
		for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			m_ColorAttachments[i] = Utils::CreateAndAttachColorAttachment(m_Specification.Samples,
																		  m_ColorAttachmentFormats[i],
																		  m_Width, m_Height,
																		  int(i));
	}

	if (m_DepthAttachmentFormat != ImageFormat::None)
	{
		m_DepthAttachment = Utils::AttachDepthTexture(m_Specification.Samples,
													  m_DepthAttachmentFormat,
													  m_Width, m_Height);
	}

	if (m_ColorAttachments.size() > 1)
	{
		ENGINE_ASSERT(m_ColorAttachments.size() <= 4);
		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		//glDrawBuffers(int(m_ColorAttachments.size()), buffers);
		glNamedFramebufferDrawBuffers(m_RendererID, (GLsizei)m_ColorAttachments.size(), buffers);
	}
	else if (m_ColorAttachments.empty())
	{
		// Only depth-pass
		glDrawBuffer(GL_NONE);
	}
	auto res = glCheckNamedFramebufferStatus(m_RendererID,GL_FRAMEBUFFER);
	ENGINE_ASSERT_MSG(res == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

	//res = glGetError();
	//while (res != GL_NO_ERROR)
	//{
	//	res = glGetError();
	//}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	glViewport(0, 0, m_Width, m_Height);
}

void OpenGLFramebuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::BindTexture(uint32_t attachmentIndex, uint32_t slot) const
{
	glBindTextureUnit(slot,m_ColorAttachments[attachmentIndex]->GetID());
}

}
