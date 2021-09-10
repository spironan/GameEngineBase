#pragma once

#include <type_traits>
#include <cmath>

namespace oom
{
    template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T pi() { return static_cast<T>(3.14159253589793243846); }

    template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T rad_to_deg() { return static_cast<T>(180.0) / pi<T>(); }

    template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T deg_to_rad() { return pi<T>() / static_cast<T>(180.0); }

    template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T radians(T const& degrees) { return degrees * deg_to_rad<T>(); }

    template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T degrees(T const& radians) { return radians * rad_to_deg<T>(); }

    template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T sin(T const& value) { return std::sin(value); };

    template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T cos(T const& value) { return std::cos(value); };

    template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T tan(T const& value) { return std::tan(value); };

    template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T asin(T const& value) { return std::asin(value); };
    
    template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T acos(T const& value) { return std::acos(value); };
    
    template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T atan(T const& value) { return std::atan(value); };
    
    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T atan2(T const& first, T const& second) { return std::atan2(value); };

}
