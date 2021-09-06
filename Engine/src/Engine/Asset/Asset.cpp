/************************************************************************************//*!
\file          Asset.cpp
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          September 6, 2021
\brief         This file contains definitions for creating assets. This will select the platform dependent type 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "Asset.h"
#include "Engine/Platform/OpenGL/OpenGLTexture.h"

namespace engine
{

/*********************************************************************************//*!
  Textures
*//**********************************************************************************/

std::shared_ptr<Texture2D> engine::Texture2D::Create(ImageFormat format, uint32_t width, uint32_t height, const void* data, TextureProperties properties)
{
    return std::make_shared<OpenGLTexture2D>(format, width, height, data, properties);
}

std::shared_ptr<Texture2D> engine::Texture2D::Create(const std::string& path, TextureProperties properties)
{
    return std::make_shared<OpenGLTexture2D>(path, properties);
}
/*********************************************************************************//*!
 END Textures
*//**********************************************************************************/


}// End namespace engine