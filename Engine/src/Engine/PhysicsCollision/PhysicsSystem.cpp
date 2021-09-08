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
#include "Algorithms/PhysicsManifold.h"
#include "Colliders.h"
#include "ColliderCore.h"
#include "PhysicsUtils.h"

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

                rb.SetForce(oom::vec2{ 0.f });
            }

            m_accumulator -= FixedDeltaTime;
        }

        double alpha = m_accumulator / FixedDeltaTime;

        // Update dynamics
        auto& container = m_ECS_Manager.GetComponentDenseArray<Rigidbody2D>();

        for (auto& rb : container)
        {
            if (rb.IsStatic()) continue;

            rb.Interpolate(static_cast<float>(alpha));
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

        //{
        //    // retrieve all colliders.
        //    auto view = m_ECS_Manager.GetComponentView<Collider2D>();
        //    // perform sort and sweep and prune for all colliders.


        //    // sort remaining colliders(potentially colliding) into two categories : triggers and rigidbodies.
        //    for (auto& [collider] : view)
        //    {
        //        if (collider.IsTrigger)
        //        {
        //            //add to triggers list
        //            m_narrowPhaseTriggers.emplace_back(std::move(collider));
        //        }
        //        else if (collider.HasComponent<Rigidbody2D>())  // not a trigger and has rigidbody
        //        {
        //            //add to physics list
        //            m_narrowPhaseColliders.emplace_back(std::move(collider));
        //        }
        //    }

        //    // narrowphase physics.
        //    for (auto& colliderA : m_narrowPhaseColliders)
        //    {
        //        for (auto& colliderB : m_narrowPhaseColliders)
        //        {
        //            if (colliderA.GetEntity() == colliderB.GetEntity()) break;

        //            if (PhysicsUtils::TestCollision2D(colliderA, colliderB))
        //            {
        //                Manifold2D result = PhysicsUtils::GenerateManifold2D(colliderA, colliderB);
        //                result.ObjA = &colliderA.GetComponent<Rigidbody2D>();
        //                result.ObjB = &colliderB.GetComponent<Rigidbody2D>();
        //                m_collisions.emplace_back(result);

        //                colliderA.m_collisions.emplace(colliderB.GetEntity(), result);
        //                colliderB.m_collisions.emplace(colliderA.GetEntity(), result);
        //            }
        //        }
        //    }

        //    //// narrowphase triggers.
        //    //for (auto& triggerA : m_narrowPhaseTriggers)
        //    //{
        //    //    // check against other triggers
        //    //    for (auto& triggerB : m_narrowPhaseTriggers)
        //    //    {
        //    //        if (triggerA.GetEntity() == triggerB.GetEntity()) break;

        //    //        if (PhysicsUtils::TestCollision2D(triggerA, triggerB))
        //    //        {
        //    //            triggerA.m_triggers.emplace(triggerB.GetEntity(), triggerB);
        //    //            triggerB.m_triggers.emplace(triggerA.GetEntity(), triggerA);
        //    //        }
        //    //    }

        //    //    // check against colliders
        //    //    for (auto& colliderB : m_narrowPhaseColliders)
        //    //    {
        //    //        // there will never be a trigger that is also a collider(no need to check itself)

        //    //        if (PhysicsUtils::TestCollision2D(triggerA, colliderB))
        //    //        {
        //    //            triggerA.m_triggers.emplace(colliderB.GetEntity(), colliderB);
        //    //        }
        //    //    }
        //    //}

        //    // Update all colliders
        //    for (auto& [collider] : view)
        //    {
        //        collider.Update();
        //    }
        //}


        auto view = m_ECS_Manager.GetComponentView<Rigidbody2D, Collider2D, CollisionInfo>();
        
        for (auto& [rigidbodyA, colliderA, collisionInfoA]: view)
        {
            for (auto& [rigidbodyB, colliderB, collisionInfoB]: view)
            {
                // skip same entity and everything onwards
                if (colliderA.GetEntity() == colliderB.GetEntity()) break;
                
                if (PhysicsUtils::TestCollision2D(colliderA, colliderB))
                {
                    if (colliderA.IsTrigger || colliderB.IsTrigger)
                    {
                        if (colliderA.IsTrigger)
                        {
                            collisionInfoA.m_triggers.emplace(colliderB.GetEntity(), colliderB);
                        }
                        if (colliderB.IsTrigger)
                        {
                            collisionInfoB.m_triggers.emplace(colliderA.GetEntity(), colliderA);
                        }
                    }
                    else
                    {
                        Manifold2D result = PhysicsUtils::GenerateManifold2D(colliderA, colliderB);
                        result.ObjA = &rigidbodyA;
                        result.ObjB = &rigidbodyB;
                        m_collisions.emplace_back(result);

                        collisionInfoA.m_collisions.emplace(colliderB.GetEntity(), result);
                        collisionInfoB.m_collisions.emplace(colliderA.GetEntity(), result);
                    }
                }
                
            }
        }

        // Update all collision callbacks
        for (auto& [rigidbody, collider, collisionInfo] : view)
        {
            collisionInfo.Update();
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
        //iterate through all trigger objects
        //
        /*for (auto* solver : m_triggers)
        {
            m_triggers
        }*/

    }

    //// Attempting Sort and Sweep and Prune for AABBs
    //void PhysicsSystem::BroadPhase()
    //{
    //    //Finding max variance
    //    vec2 centerSum{ 0.f }, centerSumSq{ 0.f };

    //    auto& view = m_ECS_Manager.GetComponentView<Rigidbody2D, Collider2D>();
    //    
    //    size_t view_size = 0;
    //    for (auto& [rigidbody, collider]: view)
    //    {
    //        vec2 center = collider.WorldPosition();
    //        centerSum += center;
    //        centerSumSq += center * center;
    //        ++view_size;
    //    }

    //    // set max variance as axis
    //    centerSum /= view_size;
    //    centerSumSq /= view_size;

    //    vec2 variance = centerSumSq - (centerSum * centerSum);
    //    float maxVar = variance.x;
    //    int maxVarAxis = 0;
    //    if (variance.x > variance.y)
    //    {
    //        maxVar = variance.x;
    //        maxVarAxis = 0;
    //    }
    //    else 
    //    {
    //        maxVar = variance.y;
    //        maxVarAxis = 1;
    //    }
    //    
    //    //std::sort(view.begin(), view.end(), m_broadphaseCompare);

    //    for (auto& [rigidbodyA, colliderA] : view)
    //    {
    //        for (auto& [rigidbodyB, colliderB] : view)
    //        {
    //            if (colliderA.GetEntity() == colliderB.GetEntity()) break;

    //            //auto& secondCollider = colliderB;

    //            float minA, minB;

    //            /*auto TestCollision = [&](auto const& v1)
    //            {
    //                std::visit([&](auto& v2)
    //                    {
    //                    }, secondCollider.collider);
    //            };

    //            std::visit(TestCollision, colliderA.collider);*/

    //            if (minB > minA) break;

    //            m_narrowPhase.emplace_back(colliderA.GetEntity(), colliderB.GetEntity());
    //        }
    //    }

    //    /*for (auto iterA = view.begin(); iterA != view.end(); ++iterA)
    //    {
    //        m_ECS_Manager.GetComponent<Collider2D>(*iterA);

    //        float minA = m_ECS_Manager.GetComponent<Collider2D>(*iterA).GetGlobalBounds().min[m_broadphaseCompare.Axis];

    //        for (auto [rigidbody2, boxCollider2] : view)
    //        {
    //            if (boxCollider.GetEntity() == boxCollider2.GetEntity()) continue;
    //            float minB = boxCollider2.GetGlobalBounds().min[m_broadphaseCompare.Axis];

    //            if (minB > minA) break;

    //            m_narrowPhase.emplace_back(boxCollider.GetEntity(), boxCollider2.GetEntity());
    //        }
    //    }*/
    //}

    //void PhysicsSystem::NarrowPhase()
    //{
    //    for (auto& [entA, entB] : m_narrowPhase)
    //    {
    //        auto& colliderA = m_ECS_Manager.GetComponent<Collider2D>(entA);
    //        auto& colliderB = m_ECS_Manager.GetComponent<Collider2D>(entB);

    //        /*auto TestCollision = [&](auto const& v1)
    //        {
    //            std::visit([&](auto const& v2)
    //                {
    //                    Manifold2D result = PhysicsManifold::Test2DCollision(v1, v2);
    //                    if (result.HasCollision)
    //                    {
    //                        result.ObjA = colliderA.GetComponent<Rigidbody2D>();
    //                        result.ObjB = colliderB.GetComponent<Rigidbody2D>();
    //                        m_collisions.emplace_back(result);
    //                    }
    //                }, colliderB.collider);
    //        };

    //        std::visit(TestCollision, colliderA.collider);*/

    //        Manifold2D result = PhysicsUtils::TestCollision2D(colliderA, colliderB);//collider.TestCollision(&collider2);

    //        LOG_INFO("Collision {0} Normal ({1},{2}) PenDepth {3}", result.HasCollision, result.Normal.x, result.Normal.y, result.PenetrationDepth);

    //        if (result.HasCollision) m_collisions.emplace_back(result);
    //    }
    //}

    //bool SortSweepCompare::operator()(Entity a, Entity b)
    //{
    //    // more correct would be : Manager.GetComponent<Collider2d>().GetBroadPhaseCollider().GetGlobalBounds().Min(Axis);

    //    float aMin = Manager.GetComponent<BoxCollider2D>(a).GetGlobalBounds().min[Axis];
    //    float bMin = Manager.GetComponent<BoxCollider2D>(b).GetGlobalBounds().min[Axis];

    //    return aMin < bMin;
    //}


}
