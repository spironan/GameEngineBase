/************************************************************************************//*!
\file          Rigidbody2D.cpp
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

#include "Rigidbody.h"

#include "Engine/ECS/GameObject.h"

namespace engine
{
    Rigidbody2D::Rigidbody2D(Entity entity, bool active)
        : Component { entity, active }
    {
    }

    void Rigidbody2D::SetMass(float newMass)
    {
        ENGINE_ASSERT_MSG(newMass > 0.f, "Mass canont be lesser then 0!");
        if (newMass < 0.f)
            throw "Mass cannot be lesser than 0!";
        m_mass = newMass; 
        m_inverseMass = 1.0f / m_mass;
    }

    /*void Rigidbody2D::Interpolate(float alpha)
    {
        Transform3D& trans = static_cast<GameObject>(GetEntity()).Transform;
        glm::vec3 previous = m_previoiusPosition * alpha;
        glm::vec3 current = trans.Position() * (1.f - alpha);
        trans.Position() = previous + current;
    }*/

    void Rigidbody2D::ApplyGravity(glm::vec2 gravity)
    {
        AddForce(gravity * m_mass * GravityScale);
    }

    void Rigidbody2D::UpdateVelocity(Timestep dt)
    {
        m_linearVelocity += (m_force / m_mass) * static_cast<float>(dt);

        m_linearVelocity *= 1.f - DynamicFriction;  // is this correct for dynamic friction?
    }

    void Rigidbody2D::UpdatePosition(Timestep dt)
    {
        Transform3D& trans = static_cast<GameObject>(GetEntity()).Transform;
        m_previoiusPosition = trans.GetPosition();
        trans.Position() += glm::vec3{ m_linearVelocity, 0.f } * static_cast<float>(dt);
    }

}
