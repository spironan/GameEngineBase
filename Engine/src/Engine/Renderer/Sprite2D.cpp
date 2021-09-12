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
#include "Engine/Asset/AssetsManager.h"

#include "Engine/ECS/GameObject.h"

#include <rttr/registration>

namespace engine
{

    RTTR_REGISTRATION
    {
        using namespace rttr;
        registration::class_<Sprite2D>("Sprite2D")
        .property("Texture", &Sprite2D::GetHandle, select_overload<void(AssetHandle)>(&Sprite2D::SetTexture))
        .property("Colour", &Sprite2D::GetColor, select_overload<void(oom::vec4)>(&Sprite2D::SetColor));
    }


Sprite2D::Sprite2D(Entity _entityID, bool _active): 
    Component{ _entityID, _active },
    m_color{ 1.0f,1.0f,1.0f,1.0f }
{
}


void Sprite2D::SetTexture(std::shared_ptr<Texture2D> tex)
{
    m_texture = tex;
}

void Sprite2D::SetTexture(std::shared_ptr<Texture> tex)
{
    m_texture = tex;
}

void Sprite2D::SetTexture(AssetHandle tex)
{
    m_texture = AssetManager::GetAsset<Texture>(tex);
}

void Sprite2D_GetColor(Entity instanceID, float* r, float* g, float* b, float* a)
{
    GameObject obj{ instanceID };
    oom::vec4& color = obj.GetComponent<Sprite2D>().GetColor();
    *r = color.r;
    *g = color.g;
    *b = color.b;
    *a = color.a;
}

void Sprite2D_SetColor(Entity instanceID, float r, float g, float b, float alpha)
{
    GameObject obj{ instanceID };
    obj.GetComponent<Sprite2D>().SetColor(r, g, b, alpha);
}

}