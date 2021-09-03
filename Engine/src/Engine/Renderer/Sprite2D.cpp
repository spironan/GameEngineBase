/************************************************************************************//*!
\file          Sprite2D.cpp
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 1, 2021
\brief         File contains implementation of a Sprite2D component.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "Sprite2D.h"

#include <rttr/registration>

#include "Engine/ECS/WorldManager.h"

namespace engine
{

    RTTR_REGISTRATION
    {
        using namespace rttr;
        registration::class_<Sprite2D>("Sprite2D")
        .property("Texture", &Sprite2D::GetTexture, select_overload<void(ooRendererID)>(&Sprite2D::SetTexture))
        .property("Colour", &Sprite2D::GetColor, select_overload<void(oom::vec4)>(&Sprite2D::SetColor));
    }


Sprite2D::Sprite2D(Entity _entityID, bool _active): 
    Component{ _entityID, _active },
    m_texture{ std::numeric_limits<uint32_t>::max() },
    m_color{ 1.0f,1.0f,1.0f,1.0f }
{
}

void Sprite2D::SetTexture(ooRendererID textureID)
{
     m_texture = textureID; 
}

void Sprite2D::SetTexture(Texture tex)
{
    m_texture = tex.id;
}

void Sprite2D_GetColor(int instanceID, float* r, float* g, float* b, float* a)
{
    oom::vec4& color = WorldManager::GetActiveWorld().GetComponent<Sprite2D>(instanceID).GetColor();
    *r = color.r;
    *g = color.g;
    *b = color.b;
    *a = color.a;
}

void Sprite2D_SetColor(int instanceID, float r, float g, float b, float alpha)
{
    WorldManager::GetActiveWorld().GetComponent<Sprite2D>(instanceID).SetColor(r, g, b, alpha);
}

}