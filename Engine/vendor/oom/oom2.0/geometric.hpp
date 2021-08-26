#pragma once

#include "vector/vec2.hpp"
#include "vector/vec3.hpp"

namespace oom
{
    template<length_t Size, typename T>
    T dot(vec<Size, T> const& first, vec<Size, T> const& second)
    {
        T result{ 0 };
        for (length_t i = 0; i < Size; ++i)
        {
            result += first[i] * second[i];
        }
        return result;
    }

    template<typename T>
    T cross(vec<2, T> const& first, vec<2, T> const& second)
    {
        return first.x * second.y - first.y * second.x;
    }

    template<typename T>
    T signed_area(vec<3, T> const& first, vec<3, T> const& second)
    {
        return first.x * second.y - first.y * second.x;
    }

    template<typename T>
    vec<3, T> cross(vec<3, T> const& first, vec<3, T> const& second)
    {
        vec<3, T> result{};

        result.x = first.y * second.z - first.z * second.y;
        result.y = first.z * second.x - first.x * second.z;
        result.z = first.x * second.y - first.y * second.x;

        return result;
    }

    template<length_t Size, typename T>
    T length2(vec<Size, T> const& vec)
    {
        return dot(vec, vec);
    }

    template<length_t Size, typename T>
    T length(vec<Size, T> const& vec)
    {
        return sqrtf(length2(vec));
    }

    template<length_t Size, typename T>
    T distance2(vec<Size, T> const& first, vec<Size, T> const& second)
    {
        return length2(first - second);
    }

    template<length_t Size, typename T>
    T distance(vec<Size, T> const& first, vec<Size, T> const& second)
    {
        return length(first - second);
    }

    template<length_t Size, typename T>
    vec<Size, T> normalized(vec<Size, T> const& vec)
    {
        return vec / length(vec);
    }

    template<length_t Size, typename T>
    vec<Size, T>& normalize(vec<Size, T>& vec)
    {
        vec = normalized(vec);
        return vec;
    }

}//namespace oom
