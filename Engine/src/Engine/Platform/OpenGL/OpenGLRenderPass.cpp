#include "pch.h"
#include "OpenGLRenderPass.h"

engine::OpenGLRenderPass::OpenGLRenderPass(const RenderPassSpecification& spec)
	:m_Specification{spec}
{
}

engine::OpenGLRenderPass::~OpenGLRenderPass()
{
	//does nothing for now..
}
