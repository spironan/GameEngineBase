#include "pch.h"
#include "Sprite2D.h"

namespace engine
{


Sprite2D::Sprite2D(Entity _entityID, bool _active): 
    Component{ _entityID, _active },
    m_texture{ std::numeric_limits<uint32_t>::max() },
    m_color{ 1.0f,1.0f,1.0f,1.0f }
{
}

void Sprite2D::SetTexture(ooTexID textureID)
{
     m_texture = textureID; 
}

void Sprite2D::SetTexture(Texture tex)
{
    m_texture = tex.id;
}

}