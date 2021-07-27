#pragma once

#include "Engine/ECS/System.h"
#include "Engine/Core/Timestep.h"
#include <glm/glm.hpp>

namespace engine
{
    class PhysicsSystem : public System
    {
    public:
        PhysicsSystem(ECS_Manager& ECS_Manager) 
            : System { ECS_Manager } 
            , Gravity { 0.f, -9.81f }
        {};

        void Update(Timestep deltaTime);
        
        glm::vec2 Gravity;

    private:
        void UpdateDynamics(Timestep deltaTime);
        void UpdatePhysicsCollision();
        void UpdatePhysicsResolution();

    };
}
