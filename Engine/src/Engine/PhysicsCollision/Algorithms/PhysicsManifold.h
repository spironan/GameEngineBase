/************************************************************************************//*!
\file           PhysicsManifold.h
\project        <PROJECT_NAME>
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           September 2, 2021
\brief          Contains a Physics Manifold Generator that generates a manifold
                between 2 Shapes. See manifold.h for more information

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/PhysicsCollision/Components/Colliders.h"

namespace engine
{
    // generate manifold assumes that collision has been detected.
    struct PhysicsManifold
    {
        static Manifold2D Generate(Circle const& circleA, Circle const& circleB);
        static Manifold2D Generate(Circle const& circle, AABB2D const& aabb);
        static Manifold2D Generate(AABB2D const& aabb, Circle const& circle);
        static Manifold2D Generate(AABB2D const& aabbA, AABB2D const& aabbB);
    };

}