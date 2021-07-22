#include "pch.h"
#include "Sprite2D.h"

engine::Sprite2D::Sprite2D(Entity _entityID, bool _active): 
    Component{ _entityID, _active },
    m_texture{0},
    m_color{ 1.0f,1.0f,1.0f,1.0f }
{
}
