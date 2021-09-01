/************************************************************************************//*!
\file          PhysicsSystem.cpp
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
#include "pch.h"

#include "PhysicsSystem.h"
#include "Engine/ECS/ECS.h"

#include "Engine/ECS/GameObject.h"

#include "RigidBody.h"

namespace engine
{
    void PhysicsSystem::Update(Timestep deltaTime)
    {
        UpdateDynamics(deltaTime);
    }

    void PhysicsSystem::UpdateDynamics(Timestep deltaTime)
    {
        auto& container = m_ECS_Manager.GetComponentDenseArray<RigidBody>();
        for (auto& rb : container)
        {
            if (rb.IsStatic) continue;

            if (rb.UseGravity)
            {
                rb.ApplyGravity(Gravity);
                rb.UpdateVelocity(deltaTime);
            }

            rb.UpdatePosition(deltaTime);

            rb.SetForce(oom::vec2{0.f});
        }
    }

    void PhysicsSystem::UpdatePhysicsCollision()
    {

    }

    void PhysicsSystem::UpdatePhysicsResolution()
    {

    }

}
