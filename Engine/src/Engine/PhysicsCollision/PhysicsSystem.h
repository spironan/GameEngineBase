/************************************************************************************//*!
\file          PhysicsSystem.h
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
#pragma once

#include "Engine/ECS/System.h"
#include "Engine/Core/Timestep.h"
//#include <glm/glm.hpp>
#include <oom/oom.hpp>

namespace engine
{
    class PhysicsSystem : public System
    {
    public:
        /*********************************************************************************//*!
        \brief    Default constructor
         
        \param    ECS_Manager
                  The Manager that controls of the systems
        *//**********************************************************************************/
        PhysicsSystem(ECS_Manager& ECS_Manager) 
            : System { ECS_Manager } 
            , Gravity { 0.f, -9.81f }
        {};


        /*********************************************************************************//*!
        \brief    Updates the Physics System everyframe
         
        \param    deltaTime
                    Amount of time passed since the previous frame.
        *//**********************************************************************************/
        void Update(Timestep deltaTime);
        
        oom::vec2 Gravity;

    private:
        void UpdateDynamics(Timestep deltaTime);
        void UpdatePhysicsCollision();
        void UpdatePhysicsResolution();

    };
}
