/************************************************************************************//*!
\file          PhysicsSystem.h
\project       <PROJECT_NAME>
\author        Chua Teck Lee, c.tecklee, 390008420
\par           email: c.tecklee\@digipen.edu
\date          July 27, 2021
\brief         Describes a Physics System that applies dynamics to all physics objects
               performs collision detection between physics objects and 
               resolve Physics based Collision resolution.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/PhysicsCollision/PhysicsFwd.h"
#include "Engine/ECS/System.h"
#include "Engine/Core/Timestep.h"
#include "Engine/ECS/World.h"
#include "Engine/PhysicsCollision/Manifold.h"

#include "Solvers/ImpulseSolver.h"

namespace engine
{
    class PhysicsSystem : public System
    {
    public:
        /*********************************************************************************//*!
        \brief    Default constructor
         
        \param    ECS_Manager
                  The Manager that controls of the systems
        *//**********************************************************************************/
        PhysicsSystem(ECS_Manager& ECS_Manager);

        /*********************************************************************************//*!
        \brief    Updates the Physics System everyframe
         
        \param    deltaTime
                    Amount of time passed since the previous frame.
        *//**********************************************************************************/
        void Update(Timestep deltaTime);
        
        vec2 Gravity;

        static constexpr double AccumulatorLimit = 0.2;
        static constexpr double FixedDeltaTime = 1.0 / 120.0;

    private:
        void UpdateDynamics(Timestep deltaTime);
        void UpdatePhysicsCollision();
        void UpdatePhysicsResolution(Timestep deltaTime);

        double m_accumulator;

        std::vector<Manifold2D> m_collisions;
        std::vector<Manifold2D> m_triggers;
        std::vector<Solver*> m_solvers;

        ImpulseSolver m_impulseSolver;
    };
}
