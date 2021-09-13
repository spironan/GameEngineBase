/************************************************************************************//*!
\file           Rigidbody2D.cpp
\project        <PROJECT_NAME>
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           July 27, 2021
\brief          Rigidbody Component describes the gameobject that has it to
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
#include "Engine/ECS/WorldManager.h"
#include "Engine/ECS/GameObject.h"

#include "ColliderCore.h"
#include "Colliders.h"

#include <rttr/registration>

namespace engine
{
    RTTR_REGISTRATION
    {
        using namespace rttr;
        registration::class_<PhysicsMaterial>("PhysicsMaterial")
            .property("Density", &PhysicsMaterial::Density)
            .property("Restitution", &PhysicsMaterial::Restitution)
            .property("DynamicFriction", &PhysicsMaterial::DynamicFriction)
            .property("StaticFriction", &PhysicsMaterial::StaticFriction);

        registration::class_<Rigidbody2D>("Rigidbody2D")
            .enumeration<BodyType>("BodyType")(value("STATIC", BodyType::STATIC), value("KINEMATIC", BodyType::KINEMATIC), value("DYNAMIC", BodyType::DYNAMIC))
            .property("BodyType", &Rigidbody2D::BodyType)
            .property("UseAutoMass", &Rigidbody2D::UseAutoMass)
            .property("GravityScale", &Rigidbody2D::GravityScale)
            .property("Mass", &Rigidbody2D::GetMass, &Rigidbody2D::SetMass)
            .property("Material", &Rigidbody2D::GetMaterial, &Rigidbody2D::SetMaterial);
    }

    Rigidbody2D::Rigidbody2D(Entity entity, bool active)
        : Component { entity, active }
        , m_material { }
        , m_data { }
        , m_linearVelocity { }
        , m_force { }
        , m_prevPos { static_cast<GameObject>(entity).Transform().GetPosition() }
        , m_angularVelocity { }
        , m_torque {}
    {
    }

    void Rigidbody2D::SetMass(float newMass)
    {
        ENGINE_ASSERT_MSG(newMass > 0.f, "Mass canont be lesser then 0!");
        if (newMass < 0.f) throw "Mass cannot be lesser than 0!";
        
        //sets both mass and inverse mass to 0
        m_data.Mass = 0.f;
        m_data.InverseMass = 0.f;

        // Set Rotational Involvement to 0
        m_data.Inertia = 0.f;
        m_data.InverseInertia = 0.f;

        if (newMass != 0.f)
        {
            m_data.Mass = newMass; 
            m_data.InverseMass = 1.0f / m_data.Mass;

            if (!HasComponent<Collider2D>()) return;

            // Set Moment of Inertia base on shape of object
            switch(GetComponent<Collider2D>().GetNarrowPhaseCollider())
            {
            case ColliderType::BOX:
                {
                    auto bounds = ColliderUtil::GetGlobalDimensions(GetComponent<BoxCollider2D>(), GetComponent<Transform3D>());
                    m_data.Inertia = m_data.Mass * (bounds.x * bounds.x + bounds.y * bounds.y)/ 12;
                    m_data.InverseInertia = 1.0f / m_data.Inertia;
                }
                break;
            case ColliderType::CIRCLE:
                {
                    m_data.Inertia = 0.5f * m_data.Mass * ColliderUtil::GetGlobalRadius(GetComponent<CircleCollider2D>(), GetComponent<Transform3D>());
                    m_data.InverseInertia = 1.0f / m_data.Inertia;
                }
                break;
            }
        }

    }

    void Rigidbody2D::SetAutoMass(bool useAutoMass)
    {
        UseAutoMass = useAutoMass;
        if (UseAutoMass)    // if Use Auto Mass is set to true.
        {
            // Use Density for now, should be Mass = Density * Volume
            SetMass(m_material.Density);
        }
    }

    void Rigidbody2D::Interpolate(float alpha)
    {
        Transform3D& trans = static_cast<GameObject>(GetEntity()).Transform();
        oom::vec3 previous = m_prevPos * alpha;
        oom::vec3 current = trans.GetPosition() * (1.f - alpha);
        trans.Position() = previous + current;

        m_prevPos = trans.GetPosition();
    }

    /*void Rigidbody2D::ApplyGravity(oom::vec2 gravity)
    {
        ApplyForce(gravity * m_mass * GravityScale);
    }*/

    void Rigidbody2D::UpdateVelocity(Timestep dt)
    {
        // linear acceleration = force / mass
        m_linearVelocity += (m_force * m_data.InverseMass) * static_cast<float>(dt);

        // angular acceleration = torque / inertia 
        m_angularVelocity += m_torque * m_data.InverseInertia * static_cast<float>(dt);
    }

    void Rigidbody2D::UpdatePosition(Timestep dt)
    {
        Transform3D& trans = static_cast<GameObject>(GetEntity()).Transform();
        //m_previoiusPosition = trans.GetPosition();
        trans.Position() += oom::vec3{ m_linearVelocity, 0.f } * static_cast<float>(dt);
        trans.RotationAngle() += oom::radians(m_angularVelocity * static_cast<float>(dt));
    }

    float Rigidbody2D_GetGravityScale(Entity instanceID)
    {
        GameObject obj{ instanceID };
        return obj.GetComponent<Rigidbody2D>().GravityScale;
    }

    void Rigidbody2D_SetGravityScale(Entity instanceID, float value)
    {
        GameObject obj{ instanceID };
        obj.GetComponent<Rigidbody2D>().GravityScale = value;
    }
}
