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

//#include "BoxCollider.h"
//#include "CircleCollider.h"

#include "Engine/Transform/TransformSystem.h"

#include "Rigidbody.h"
#include "Manifold.h"
#include "Algorithms/PhysicsCollision.h"
#include "Colliders.h"
#include "Collider.h"

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

        auto view = m_ECS_Manager.GetComponentView<Rigidbody2D, Collider2D>();
        
        for (auto& [rigidbodyA, colliderA]: view)
        {
            /*auto& rigidbodyA = std::get<Rigidbody2D>(componentA);
            auto& colliderA = std::get<Collider2D>(componentA);*/

            //auto& collider = m_ECS_Manager.GetComponent<BoxCollider2D>(ent);
            //auto& colliderA = m_ECS_Manager.GetComponent<Collider2D>(entA);
            for (auto& [rigidbodyB, colliderB]: view)
            {
                /*auto& rigidbodyB = std::get<Rigidbody2D>(componentB);
                auto& colliderB = std::get<Collider2D>(componentB);*/

                //auto& collider2 = m_ECS_Manager.GetComponent<BoxCollider2D>(ent2);
                //auto& colliderB = m_ECS_Manager.GetComponent<Collider2D>(entB);
                if (colliderA.GetEntity() == colliderB.GetEntity()) break;

                /*if(collider.TestCollision(&collider2))
                    LOG_INFO("Collision!");*/

                Manifold2D result = CollisionMap::TestCollision2D(colliderA, colliderB);

                /*auto& secondCollider = colliderB;

                auto TestCollision = [&](auto const& v1)
                {
                    std::visit([&](auto& v2)
                        {
                            result = PhysicsCollision::Test2DCollision(v1, v2);
                        }, secondCollider.collider);
                };

                std::visit(TestCollision, colliderA.collider);*/

                if (result.HasCollision)
                {
                    result.ObjA = &rigidbodyA; //colliderA.GetComponent<Rigidbody2D>();
                    result.ObjB = &rigidbodyB; //colliderB.GetComponent<Rigidbody2D>();
                    m_collisions.emplace_back(result);
                }

                //Manifold2D result = collider.TestCollision(&collider2);
                
                //LOG_INFO("Collision {0} Normal ({1},{2}) PenDepth {3}", result.HasCollision, result.Normal.x, result.Normal.y, result.PenetrationDepth);

                //if (result.HasCollision) m_collisions.emplace_back(result);
                
            }
        }
    }

    void PhysicsSystem::UpdatePhysicsResolution(Timestep dt)
    {
        //Resolve all the collision
        for (auto* solver : m_solvers) 
        {
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

        auto& view = m_ECS_Manager.GetComponentView<Rigidbody2D, Collider2D>();
        
        for (auto& [rigidbody, collider]: view)
        {
            //BoxCollider2D boxCollider = m_ECS_Manager.GetComponent<BoxCollider2D>(ent);
            //vec2 center;
            //std::visit([&](auto const& v1) { center = v1.WorldPosition(); }, collider.collider);
            vec2 center = collider.WorldPosition();
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

        for (auto& [rigidbodyA, colliderA] : view)
        {
            for (auto& [rigidbodyB, colliderB] : view)
            {
                if (colliderA.GetEntity() == colliderB.GetEntity()) break;

                //auto& secondCollider = colliderB;

                float minA, minB;

                /*auto TestCollision = [&](auto const& v1)
                {
                    std::visit([&](auto& v2)
                        {
                        }, secondCollider.collider);
                };

                std::visit(TestCollision, colliderA.collider);*/

                if (minB > minA) break;

                m_narrowPhase.emplace_back(colliderA, colliderB);
            }
        }

        /*for (auto iterA = view.begin(); iterA != view.end(); ++iterA)
        {
            m_ECS_Manager.GetComponent<Collider2D>(*iterA);

            float minA = m_ECS_Manager.GetComponent<Collider2D>(*iterA).GetGlobalBounds().min[m_broadphaseCompare.Axis];

            for (auto [rigidbody2, boxCollider2] : view)
            {
                if (boxCollider.GetEntity() == boxCollider2.GetEntity()) continue;
                float minB = boxCollider2.GetGlobalBounds().min[m_broadphaseCompare.Axis];

                if (minB > minA) break;

                m_narrowPhase.emplace_back(boxCollider.GetEntity(), boxCollider2.GetEntity());
            }
        }*/
    }

    void PhysicsSystem::NarrowPhase()
    {
        for (auto& [colliderA, colliderB] : m_narrowPhase)
        {
            /*auto& colliderA = m_ECS_Manager.GetComponent<Collider2D>(entA);
            auto& colliderB = m_ECS_Manager.GetComponent<Collider2D>(entB);*/

            /*auto TestCollision = [&](auto const& v1)
            {
                std::visit([&](auto const& v2)
                    {
                        Manifold2D result = PhysicsCollision::Test2DCollision(v1, v2);
                        if (result.HasCollision)
                        {
                            result.ObjA = colliderA.GetComponent<Rigidbody2D>();
                            result.ObjB = colliderB.GetComponent<Rigidbody2D>();
                            m_collisions.emplace_back(result);
                        }
                    }, colliderB.collider);
            };

            std::visit(TestCollision, colliderA.collider);*/

            Manifold2D result = CollisionMap::TestCollision2D(colliderA, colliderB);//collider.TestCollision(&collider2);

            LOG_INFO("Collision {0} Normal ({1},{2}) PenDepth {3}", result.HasCollision, result.Normal.x, result.Normal.y, result.PenetrationDepth);

            if (result.HasCollision) m_collisions.emplace_back(result);
        }
    }

    bool SortSweepCompare::operator()(Entity a, Entity b)
    {
        //float aMin = std::get<BoxCollider2D>(Manager.GetComponent<Collider2D>(a).collider).GetGlobalBounds().min[Axis];
        //float bMin = std::get<BoxCollider2D>(Manager.GetComponent<Collider2D>(b).collider).GetGlobalBounds().min[Axis];
        float aMin = Manager.GetComponent<BoxCollider2D>(a).GetGlobalBounds().min[Axis];
        float bMin = Manager.GetComponent<BoxCollider2D>(b).GetGlobalBounds().min[Axis];

        return aMin < bMin;
    }


}
