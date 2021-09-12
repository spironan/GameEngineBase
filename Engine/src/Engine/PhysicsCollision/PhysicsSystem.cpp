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
                rb.SetTorque(0.f);
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
        //Broadphase Collection : Using Sort And Sweep Algorithm
        BroadPhase();

        //NarrowPhase Stringest Test
        NarrowPhase();

        //Update Callbacks
        UpdateCallbacks();

        //Generate Manifold : Rigidbody only. If trigger : 2 flags and set them
        //_mm_rsqrt_ss

        //auto view = m_ECS_Manager.GetComponentView<Rigidbody2D, Collider2D, CollisionInfo>();
        //
        //for (auto& [rigidbodyA, colliderA, collisionInfoA]: view)
        //{
        //    for (auto& [rigidbodyB, colliderB, collisionInfoB]: view)
        //    {
        //        // skip same entity and everything onwards
        //        if (colliderA.GetEntity() == colliderB.GetEntity()) break;
        //        
        //        if (PhysicsUtils::TestCollision2D(colliderA, colliderB))
        //        {
        //            if (colliderA.IsTrigger || colliderB.IsTrigger)
        //            {
        //                if (colliderA.IsTrigger)
        //                {
        //                    collisionInfoA.m_triggers.emplace(colliderB.GetEntity(), colliderB);
        //                }
        //                if (colliderB.IsTrigger)
        //                {
        //                    collisionInfoB.m_triggers.emplace(colliderA.GetEntity(), colliderA);
        //                }
        //            }
        //            else
        //            {
        //                Manifold2D result = PhysicsUtils::GenerateManifold2D(colliderA, colliderB);
        //                result.ObjA = &rigidbodyA;
        //                result.ObjB = &rigidbodyB;
        //                m_collisions.emplace_back(result);

        //                collisionInfoA.m_collisions.emplace(colliderB.GetEntity(), result);
        //                collisionInfoB.m_collisions.emplace(colliderA.GetEntity(), result);
        //            }
        //        }
        //        
        //    }
        //}

    }

    // Attempting Sort and Sweep and Prune for AABBs
    void PhysicsSystem::BroadPhase()
    {
        // Clear Previous Frame Data
        m_narrowPhaseColliders.clear();
        m_narrowPhaseTriggers.clear();

        // INTENTIONAL COPY! do not want to mess with the actual array
        auto dense = m_ECS_Manager.GetComponentDenseArray<BoundingVolume>();

        /*int count = 0;
        for (auto& broadColliderA : dense)
        {
            auto& [transformA, colliderA] = m_ECS_Manager.GetComponents<Transform3D, Collider2D>(broadColliderA.GetEntity());
            AABB2D result = PhysicsUtils::MakeCollider(broadColliderA, transformA, colliderA);

            LOG_ENGINE_INFO("original map entry {0}, Min ({1}, {2}), Max({3}, {4})", count++, result.min.x, result.min.y, result.max.x, result.max.y);
        }*/
        std::sort(dense.begin(), dense.end(), m_broadphaseCompare);
        /*count = 0;
        for (auto& broadColliderA : dense)
        {
            auto& [transformA, colliderA] = m_ECS_Manager.GetComponents<Transform3D, Collider2D>(broadColliderA.GetEntity());
            AABB2D result = PhysicsUtils::MakeCollider(broadColliderA, transformA, colliderA);

            LOG_ENGINE_INFO("After Sorted map entry {0}, Min({1}, {2}), Max({3}, {4})", count++, result.min.x, result.min.y, result.max.x, result.max.y);
        }*/

        //// not guaranteed to work
        //auto& broadphaseView = m_ECS_Manager.GetComponentView<Transform3D, BoundingVolume, Collider2D>();
        
        for (auto iterA = dense.begin(); iterA != dense.end(); ++iterA)
        {
            for (auto iterB = iterA + 1; iterB != dense.end(); ++iterB)
            {
                // Later Object
                auto& [transformA, colliderA] = m_ECS_Manager.GetComponents<Transform3D, Collider2D>(iterA->GetEntity());
                float maxA = PhysicsUtils::MakeCollider(*iterA, transformA, colliderA).max[m_broadphaseCompare.Axis];

                // Earlier Object 
                auto& [transformB, colliderB] = m_ECS_Manager.GetComponents<Transform3D, Collider2D>(iterB->GetEntity());
                float minB = PhysicsUtils::MakeCollider(*iterB, transformB, colliderB).min[m_broadphaseCompare.Axis];

                // Early out condition : should happen relatively often
                if (minB > maxA)
                    break;

                // Extra Check for categorizing collision : Trigger or Collision
                if (colliderA.IsTrigger || colliderB.IsTrigger)
                    m_narrowPhaseTriggers.emplace_back(colliderA, colliderB);
                else
                    m_narrowPhaseColliders.emplace_back(colliderA, colliderB);
            }
        }
        auto size = (m_narrowPhaseColliders.size() + m_narrowPhaseTriggers.size());
        LOG_ENGINE_INFO("Total potential overlaps(broadphase) {0} on axis {1} ", size, m_broadphaseCompare.Axis);


        // Calculate new best axis: putting this code here means its 1 frame behind in accuracy

        //Finding max variance
        vec2 centerSum{ 0.f }, centerSumSq{ 0.f };

        auto& view = m_ECS_Manager.GetComponentView<Transform3D>();

        size_t view_size = 0;
        for (auto& [transform] : view)
        {
            vec2 center = transform.GetGlobalPosition();
            centerSum += center;
            centerSumSq += center * center;
            ++view_size;
        }

        // set max variance as axis
        centerSum /= view_size;
        centerSumSq /= view_size;

        vec2 variance = centerSumSq - (centerSum * centerSum);
        float maxVar = variance.x;
        int maxVarAxis = 0;
        if (variance.x > variance.y)
        {
            maxVar = variance.x;
            maxVarAxis = 0; // x-axis chosen
        }
        else
        {
            maxVar = variance.y;
            maxVarAxis = 1; // y-axis chosen
        }
        // Set new max variance axis
        m_broadphaseCompare.Axis = maxVarAxis;

    }

    //narrowphase detection.
    void PhysicsSystem::NarrowPhase()
    {
        // Stringent Physics Collision Detection : both object will have rigidbody
        for (auto& [colliderA, colliderB] : m_narrowPhaseColliders)
        {
            if (PhysicsUtils::TestCollision2D(colliderA, colliderB))
            {
                auto& collisionInfoA = m_ECS_Manager.GetComponent<CollisionInfo>(colliderA.GetEntity());
                auto& collisionInfoB = m_ECS_Manager.GetComponent<CollisionInfo>(colliderB.GetEntity());

                Manifold2D result = PhysicsUtils::GenerateManifold2D(colliderA, colliderB);
                result.ObjA = &colliderA.GetComponent<Rigidbody2D>();
                result.ObjB = &colliderB.GetComponent<Rigidbody2D>();
                m_collisions.emplace_back(result);

                collisionInfoA.m_collisions.emplace(colliderB.GetEntity(), result);
                collisionInfoB.m_collisions.emplace(colliderA.GetEntity(), result);
            }
        }

        // narrowphase checks : one or both objects are triggers, meaning triggers only check
        for (auto& [triggerA, triggerB] : m_narrowPhaseTriggers)
        {
            if (PhysicsUtils::TestCollision2D(triggerA, triggerB))
            {
                auto& collisionInfoA = m_ECS_Manager.GetComponent<CollisionInfo>(triggerA.GetEntity());
                auto& collisionInfoB = m_ECS_Manager.GetComponent<CollisionInfo>(triggerB.GetEntity());

                collisionInfoA.m_triggers.emplace(triggerB.GetEntity(), triggerB);
                collisionInfoB.m_triggers.emplace(triggerA.GetEntity(), triggerA);
            }
        }
    }

    void PhysicsSystem::UpdateCallbacks()
    {
        auto view = m_ECS_Manager.GetComponentView<Collider2D, CollisionInfo>();

        // Update all collision callbacks
        for (auto& [collider, collisionInfo] : view)
        {
            if (collider.IsTrigger)
            {
                for (auto const& [entity, trigger] : collisionInfo.m_triggers)
                {
                    // if previously not collided with this object
                    if (collisionInfo.m_previousTriggers.find(entity) == collisionInfo.m_previousTriggers.end())
                        collisionInfo.OnTriggerEnter(trigger);
                    else
                        collisionInfo.OnTriggerStay(trigger);
                }

                for (auto const& [entity, prevTrigger] : collisionInfo.m_previousTriggers)
                {
                    // if currently not trigger with this object
                    if (collisionInfo.m_triggers.find(entity) == collisionInfo.m_triggers.end())
                        collisionInfo.OnTriggerExit(prevTrigger);
                }
            }
            else
            {
                for (auto const& [entity, collider] : collisionInfo.m_collisions)
                {
                    // if previously not collided with this object
                    if (collisionInfo.m_previousCollisions.find(entity) == collisionInfo.m_previousCollisions.end())
                        collisionInfo.OnCollisionEnter(collider);
                    else
                        collisionInfo.OnCollisionStay(collider);
                }

                for (auto const& [entity, prevTrigger] : collisionInfo.m_previousCollisions)
                {
                    // if currently not colliding with this object
                    if (collisionInfo.m_collisions.find(entity) == collisionInfo.m_collisions.end())
                        collisionInfo.OnCollisionExit(prevTrigger);
                }
            }

            //Do both outside so that you can freely swap between them
            collisionInfo.m_previousTriggers.clear();
            collisionInfo.m_previousTriggers = std::move(collisionInfo.m_triggers);
            collisionInfo.m_triggers.clear();

            collisionInfo.m_previousCollisions.clear();
            collisionInfo.m_previousCollisions = std::move(collisionInfo.m_collisions);
            collisionInfo.m_collisions.clear();
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

    bool SortSweepCompare::operator()(BoundingVolume a, BoundingVolume b)
    {
        auto& [transformA, colliderA] = Manager.GetComponents<Transform3D, Collider2D>(a.GetEntity());
        float minA = PhysicsUtils::MakeCollider(a, transformA, colliderA).min[Axis];

        auto& [transformB, colliderB] = Manager.GetComponents<Transform3D, Collider2D>(b.GetEntity());
        float minB = PhysicsUtils::MakeCollider(b, transformB, colliderB).min[Axis];

        return minA < minB;
    }


}
