#pragma once

#include "Engine/Renderer/RenderPass.h"

namespace engine
{

class OpenGLRenderPass : public RenderPass
{
public:
	OpenGLRenderPass(const RenderPassSpecification& spec);
	virtual ~OpenGLRenderPass();

	virtual RenderPassSpecification& GetSpecification() override { return m_Specification; }
	virtual const RenderPassSpecification& GetSpecification() const override { return m_Specification; }
private:
	RenderPassSpecification m_Specification;
};

}
