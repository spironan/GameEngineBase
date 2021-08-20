#pragma once

#include <memory>

#include "Engine/Core/Assert.h"

#include "Engine/Renderer/Framebuffer.h"

namespace engine
{

class OpenGLFramebuffer : public Framebuffer
{
public:
	OpenGLFramebuffer(const FramebufferSpecification& spec);
	virtual ~OpenGLFramebuffer();

	virtual void Resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const override;

	virtual std::shared_ptr<Image2D> GetImage(uint32_t attachmentIndex = 0) const override { ENGINE_ASSERT(attachmentIndex < m_ColorAttachments.size()); return m_ColorAttachments[attachmentIndex]; }
	virtual std::shared_ptr<Image2D> GetDepthImage() const override { return m_DepthAttachment; }

	virtual uint32_t GetWidth() const override { return m_Width; }
	virtual uint32_t GetHeight() const override { return m_Height; }
	virtual ooRendererID GetRendererID() const { return m_RendererID; }
	virtual ooRendererID GetColorAttachmentRendererID(int index = 0) const { return m_ColorAttachments[index]->GetID(); }
	virtual ooRendererID GetDepthAttachmentRendererID() const { return m_DepthAttachment->GetID(); }

	virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

private:
	FramebufferSpecification m_Specification;
	ooRendererID m_RendererID = 0;

	std::vector<std::shared_ptr<Image2D>> m_ColorAttachments;
	std::shared_ptr<Image2D> m_DepthAttachment;

	std::vector<ImageFormat> m_ColorAttachmentFormats;
	ImageFormat m_DepthAttachmentFormat = ImageFormat::None;

	uint32_t m_Width = 0, m_Height = 0;
};

}
