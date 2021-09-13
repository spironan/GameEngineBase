/************************************************************************************//*!
\file           Solver.h
\project        <PROJECT_NAME>
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           September 2, 2021
\brief          Describes a Solver that has the ability to solve various rigidbody 
                constraints

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/PhysicsCollision/Manifold.h"
#include <vector>

namespace engine
{
    struct Solver
    {
        virtual void Solve(std::vector<Manifold2D>& manifolds, float dt) = 0;
    };
}