#pragma once

#include "../fwd.hpp"

namespace oom
{
    template<typename T>
    struct vec<2, T>
    {
        // -- Implementation detail --

        using value_type    = T;
        using type          = vec<2, T>;
        using bool_type     = vec<2, bool>;

        // -- Data --

        union {T x, r, s;};
        union {T y, g, t;};

        // -- Component accesses --

        /// Return the count of components of the vector
        using length_type = std::size_t;
        static constexpr length_type size(){return 2;}

        constexpr T& operator[](length_type i);
        constexpr T const& operator[](length_type i) const;

        // -- Implicit basic constructors --

        constexpr vec() = default;
        constexpr vec(vec const& v) = default;
        
        // -- Explicit basic constructors --

        constexpr explicit vec(T scalar);
        constexpr vec(T x, T y);

        // -- Conversion constructors --

        template<typename U>
        constexpr explicit vec(vec<1, U> const& v);

        /// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
        template<typename A, typename B>
        constexpr vec(A x, B y);
        template<typename A, typename B>
        constexpr vec(vec<1, A> const& x, B y);
        template<typename A, typename B>
        constexpr vec(A x, vec<1, B> const& y);
        template<typename A, typename B>
        constexpr vec(vec<1, A> const& x, vec<1, B> const& y);

        // -- Conversion vector constructors --

        /// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
        template<typename U>
        constexpr OOM_EXPLICIT vec(vec<3, U> const& v);
        /// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
        template<typename U>
        constexpr OOM_EXPLICIT vec(vec<4, U> const& v);

        /// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
        template<typename U>
        constexpr OOM_EXPLICIT vec(vec<2, U> const& v);

        // -- Unary arithmetic operators --

        constexpr vec<2, T> & operator=(vec const& v) = default;

        template<typename U>
        constexpr vec<2, T> & operator=(vec<2, U> const& v);
        template<typename U>
        constexpr vec<2, T> & operator+=(U scalar);
        template<typename U>
        constexpr vec<2, T> & operator+=(vec<1, U> const& v);
        template<typename U>
        constexpr vec<2, T> & operator+=(vec<2, U> const& v);
        template<typename U>
        constexpr vec<2, T> & operator-=(U scalar);
        template<typename U>
        constexpr vec<2, T> & operator-=(vec<1, U> const& v);
        template<typename U>
        constexpr vec<2, T> & operator-=(vec<2, U> const& v);
        template<typename U>
        constexpr vec<2, T> & operator*=(U scalar);
        template<typename U>
        constexpr vec<2, T> & operator*=(vec<1, U> const& v);
        template<typename U>
        constexpr vec<2, T> & operator*=(vec<2, U> const& v);
        template<typename U>
        constexpr vec<2, T> & operator/=(U scalar);
        template<typename U>
        constexpr vec<2, T> & operator/=(vec<1, U> const& v);
        template<typename U>
        constexpr vec<2, T> & operator/=(vec<2, U> const& v);

        // -- Increment and decrement operators --

        constexpr vec<2, T> & operator++();
        constexpr vec<2, T> & operator--();
        constexpr vec<2, T> operator++(int);
        constexpr vec<2, T> operator--(int);

        // -- helper functions -- 

        T Dot(vec<2, T> const& other) const;
        T Cross(vec<2, T> const& other) const;
        T Length2() const;
        T Length() const;
        T Distance2(vec<2, T> const& other) const;
        T Distance(vec<2, T> const& other) const;
        vec<2, T> Normalized() const;
        vec<2, T>& Normalize();

    };

    // -- Unary operators --

    template<typename T>
    constexpr vec<2, T> operator+(vec<2, T> const& v);

    template<typename T>
    constexpr vec<2, T> operator-(vec<2, T> const& v);

    // -- Binary operators --

    template<typename T>
    constexpr vec<2, T> operator+(vec<2, T> const& v, T scalar);

    template<typename T>
    constexpr vec<2, T> operator+(vec<2, T> const& v1, vec<1, T> const& v2);

    template<typename T>
    constexpr vec<2, T> operator+(T scalar, vec<2, T> const& v);

    template<typename T>
    constexpr vec<2, T> operator+(vec<1, T> const& v1, vec<2, T> const& v2);

    template<typename T>
    constexpr vec<2, T> operator+(vec<2, T> const& v1, vec<2, T> const& v2);

    template<typename T>
    constexpr vec<2, T> operator-(vec<2, T> const& v, T scalar);

    template<typename T>
    constexpr vec<2, T> operator-(vec<2, T> const& v1, vec<1, T> const& v2);

    template<typename T>
    constexpr vec<2, T> operator-(T scalar, vec<2, T> const& v);

    template<typename T>
    constexpr vec<2, T> operator-(vec<1, T> const& v1, vec<2, T> const& v2);

    template<typename T>
    constexpr vec<2, T> operator-(vec<2, T> const& v1, vec<2, T> const& v2);

    template<typename T>
    constexpr vec<2, T> operator*(vec<2, T> const& v, T scalar);

    template<typename T>
    constexpr vec<2, T> operator*(vec<2, T> const& v1, vec<1, T> const& v2);

    template<typename T>
    constexpr vec<2, T> operator*(T scalar, vec<2, T> const& v);

    template<typename T>
    constexpr vec<2, T> operator*(vec<1, T> const& v1, vec<2, T> const& v2);

    template<typename T>
    constexpr vec<2, T> operator*(vec<2, T> const& v1, vec<2, T> const& v2);

    template<typename T>
    constexpr vec<2, T> operator/(vec<2, T> const& v, T scalar);

    template<typename T>
    constexpr vec<2, T> operator/(vec<2, T> const& v1, vec<1, T> const& v2);

    template<typename T>
    constexpr vec<2, T> operator/(T scalar, vec<2, T> const& v);

    template<typename T>
    constexpr vec<2, T> operator/(vec<1, T> const& v1, vec<2, T> const& v2);

    template<typename T>
    constexpr vec<2, T> operator/(vec<2, T> const& v1, vec<2, T> const& v2);

    // -- Boolean operators --
    template<typename T>
    constexpr bool operator==(vec<2, T> const& v1, vec<2, T> const& v2);
    
    template<typename T>
    constexpr bool operator!=(vec<2, T> const& v1, vec<2, T> const& v2);
    
    

}//namespace oom

#include "vec2_impl.hpp"
