/************************************************************************************//*!
\file          PhysicsManifold.h
\project       <PROJECT_NAME>
\author        Chua Teck Lee, c.tecklee, 390008420
\par           email: c.tecklee\@digipen.edu
\date          September 2, 2021
\brief

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/PhysicsCollision/Colliders.h"

namespace engine
{
    // generate manifold assumes that collision has been detected.
    struct PhysicsManifold
    {
        static Manifold2D GenerateManifold2D(CircleCollider2D const& circle, CircleCollider2D const& circle2);
        static Manifold2D GenerateManifold2D(CircleCollider2D const& circle, BoxCollider2D const& aabb);
        static Manifold2D GenerateManifold2D(BoxCollider2D const& aabb, CircleCollider2D const& circle);
        static Manifold2D GenerateManifold2D(BoxCollider2D const& aabb, BoxCollider2D const& aabb2);
    };

}