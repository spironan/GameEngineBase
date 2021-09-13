/************************************************************************************//*!
\file           PhysicsFwd.h
\project        <PROJECT_NAME>
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           September 2, 2021
\brief          Forward Declaration of all the various types that are used in the 
                Physics Engine

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <oom/oom.hpp>

namespace engine
{
    using vec2 = oom::vec2;
    using vec3 = oom::vec3;
    using mat2 = oom::mat2;
    using mat3 = oom::mat3;

    class Rigidbody2D;
    enum class BodyType;

    class Collider2D;

    struct BoundingVolume2D;    // broadphase AABB-Collider
    struct ColliderBase2D;
    struct BoxCollider2D;
    struct CircleCollider2D;

    struct Contact2D;
    struct Manifold2D;
    struct PhysicsMaterial;
    struct MassData;

    struct Solver;
    struct ImpulseSolver;
}
