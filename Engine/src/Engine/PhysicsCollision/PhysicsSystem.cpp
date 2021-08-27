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

#include "Rigidbody.h"

#include "BoxCollider.h"
#include "CircleCollider.h"

#include "Engine/Transform/TransformSystem.h"

#include "Manifold.h"

namespace engine
{
    /*********************************************************************************//*!
        \brief    Default constructor

        \param    ECS_Manager
                  The Manager that controls of the systems
        *//**********************************************************************************/
    PhysicsSystem::PhysicsSystem(ECS_Manager& ECS_Manager)
        : System{ ECS_Manager }
        , m_collisions{ }
        , m_triggers{ }
        , m_solvers{ }
        , m_impulseSolver {}
        , m_accumulator{ 0.f }
        , m_broadphaseCompare { ECS_Manager, 0 }
    {
        m_solvers.emplace_back(&m_impulseSolver);
    };


    void PhysicsSystem::Update(Timestep deltaTime)
    {
        UpdateDynamics(deltaTime);
        UpdatePhysicsCollision();
        UpdatePhysicsResolution(deltaTime);
    }

    void PhysicsSystem::UpdateDynamics(Timestep deltaTime)
    {
        m_accumulator += deltaTime;
        
        //avoids spiral of death.
        if (m_accumulator > AccumulatorLimit)
            m_accumulator = AccumulatorLimit;

        while (m_accumulator > FixedDeltaTime)
        {
            // Update dynamics
            auto& container = m_ECS_Manager.GetComponentDenseArray<Rigidbody2D>();

            for (auto& rb : container)
            {
                if (rb.IsStatic()) continue;

                // apply gravity onto object
                rb.ApplyForce(Gravity * rb.GravityScale * rb.m_data.Mass);

                rb.UpdateVelocity(FixedDeltaTime);
                rb.UpdatePosition(FixedDeltaTime);

                //rb.ApplyForce(-Gravity * rb.GravityScale * rb.m_data.Mass); //normal gravity force acting against object

                rb.SetForce(glm::vec2{ 0.f });
            }

            m_accumulator -= FixedDeltaTime;
        }

        double alpha = m_accumulator / FixedDeltaTime;

        // Update dynamics
        auto& container = m_ECS_Manager.GetComponentDenseArray<Rigidbody2D>();

        for (auto& rb : container)
        {
            if (rb.IsStatic()) continue;

            rb.Interpolate(alpha);
        }
    }

    void PhysicsSystem::UpdatePhysicsCollision()
    {
        //Broadphase Collection Generation : Collider
        //Broadphase Rejection
        // Using Sort And Sweep Algorithm
        //BroadPhase();

        //NarrowPhase Check
        //NarrowPhase();

        //Generate Manifold : Rigidbody only. If trigger : 2 flags and set them
        //_mm_rsqrt_ss

        auto view = m_ECS_Manager.GetComponentView<Rigidbody2D, CircleCollider2D>();
        //auto view = m_ECS_Manager.GetComponentView<Rigidbody2D, BoxCollider2D>();

        for (auto[rigidbody, collider] : view)
        {
            //auto& collider = m_ECS_Manager.GetComponent<BoxCollider2D>(ent);
            //auto& collider = m_ECS_Manager.GetComponent<CircleCollider2D>(ent);
            for (auto[rigidbody2, collider2] : view)
            {
                //auto& collider2 = m_ECS_Manager.GetComponent<BoxCollider2D>(ent2);
                //auto& collider2 = m_ECS_Manager.GetComponent<CircleCollider2D>(ent2);
                if (collider.GetEntity() == collider2.GetEntity()) break;

                /*if(collider.TestCollision(&collider2))
                    LOG_INFO("Collision!");*/
                
                Manifold2D result = collider.TestCollision(&collider2);
                
                LOG_INFO("Collision {0} Normal ({1},{2}) PenDepth {3}", result.HasCollision, result.Normal.x, result.Normal.y, result.PenetrationDepth);

                if (result.HasCollision) m_collisions.emplace_back(result);
                
            }
        }
    }

    void PhysicsSystem::UpdatePhysicsResolution(Timestep dt)
    {
        //Resolve all the collision
        for (auto* solver : m_solvers) {
            solver->Solve(m_collisions, static_cast<float>(dt));
        }
        
        m_collisions.clear();

        //m_impulseSolver.Solve(m_collisions, dt);
       
        // Update all Triggers

    }

    // Attempting Sort and Sweep and Prune for AABBs
    void PhysicsSystem::BroadPhase()
    {
        //Finding max variance
        vec2 centerSum{ 0.f }, centerSumSq{ 0.f };

        auto& view = m_ECS_Manager.GetComponentView<Rigidbody2D, BoxCollider2D>();
        
        for (auto[rigidbody, boxCollider] : view)
        {
            //BoxCollider2D boxCollider = m_ECS_Manager.GetComponent<BoxCollider2D>(ent);

            vec2 center = boxCollider.WorldPosition();
            centerSum += center;
            centerSumSq += center * center;
        }

        // set max variance as axis
        /*centerSum /= view.size();
        centerSumSq /= view.size();*/
        vec2 variance = centerSumSq - (centerSum * centerSum);
        float maxVar = variance.x;
        int maxVarAxis = 0;
        if (variance.x > variance.y)
        {
            maxVar = variance.x;
            maxVarAxis = 0;
        }
        else 
        {
            maxVar = variance.y;
            maxVarAxis = 1;
        }
        
        //std::sort(view.begin(), view.end(), m_broadphaseCompare);
        for (auto [rigidbody, boxCollider] : view)
        {
            float minA = boxCollider.GetGlobalBounds().min[m_broadphaseCompare.Axis];

            for (auto [rigidbody2, boxCollider2] : view)
            {
                if (boxCollider.GetEntity() == boxCollider2.GetEntity()) continue;
                float minB = boxCollider2.GetGlobalBounds().min[m_broadphaseCompare.Axis];

                if (minB > minA) break;

                m_narrowPhase.emplace_back(boxCollider.GetEntity(), boxCollider2.GetEntity());
            }
        }
    }

    void PhysicsSystem::NarrowPhase()
    {
        for (auto& [entA, entB] : m_narrowPhase)
        {
            auto& collider = m_ECS_Manager.GetComponent<BoxCollider2D>(entA);
            auto& collider2 = m_ECS_Manager.GetComponent<BoxCollider2D>(entB);

            Manifold2D result = collider.TestCollision(&collider2);

            LOG_INFO("Collision {0} Normal ({1},{2}) PenDepth {3}", result.HasCollision, result.Normal.x, result.Normal.y, result.PenetrationDepth);

            if (result.HasCollision) m_collisions.emplace_back(result);
        }
    }

    bool SortSweepCompare::operator()(Entity a, Entity b)
    {
        float aMin = Manager.GetComponent<BoxCollider2D>(a).GetGlobalBounds().min[Axis];
        float bMin = Manager.GetComponent<BoxCollider2D>(b).GetGlobalBounds().min[Axis];
        return aMin < bMin;
    }


}
