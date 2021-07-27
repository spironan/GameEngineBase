#include "pch.h"

#include "RigidBody.h"

#include "Engine/ECS/GameObject.h"

namespace engine
{
    RigidBody::RigidBody(Entity entity, bool active)
        : Component { entity, active }
    {
    }
}
