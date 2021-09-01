/************************************************************************************//*!
\file          RigidBody.cpp
\project       <PROJECT_NAME>
\author        Chua Teck Lee, c.tecklee, 390008420
\par           email: c.tecklee\@digipen.edu
\date          July 27, 2021
\brief         Rigidbody Component describes the gameobject that has it to
               have physics related dynamics (gravity, restitution, forces etc)
               and could result itself or others to have physics based
               responses (collision response)

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"

#include "RigidBody.h"

#include "Engine/ECS/GameObject.h"

namespace engine
{
    RigidBody::RigidBody(Entity entity, bool active)
        : Component { entity, active }
    {
    }

    void RigidBody::SetMass(float newMass)
    {
        ENGINE_ASSERT_MSG(newMass > 0.f, "Mass canont be lesser then 0!");
        if (newMass < 0.f)
            throw "Mass cannot be lesser than 0!";
        m_mass = newMass; 
        m_inverseMass = 1.0f / m_mass;
    }

    void RigidBody::ApplyGravity(oom::vec2 gravity)
    {
        AddForce(gravity * m_mass * GravityScale);
    }

    void RigidBody::UpdateVelocity(Timestep dt)
    {
        m_velocity += m_force / m_mass * static_cast<float>(dt);
    }

    void RigidBody::UpdatePosition(Timestep dt)
    {
        static_cast<GameObject>(GetEntity()).Transform().Position() += oom::vec3{ m_velocity, 0.f } *static_cast<float>(dt);
    }

}
