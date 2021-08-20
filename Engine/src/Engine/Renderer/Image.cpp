#include "pch.h"
#include "Image.h"

#include "Engine/Platform/OpenGL/OpenGLImage.h"

namespace engine
{

std::shared_ptr<Image2D> Image2D::Create(ImageSpecification specification, Buffer buffer)
{
    return std::make_shared<OpenGLImage2D>(specification,buffer);
}

std::shared_ptr<Image2D> Image2D::Create(ImageSpecification specification, const void* data)
{
    return std::make_shared<OpenGLImage2D>(specification, data);
}

}
