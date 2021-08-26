#pragma once

#include <type_traits>
#include <cmath>

namespace oom
{
    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T pi() { return static_cast<T>(3.14159253589793243846); }

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr double rad_to_deg() { return static_cast<T>(180.0) / static_cast<T>(pi); }

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr double deg_to_rad() { return static_cast<T>(pi) / static_cast<T>(180.0); }

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T radians(T degrees) { return degrees * rad_to_deg; }

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T degrees(T radians) { return radians * deg_to_rad; }

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T sin(T value) { return std::sin(value); };

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T cos(T value) { return std::cos(value); };

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T tan(T value) { return std::tan(value); };

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T asin(T value) { return std::asin(value); };
    
    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T acos(T value) { return std::acos(value); };
    
    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T atan(T value) { return std::atan(value); };
    
    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T atan2(T first, T second) { return std::atan2(value); };

}
