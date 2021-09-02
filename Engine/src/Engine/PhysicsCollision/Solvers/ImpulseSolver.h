/************************************************************************************//*!
\file          ImpulseSolver.h
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

#include "Solver.h"

namespace engine
{
    struct ImpulseSolver final : public Solver
    {
        virtual void Solve(std::vector<Manifold2D>& manifolds, float dt) override;
    };
}
