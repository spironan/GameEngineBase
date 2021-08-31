#pragma once

#include <glm/glm.hpp>
//#include <oom/oom.hpp>

namespace engine
{
    using vec2 = glm::vec2;
    using vec3 = glm::vec3;
    using mat2 = glm::mat2;
    using mat3 = glm::mat3;

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
