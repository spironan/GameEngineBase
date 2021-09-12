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
    // Forward Declare
    struct BoundingVolume;
    // Broad-phase sorting
    struct SortSweepCompare
    {
        ECS_Manager& Manager;
        int Axis;

        SortSweepCompare(ECS_Manager& ecsManager, int axis)
            : Manager{ ecsManager }
            , Axis{ axis }
        {};

        bool operator()(BoundingVolume a, BoundingVolume b);

    };

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
        
        vec2 Gravity = { 0,-9.81f * 50 };

        static constexpr double AccumulatorLimit = 0.2;
        static constexpr double FixedDeltaTime = 0.02;

    private:
        void UpdateDynamics(Timestep deltaTime);
        void UpdatePhysicsCollision();
        void UpdateCallbacks();
        void UpdatePhysicsResolution(Timestep deltaTime);

        //time accumulator
        double m_accumulator;

        void BroadPhase();
        SortSweepCompare m_broadphaseCompare;

        void NarrowPhase();

        std::vector<std::pair<Collider2D, Collider2D>> m_narrowPhaseTriggers;
        std::vector<std::pair<Collider2D, Collider2D>> m_narrowPhaseColliders;

        std::vector<Manifold2D> m_collisions;
        std::vector<Manifold2D> m_triggers;
        std::vector<Solver*> m_solvers;

        ImpulseSolver m_impulseSolver;
    };


}
