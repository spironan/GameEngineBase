/************************************************************************************//*!
\file          GameObjectComponent.h
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
#pragma once

#include <rttr/type>

namespace engine
{
    struct GameObjectComponent final
    {
	public:
        GameObjectComponent();

        bool ActiveSelf = true;
        std::string Name = "new gameobject";

        RTTR_ENABLE();
    };

}
