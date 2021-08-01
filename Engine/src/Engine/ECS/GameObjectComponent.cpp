/************************************************************************************//*!
\file          GameObjectComponent.cpp
\project       <PROJECT_NAME>
\author        Chua Teck Lee, c.tecklee, 390008420
\par           email: c.tecklee\@digipen.edu
\date          August 2, 2021
\brief         GameObjectComponent holds essential/useful
               information in general that each gameobject has

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "GameObjectComponent.h"

#include <rttr/registration>

namespace engine
{
    /********************************************************************************//*!
     @brief     Used to register for RTTR Specifically to display the relevant
                information that will be displayed to the editor
    *//*********************************************************************************/
    RTTR_REGISTRATION
    {
        using namespace rttr;
        registration::class_<GameObjectComponent>("GameObject")
            .property("Active", &GameObjectComponent::ActiveSelf)
            .property("Name", &GameObjectComponent::Name);
    }
}
