/************************************************************************************//*!
\file          Rigidbody2D.h
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
#pragma once

#include "Engine/Core/Base.h"
#include "Engine/ECS/Component.h"
#include "Engine/Transform/Transform3D.h"
#include "Engine/Core/Timestep.h"
//#include <glm/glm.hpp>
#include <oom/oom.hpp>

#include <rttr/type>

namespace engine
{
    enum class BodyType : int
    {
        STATIC,
        KINEMATIC,
        DYNAMIC
    };

    struct MassData
    {
        float Mass = 1.0f;
        float InverseMass = 1.0f;

        // for rotations
        float Inertia = 0.f;
        float InverseInertia = 0.f;

        RTTR_ENABLE();
    };

    struct PhysicsMaterial
    {
        float Density           = 1.0f;
        float Restitution       = 0.f;
        float DynamicFriction   = 0.6f;
        float StaticFriction    = 0.6f;

        RTTR_ENABLE();
    };

    class Rigidbody2D : public Component
    {
    public:
        BodyType BodyType = BodyType::DYNAMIC;
        
        bool UseAutoMass = true;
        float GravityScale = 1.0f;
        
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        Rigidbody2D()                               = delete;
        Rigidbody2D(Rigidbody2D const&)             = default;
        Rigidbody2D(Rigidbody2D &&)                 = default;
        Rigidbody2D& operator=(Rigidbody2D const&)  = default;
        Rigidbody2D& operator=(Rigidbody2D &&)      = default;
        virtual ~Rigidbody2D() override             = default;

        Rigidbody2D(Entity entity, bool active = true);
        

        /*********************************************************************************//*!
        \brief    Applies a force to the rigidbody
         
        \warning  Only moves the object if its dynamic.

        \param    force
                vector indicating the direction and strength of the force to apply onto
                this object. 
        *//**********************************************************************************/
        void ApplyForce(oom::vec2 force) { if(IsDynamic()) m_force += force; }
        
        /*********************************************************************************//*!
        \brief    Applies a velocity to the rigidbody

        \warning  Only moves the object if its kinematic.

        \param    force
                vector indicating the direction and strength of the force to apply onto
                this object's velocity
        *//**********************************************************************************/
        void ApplyVelocity(oom::vec2 velocity) { if (IsKinematic()) m_linearVelocity += velocity; }

        /*********************************************************************************//*!
        \brief    Set the mass of the rigidbody with new mass
        
        \throw    throws when the mass is lesser then 0.

        \param    newMass
                The new mass to assign this rigidbody's mass to.
        *//**********************************************************************************/
        void SetMass(float newMass); 

        /*********************************************************************************//*!
        \brief    Retrieve the current mass of the rigidbody
         
        \return   the current mass of the rigidbody.
        *//**********************************************************************************/
        float GetMass() const { return m_data.Mass; }

        float GetInverseMass() const { return m_data.InverseMass; }

        /*********************************************************************************//*!
        \brief    Set the velocity of the rigidbody with new velocity.

        \param    newVel
                The new velocity to assign this rigidbody's velocity to.
        *//**********************************************************************************/
        void SetVelocity(oom::vec2 newVel) { m_linearVelocity = newVel; }

        /*********************************************************************************//*!
        \brief    Retrieve the current velocity of the rigidbody

        \return   the current velocity of the rigidbody.
        *//**********************************************************************************/
        oom::vec2 GetVelocity() const { return m_linearVelocity; }
        
        /*********************************************************************************//*!
        \brief    Set the force of the rigidbody with new force.

        \param    newForce
                The new force to assign this rigidbody's force to.
        *//**********************************************************************************/
        void SetForce(oom::vec2 newForce) { m_force = newForce; }

        /*********************************************************************************//*!
        \brief    Retrieve the current force of the rigidbody

        \return   the current force of the rigidbody.
        *//**********************************************************************************/
        oom::vec2 GetForce() const { return m_force; }
        
        bool IsStatic()     const { return BodyType == BodyType::STATIC; }
        bool IsKinematic()  const { return BodyType == BodyType::KINEMATIC; }
        bool IsDynamic()    const { return BodyType == BodyType::DYNAMIC; }

        PhysicsMaterial GetMaterial() const         { return m_material; }
        void SetMaterial(PhysicsMaterial material)  { m_material = material; }

        void SetAutoMass(bool useAutoMass);
        
        RTTR_ENABLE();

    private:

        PhysicsMaterial m_material;

        MassData m_data;

        oom::vec2 m_linearVelocity;
        oom::vec2 m_force;

        //// Angular components
        //float m_orientation;
        //float m_angularVelocity;
        //float m_torque;

        oom::vec3 m_prevPos;

        friend class PhysicsSystem;

        void Interpolate(float alpha);

        /*********************************************************************************//*!
        \brief    Apply Gravity to object. Should only be called By Physics System.

        \param    gravity
                    Gravity of the world to apply to this rigidbody.
        *//**********************************************************************************/
        void ApplyGravity(oom::vec2 gravity);

        /*********************************************************************************//*!
        \brief    Updates the Velocity of the object with forces.

        \param    dt
                the amount of time that has past since previous frame.
        *//**********************************************************************************/
        void UpdateVelocity(Timestep dt);
        /*********************************************************************************//*!
        \brief    Updates the Position of the object with velocity.

        \param    dt
                the amount of time that has past since previous frame.
        *//**********************************************************************************/
        void UpdatePosition(Timestep dt);
    };

    extern "C"
    {
        __declspec(dllexport) float Rigidbody2D_GetGravityScale(Entity instanceID);
        __declspec(dllexport) void Rigidbody2D_SetGravityScale(Entity instanceID, float value);
    }
}
