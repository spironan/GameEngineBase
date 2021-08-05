#pragma once

#include <type_traits>
#include <numeric>

#include "fwd.h"

namespace oom
{
    /*struct maths_utils 
    {
    public:
        maths_utils() = delete;*/

        template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        static constexpr T pi() { return static_cast<T>(3.14159253589793243846); }
        
        //static constexpr double pi = 3.14159253589793243846;
        template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        static constexpr double rad2deg() { return static_cast<T>(180.0) / static_cast<T>(pi); }
        
        template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        static constexpr double deg2rad() { return static_cast<T>(pi) / static_cast<T>(180.0); }

        template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        static constexpr T radians(T degrees) { return degrees * rad2deg; }

        template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        static constexpr T degrees(T radians) { return radians * deg2rad; }


        ///**
        // * Return the smaller of the given values.
        // * @tparam T Value type.
        // * @param _a First value to compare.
        // * @param _b Second value to compare.
        // * @return The smaller of _a and _b. If the values are equivalent, returns _a.
        // */
        //template<class T>
        //static const T& min(const T& _a, const T& _b)
        //{
        //    return (_b < _a) ? _b : _a;
        //}

        ///**
        // * Return the smaller of the given values.
        // * @tparam T Value type.
        // * @tparam Args Variadic template type.
        // * @param _a First value to compare.
        // * @param _b Second value to compare.
        // * @param _args The third to nth number to compare.
        // * @return The smaller of _a and _b. If the values are equivalent, returns _a.
        // */
        //template<class T, class ...Args>
        //static const T& min(const T& _a, const T& _b, Args&& ... _args)
        //{
        //    return maths_utils::min<T>(_a, maths_utils::min<T>(_b, std::forward<Args>(_args)...));
        //}

        ///**
        // * Return the larger of the given values.
        // * @tparam T Value type.
        // * @param _a First value to compare.
        // * @param _b Second value to compare.
        // * @return The larger of _a and _b. If the values are equivalent, returns _a.
        // */
        //template<class T>
        //static const T& max(const T& _a, const T& _b)
        //{
        //    return (_a < _b) ? _b : _a;
        //}

        ///**
        // * Return the larger of the given values.
        // * @tparam T Value type.
        // * @tparam Args Variadic template type.
        // * @param _a First value to compare.
        // * @param _b Second value to compare.
        // * @param _args The third to nth number to compare.
        // * @return The larger of _a and _b. If the values are equivalent, returns _a.
        // */
        //template<class T, class ...Args>
        //static const T& max(const T& _a, const T& _b, Args&& ... _args)
        //{
        //    return maths_utils::max<T>(_a, maths_utils::max<T>(_b, std::forward<Args>(_args)...));
        //}

        /**
         * Checks if 2 floating Point numbers are approximately equal. Useful for dealing with floating Point errors.
         * @tparam T The floating Point type.
         * @param _a The first number to compare.
         * @param _b The second number to compare.
         * @return Returns true if both numbers are approximately equal. Else, returns false.
         */
        template<class T>
        static std::enable_if_t<!std::numeric_limits<T>::is_integer, bool> approx_equal(const T& _a, const T& _b)
        {
            return std::fabs(_a - _b) <= std::numeric_limits<T>::epsilon();
        }
        
        /**
         * Checks if N floating Point numbers are approximately equal. Useful for dealing with floating Point errors.
         * @tparam T The floating Point type.
         * @tparam Args The floating Point type.
         * @param _a The first number to compare.
         * @param _b The second number to compare.
         * @param _args The third to nth number to compare.
         * @return Returns true if all numbers are approximately equal. Else, returns false.
         */
        template<class T, class... Args>
        static std::enable_if_t<!std::numeric_limits<T>::is_integer, bool>
            approx_equal(const T& _a, const T& _b, Args&& ... _args)
        {
            return std::fabs(maths_utils::max<T>(_a, _b, std::forward<Args>(_args)...) -
                maths_utils::min<T>(_a, _b, std::forward<Args>(_args)...)) <= std::numeric_limits<T>::epsilon();
        }
    /*};*/
}