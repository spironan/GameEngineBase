#pragma once

#include "Engine/PhysicsCollision/Manifold.h"

#include <vector>

namespace engine
{
    class Solver
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        Solver()                            = default;
        Solver(Solver const&)               = default;
        Solver(Solver &&)                   = default;
        Solver& operator=(Solver const&)    = default;
        Solver& operator=(Solver &&)        = default;
        ~Solver()                           = default;

        virtual void Solve(std::vector<Manifold2D> manifolds, float dt) = 0;
    };
}