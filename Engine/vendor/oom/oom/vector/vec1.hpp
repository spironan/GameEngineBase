#pragma once

#include "../fwd.hpp"
#include <cstddef>

namespace oom
{
    template<typename T>
    struct vec<1, T>
    {
        // -- Implementation detail --

        using value_type    = T;
        using type          = vec<1, T>;
        using bool_type     = vec<1, bool>;

        // -- Data --
        union {T x, r, s;};

        // -- Component accesses --

        /// Return the count of components of the vector
        using length_type = std::size_t;
        static constexpr length_type size(){return 1;}

        constexpr T & operator[](length_type i);
        constexpr T const& operator[](length_type i) const;

        // -- Implicit basic constructors --

        constexpr vec() = default;
        constexpr vec(vec const& v) = default;
        
        // -- Explicit basic constructors --

        constexpr explicit vec(T scalar);

        // -- Conversion vector constructors --

        /// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
        template<typename U>
        constexpr OOM_EXPLICIT vec(vec<2, U> const& v);
        /// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
        template<typename U>
        constexpr OOM_EXPLICIT vec(vec<3, U> const& v);
        /// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
        template<typename U>
        constexpr OOM_EXPLICIT vec(vec<4, U> const& v);

        /// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
        template<typename U>
        constexpr OOM_EXPLICIT vec(vec<1, U> const& v);

        // -- Unary arithmetic operators --

        constexpr vec<1, T> & operator=(vec const& v) = default;

        template<typename U>
        constexpr vec<1, T> & operator=(vec<1, U> const& v);
        template<typename U>
        constexpr vec<1, T> & operator+=(U scalar);
        template<typename U>
        constexpr vec<1, T> & operator+=(vec<1, U> const& v);
        template<typename U>
        constexpr vec<1, T> & operator-=(U scalar);
        template<typename U>
        constexpr vec<1, T> & operator-=(vec<1, U> const& v);
        template<typename U>
        constexpr vec<1, T> & operator*=(U scalar);
        template<typename U>
        constexpr vec<1, T> & operator*=(vec<1, U> const& v);
        template<typename U>
        constexpr vec<1, T> & operator/=(U scalar);
        template<typename U>
        constexpr vec<1, T> & operator/=(vec<1, U> const& v);

        // -- Increment and decrement operators --

        constexpr vec<1, T> & operator++();
        constexpr vec<1, T> & operator--();
        constexpr vec<1, T> operator++(int);
        constexpr vec<1, T> operator--(int);

        // -- helper functions -- 

        T Dot(vec<1, T> const& other) const;
        T Length2() const;
        T Distance2(vec<1, T> const& other) const;

    };

    // -- Unary operators --

    template<typename T>
    constexpr vec<1, T> operator+(vec<1, T> const& v);

    template<typename T>
    constexpr vec<1, T> operator-(vec<1, T> const& v);

    // -- Binary operators --

    template<typename T>
    constexpr vec<1, T> operator+(vec<1, T> const& v, T scalar);

    template<typename T>
    constexpr vec<1, T> operator+(T scalar, vec<1, T> const& v);

    template<typename T>
    constexpr vec<1, T> operator+(vec<1, T> const& v1, vec<1, T> const& v2);

    template<typename T>
    constexpr vec<1, T> operator-(vec<1, T> const& v, T scalar);

    template<typename T>
    constexpr vec<1, T> operator-(T scalar, vec<1, T> const& v);

    template<typename T>
    constexpr vec<1, T> operator-(vec<1, T> const& v1, vec<1, T> const& v2);

    template<typename T>
    constexpr vec<1, T> operator*(vec<1, T> const& v, T scalar);

    template<typename T>
    constexpr vec<1, T> operator*(T scalar, vec<1, T> const& v);

    template<typename T>
    constexpr vec<1, T> operator*(vec<1, T> const& v1, vec<1, T> const& v2);

    template<typename T>
    constexpr vec<1, T> operator/(vec<1, T> const& v, T scalar);

    template<typename T>
    constexpr vec<1, T> operator/(T scalar, vec<1, T> const& v);

    template<typename T>
    constexpr vec<1, T> operator/(vec<1, T> const& v1, vec<1, T> const& v2);

    // -- Boolean operators --

    template<typename T>
    constexpr bool operator==(vec<1, T> const& v1, vec<1, T> const& v2);

    template<typename T>
    constexpr bool operator!=(vec<1, T> const& v1, vec<1, T> const& v2);
    
}//namespace oom

#include "vec1_impl.hpp"
