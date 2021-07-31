#pragma once

#include "Solver.h"

namespace engine
{
    class ImpulseSolver : public Solver
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        ImpulseSolver()                                 = default;
        ImpulseSolver(ImpulseSolver const&)             = default;
        ImpulseSolver(ImpulseSolver &&)                 = default;
        ImpulseSolver& operator=(ImpulseSolver const&)  = default;
        ImpulseSolver& operator=(ImpulseSolver &&)      = default;
        ~ImpulseSolver()                                = default;

        void Solve(std::vector<Manifold2D> manifolds, float dt);
    };
}
