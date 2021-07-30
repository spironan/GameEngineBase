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
#include <glm/glm.hpp>

namespace engine
{
    class Rigidbody2D : public Component
    {
    public:
        bool IsStatic = false;

        bool UseGravity = true;
        float GravityScale = 1.0f;

        float Restitution = 0.f;
        float DynamicFriction = 0.01f;
        float StaticFriction = 0.01f;
        
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
        \brief    Adds a force to the rigidbody
         
        \param    force
                vector indicating the direction and strength of the force to apply onto
                this object
        *//**********************************************************************************/
        void AddForce(glm::vec2 force) { m_force += force; }
        
        /*********************************************************************************//*!
        \brief    Set the mass of the rigidbody with new mass
        
        \throw    
                 throws when the mass is lesser then 0.

        \param    newMass
                The new mass to assign this rigidbody's mass to.
        *//**********************************************************************************/
        void SetMass(float newMass); 
        /*********************************************************************************//*!
        \brief    Retrieve the current mass of the rigidbody
         
        \return   the current mass of the rigidbody.
        *//**********************************************************************************/
        float GetMass() const { return m_mass; }

        float GetInverseMass() const { return m_inverseMass; }

        /*********************************************************************************//*!
        \brief    Set the velocity of the rigidbody with new velocity.

        \param    newVel
                The new velocity to assign this rigidbody's velocity to.
        *//**********************************************************************************/
        void SetVelocity(glm::vec2 newVel) { m_linearVelocity = newVel; }
        /*********************************************************************************//*!
        \brief    Retrieve the current velocity of the rigidbody

        \return   the current velocity of the rigidbody.
        *//**********************************************************************************/
        glm::vec2 GetVelocity() const { return m_linearVelocity; }


        /*********************************************************************************//*!
        \brief    Set the force of the rigidbody with new force.

        \param    newForce
                The new force to assign this rigidbody's force to.
        *//**********************************************************************************/
        void SetForce(glm::vec2 newForce) { m_force = newForce; }
        /*********************************************************************************//*!
        \brief    Retrieve the current force of the rigidbody

        \return   the current force of the rigidbody.
        *//**********************************************************************************/
        glm::vec2 GetForce() const { return m_force; }

    private:

        glm::vec3 m_previoiusPosition = { 0.f, 0.f, 0.f };

        glm::vec2 m_linearVelocity = { 0.f, 0.f };
        glm::vec2 m_force = { 0.f, 0.f };

        float m_mass = 1.0f;
        float m_inverseMass = 1.0f / m_mass;

        friend class PhysicsSystem;

        //void Interpolate(float alpha);

        /*********************************************************************************//*!
        \brief    Apply Gravity to object. Should only be called By Physics System.

        \param    gravity
                    Gravity of the world to apply to this rigidbody.
        *//**********************************************************************************/
        void ApplyGravity(glm::vec2 gravity);

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

}
