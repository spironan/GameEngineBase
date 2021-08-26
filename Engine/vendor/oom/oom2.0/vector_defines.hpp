#pragma once

#include "vector/vec1.hpp"
#include "vector/vec2.hpp"
#include "vector/vec3.hpp"
#include "vector/vec4.hpp"

namespace oom
{
    using vec1  = vec<1, float>;
    using dvec1 = vec<1, double>;
    using ivec1 = vec<1, int>;
    using uvec1 = vec<1, unsigned int>;
    using bvec1 = vec<1, bool>;

    using vec2  = vec<2, float>;
    using dvec2 = vec<2, double>;
    using ivec2 = vec<2, int>;
    using uvec2 = vec<2, unsigned int>;
    using bvec2 = vec<2, bool>;

    using vec3  = vec<3, float>;
    using dvec3 = vec<3, double>;
    using ivec3 = vec<3, int>;
    using uvec3 = vec<3, unsigned int>;
    using bvec3 = vec<3, bool>;

    using vec4  = vec<4, float>;
    using dvec4 = vec<4, double>;
    using ivec4 = vec<4, int>;
    using uvec4 = vec<4, unsigned int>;
    using bvec4 = vec<4, bool>;
}
