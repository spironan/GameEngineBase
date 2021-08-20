#include "pch.h"
#include <utility>

#include "Framebuffer.h"
#include "Engine/Platform/OpenGL/OpenGLFramebuffer.h"

namespace engine
{

std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
{
    return std::make_shared<OpenGLFramebuffer>(spec);
}


// Framebuffer pool
std::unordered_map<int32_t, std::shared_ptr<Framebuffer>>   FramebufferPool::m_nameToFrameBuffer;
std::unordered_map<ooRendererID, std::string>               FramebufferPool::m_idToName;

}