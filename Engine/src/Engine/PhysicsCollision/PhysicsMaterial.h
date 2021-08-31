#pragma once

namespace engine
{
    struct PhysicsMaterial
    {
        float Density           = 1.0f;
        float Restitution       = 0.f;
        float DynamicFriction   = 0.6f;
        float StaticFriction    = 0.6f;
    };
}
