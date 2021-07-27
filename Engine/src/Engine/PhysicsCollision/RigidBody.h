#pragma once

#include "Engine/ECS/Component.h"
#include "Engine/Transform/Transform3D.h"

#include <glm/glm.hpp>

namespace engine
{
    class RigidBody : public Component
    {
    public:
        bool IsStatic = false;
        
        glm::vec2 Velocity = {};
        glm::vec2 Force = {};

        float Mass = 1.0f;
        float InverseMass = 1.0f/Mass;

        bool UseGravity = true;
        float GravityScale = 1.0f;

        float Restitution = 0.f;
        float DynamicFriction = 0.f;
        float StaticFriction = 0.f;
        
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        RigidBody()                             = delete;
        RigidBody(RigidBody const&)             = default;
        RigidBody(RigidBody &&)                 = default;
        RigidBody& operator=(RigidBody const&)  = default;
        RigidBody& operator=(RigidBody &&)      = default;
        virtual ~RigidBody() override           = default;

        RigidBody(Entity entity, bool active = true);
    };

}
