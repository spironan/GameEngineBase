#pragma once

//#include <glm/glm.hpp>
#include <oom/oom.hpp>

namespace engine
{
    using vec2 = oom::vec2;
    using vec3 = oom::vec3;
    using mat2 = oom::mat2;
    using mat3 = oom::mat3;

    class Rigidbody2D;

    class Collider2D;
    struct BoxCollider2D;
    struct CircleCollider2D;

    struct Manifold2D;
    struct PhysicsMaterial;
    struct MassData;

    struct Solver;
    struct ImpulseSolver;
}
