#pragma once

#include <glm/glm.hpp>

namespace engine
{
    using vec2 = glm::vec2;
    using vec3 = glm::vec3;
    using mat2 = glm::mat2;
    using mat3 = glm::mat3;

    class Collider2D;
    class BoxCollider2D;
    class CircleCollider2D;

    struct Manifold2D;

    class ImpulseSolver;
}
