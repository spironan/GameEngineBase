#pragma once

#include "Solver.h"

namespace engine
{
    struct ImpulseSolver final : public Solver
    {
        virtual void Solve(std::vector<Manifold2D>& manifolds, float dt) override;
    };
}
