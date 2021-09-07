/************************************************************************************//*!
\file          ColliderDebugDraw.cpp
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          September 8, 2021
\brief         This file defines the debug drawing colours of the collider component 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "ColliderDebugDraw.h"

#include <rttr/registration>

namespace engine
{

    RTTR_REGISTRATION
    {
        using namespace rttr;
        registration::class_<ColliderDebugDraw>("ColliderDebugDraw")
        .property("Colour", &ColliderDebugDraw::GetColor, select_overload<void(const oom::vec4&)>(&ColliderDebugDraw::SetColor));
    }


ColliderDebugDraw::ColliderDebugDraw(Entity _entityID, bool _active)
	:Component{_entityID, _active}
{
}

   //engine::ColliderDebugDraw::ColliderDebugDraw(const oom::vec4& colour)
   //{
   //}

    //engine::ColliderDebugDraw::ColliderDebugDraw(Entity _entityID, bool _active, const oom::vec4& colour)
    //    :Component{ _entityID, _active },
    //    m_color{ colour }
    //{
    //}


}