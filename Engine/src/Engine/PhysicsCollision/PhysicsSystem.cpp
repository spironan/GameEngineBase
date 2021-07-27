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
                rb.Force += rb.Mass * Gravity * rb.GravityScale;
                rb.Velocity += rb.Force / rb.Mass * static_cast<float>(deltaTime);
            }

            static_cast<engine::GameObject>(rb.GetEntity()).Transform.Position() 
                += glm::vec3{ rb.Velocity, 0.f } * static_cast<float>(deltaTime);

            rb.Force = glm::vec2{};
        }
    }

    void PhysicsSystem::UpdatePhysicsCollision()
    {

    }

    void PhysicsSystem::UpdatePhysicsResolution()
    {

    }

}
