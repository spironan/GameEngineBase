#include "pch.h"
#include "RenderPass.h"

#include "Engine/Platform/OpenGL/OpenGLRenderPass.h"

namespace engine
{

std::shared_ptr<RenderPass> engine::RenderPass::Create(const RenderPassSpecification& spec)
{
    return std::make_shared<OpenGLRenderPass>(spec);
}

}
