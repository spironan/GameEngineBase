#pragma once

#include <cmath>
#include <limits>

namespace oom
{
    template<typename T, typename U, std::enable_if_t<std::is_floating_point_v<T> && (std::is_floating_point_v<U> || std::numeric_limits<T>::is_integer)>>
    static constexpr T power(T base, U exponent) { return std::power(base, exponent); };

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T exp(T value) { return std::exp(value); };

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T log(T value) { return std::log(value); };

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T exp2(T value) { return std::exp2(value); };

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T log2(T value) { return std::log2 (value); };

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T sqrt(T value) { return std::sqrt(value); };

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T inverse_sqrt(T value) { return static_cast<T>(1) / std::sqrt(value); };

}
