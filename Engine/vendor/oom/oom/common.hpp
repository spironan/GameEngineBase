#pragma once

#include <limits>
#include <type_traits>
#include <cmath>

#include "common.hpp"

namespace oom
{
    template<typename T, typename = std::enable_if<std::is_signed_v<T>>>
    static constexpr T abs(T value)
    {
        if constexpr (std::is_floating_point_v<T>)
            return std::fabs(value);
        else
            return std::abs(value);
    }

    template<typename T, typename = std::enable_if<std::is_signed_v<T>>>
    static constexpr T sign(T value)
    {
        if (value < T{ 0 }) return T{ -1 };
        else if (value > T{ 0 })  return T{ 1 };
        return T{ 0 };
    }


    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T floor(T value)
    {
        return std::floor(value);
    }

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T ceil(T value)
    {
        return std::ceil(value);
    }

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T trunc(T value)
    {
        return std::trunc(value);
    }

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>>>
    static constexpr T round(T value)
    {
        return std::round(value);
    }
    
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>>>
    static constexpr T clamp(T x, T minVal, T maxVal)
    {
        return min(max(x, minVal), maxVal);
    }

    template<length_t length, typename T>
    static constexpr vec<length, T> clamp(vec<length, T> const& x, vec<length, T> const& minVal, vec<length, T> const& maxVal)
    {
        return min(max(x, minVal), maxVal);
    }


    /**
       * Return the smaller of the given values.
       * @tparam T Value type.
       * @param a First value to compare.
       * @param b Second value to compare.
       * @return The smaller of a and b. If the values are equivalent, returns a.
       */
    template<typename T>
    static constexpr T min(T a, T b)
    {
        return (b < a) ? b : a;
    }

    template<length_t length, typename T>
    static constexpr vec<length, T> min(vec<length, T> const& a, vec<length, T> const& b)
    {
        vec<length, T> result;
        
        for (length_t len = 0; len < length; ++len)
            result[len] = b[len] < a[len] ? b[len] : a[len];
        
        return result;
    }

    /**
        * Return the smaller of the given values.
        * @tparam T Value type.
        * @tparam Args Variadic template type.
        * @param a First value to compare.
        * @param b Second value to compare.
        * @param args The third to nth number to compare.
        * @return The smaller of a and b. If the values are equivalent, returns a.
        */
    template<typename T, typename ...Args>
    static constexpr T min(T a, T b, Args&& ... args)
    {
        return min(a, min(b, std::forward<Args>(args)...));
    }

    /**
        * Return the larger of the given values.
        * @tparam T Value type.
        * @param a First value to compare.
        * @param b Second value to compare.
        * @return The larger of a and b. If the values are equivalent, returns a.
        */
    template<typename T>
    static constexpr T max(T a, T b)
    {
        return (a < b) ? b : a;
    }

    template<length_t length, typename T>
    static constexpr vec<length, T> max(vec<length, T> const& a, vec<length, T> const& b)
    {
        vec<length, T> result;

        for (length_t len = 0; len < length; ++len)
            result[len] = a[len] < b[len] ? b[len] : a[len];

        return result;
    }

    /**
        * Return the larger of the given values.
        * @tparam T Value type.
        * @tparam Args Variadic template type.
        * @param a First value to compare.
        * @param b Second value to compare.
        * @param args The third to nth number to compare.
        * @return The larger of a and b. If the values are equivalent, returns a.
        */
    template<typename T, typename ...Args>
    static constexpr T max(T a, T b, Args&& ... args)
    {
        return max(a, max(b, std::forward<Args>(args)...));
    }

    /**
        * Checks if 2 floating Point numbers are approximately equal. Useful for dealing with floating Point errors.
        * @tparam T The floating Point type.
        * @param a The first number to compare.
        * @param b The second number to compare.
        * @return Returns true if both numbers are approximately equal. Else, returns false.
        */
    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>>>
    static bool equals(T a, T b)
    {
        if constexpr (std::is_floating_point_v<T>)
            return std::fabs(a - b) <= std::numeric_limits<T>::epsilon();
        else
            return a == b;
    }

    /**
        * Checks if N floating Point numbers are approximately equal. Useful for dealing with floating Point errors.
        * @tparam T The floating Point type.
        * @tparam Args The floating Point type.
        * @param a The first number to compare.
        * @param b The second number to compare.
        * @param args The third to nth number to compare.
        * @return Returns true if all numbers are approximately equal. Else, returns false.
        */
    template<typename T, typename... Args>
    static std::enable_if_t<std::is_floating_point_v<T>, bool>
        equals(T a, T b, Args&& ... args)
    {
        return std::fabs(max<T>(a, b, std::forward<Args>(args)...) -
            min<T>(a, b, std::forward<Args>(args)...)) <= std::numeric_limits<T>::epsilon();
    }
}
