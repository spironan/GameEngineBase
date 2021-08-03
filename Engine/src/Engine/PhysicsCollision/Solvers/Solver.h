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