#pragma once

#include "../fwd.hpp"

namespace oom
{
    template<typename T>
    struct vec<3, T>
    {
        // -- Implementation detail --

        using value_type    = T;
        using type          = vec<3, T>;
        using bool_type     = vec<3, bool>;

        // -- Data --

        union { T x, r, s; };
        union { T y, g, t; };
        union { T z, b, p; };


        // -- Component accesses --

        /// Return the count of components of the vector
        using length_type = std::size_t;
        static constexpr length_type size(){return 3;}

        constexpr T & operator[](length_type i);
        constexpr T const& operator[](length_type i) const;

        // -- Implicit basic constructors --

        constexpr vec() = default;
        constexpr vec(vec const& v) = default;
        
        // -- Explicit basic constructors --

        constexpr explicit vec(T scalar);
        constexpr vec(T a, T b, T c);

        // -- Conversion scalar constructors --

        template<typename U>
        constexpr explicit vec(vec<1, U> const& v);

        /// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
        template<typename X, typename Y, typename Z>
        constexpr vec(X x, Y y, Z z);
        template<typename X, typename Y, typename Z>
        constexpr vec(vec<1, X> const& _x, Y _y, Z _z);
        template<typename X, typename Y, typename Z>
        constexpr vec(X _x, vec<1, Y> const& _y, Z _z);
        template<typename X, typename Y, typename Z>
        constexpr vec(vec<1, X> const& _x, vec<1, Y> const& _y, Z _z);
        template<typename X, typename Y, typename Z>
        constexpr vec(X _x, Y _y, vec<1, Z> const& _z);
        template<typename X, typename Y, typename Z>
        constexpr vec(vec<1, X> const& _x, Y _y, vec<1, Z> const& _z);
        template<typename X, typename Y, typename Z>
        constexpr vec(X _x, vec<1, Y> const& _y, vec<1, Z> const& _z);
        template<typename X, typename Y, typename Z>
        constexpr vec(vec<1, X> const& _x, vec<1, Y> const& _y, vec<1, Z> const& _z);

        // -- Conversion vector constructors --

        /// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
        template<typename A, typename B>
        constexpr vec(vec<2, A> const& _xy, B _z);
        /// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
        template<typename A, typename B>
        constexpr vec(vec<2, A> const& _xy, vec<1, B> const& _z);
        /// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
        template<typename A, typename B>
        constexpr vec(A _x, vec<2, B> const& _yz);
        /// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
        template<typename A, typename B>
        constexpr vec(vec<1, A> const& _x, vec<2, B> const& _yz);
        /// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
        template<typename U>
        constexpr OOM_EXPLICIT vec(vec<4, U> const& v);

        /// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
        template<typename U>
        constexpr OOM_EXPLICIT vec(vec<3, U> const& v);

        // -- Unary arithmetic operators --

        constexpr vec<3, T>& operator=(vec<3, T> const& v) = default;

        template<typename U>
        constexpr vec<3, T> & operator=(vec<3, U> const& v);
        template<typename U>
        constexpr vec<3, T> & operator+=(U scalar);
        template<typename U>
        constexpr vec<3, T> & operator+=(vec<1, U> const& v);
        template<typename U>
        constexpr vec<3, T> & operator+=(vec<3, U> const& v);
        template<typename U>
        constexpr vec<3, T> & operator-=(U scalar);
        template<typename U>
        constexpr vec<3, T> & operator-=(vec<1, U> const& v);
        template<typename U>
        constexpr vec<3, T> & operator-=(vec<3, U> const& v);
        template<typename U>
        constexpr vec<3, T> & operator*=(U scalar);
        template<typename U>
        constexpr vec<3, T> & operator*=(vec<1, U> const& v);
        template<typename U>
        constexpr vec<3, T> & operator*=(vec<3, U> const& v);
        template<typename U>
        constexpr vec<3, T> & operator/=(U scalar);
        template<typename U>
        constexpr vec<3, T> & operator/=(vec<1, U> const& v);
        template<typename U>
        constexpr vec<3, T> & operator/=(vec<3, U> const& v);

        // -- Increment and decrement operators --

        constexpr vec<3, T> & operator++();
        constexpr vec<3, T> & operator--();
        constexpr vec<3, T> operator++(int);
        constexpr vec<3, T> operator--(int);

        // -- helper functions -- 

        T Dot(vec<3, T> const& other) const;
        T SignedArea(vec<3, T> const& other) const;
        vec<3,T> Cross(vec<3, T> const& other) const;
        T Length2() const;
        T Length() const;
        T Distance2(vec<3, T> const& other) const;
        T Distance(vec<3, T> const& other) const;
        vec<3, T> Normalized() const;
        vec<3, T>& Normalize();
    };

    // -- Unary operators --

    template<typename T>
    constexpr vec<3, T> operator+(vec<3, T> const& v);

    template<typename T>
    constexpr vec<3, T> operator-(vec<3, T> const& v);

    // -- Binary operators --

    template<typename T>
    constexpr vec<3, T> operator+(vec<3, T> const& v, T scalar);

    template<typename T>
    constexpr vec<3, T> operator+(vec<3, T> const& v, vec<1, T> const& scalar);

    template<typename T>
    constexpr vec<3, T> operator+(T scalar, vec<3, T> const& v);

    template<typename T>
    constexpr vec<3, T> operator+(vec<1, T> const& v1, vec<3, T> const& v2);

    template<typename T>
    constexpr vec<3, T> operator+(vec<3, T> const& v1, vec<3, T> const& v2);

    template<typename T>
    constexpr vec<3, T> operator-(vec<3, T> const& v, T scalar);

    template<typename T>
    constexpr vec<3, T> operator-(vec<3, T> const& v1, vec<1, T> const& v2);

    template<typename T>
    constexpr vec<3, T> operator-(T scalar, vec<3, T> const& v);

    template<typename T>
    constexpr vec<3, T> operator-(vec<1, T> const& v1, vec<3, T> const& v2);

    template<typename T>
    constexpr vec<3, T> operator-(vec<3, T> const& v1, vec<3, T> const& v2);

    template<typename T>
    constexpr vec<3, T> operator*(vec<3, T> const& v, T scalar);

    template<typename T>
    constexpr vec<3, T> operator*(vec<3, T> const& v1, vec<1, T> const& v2);

    template<typename T>
    constexpr vec<3, T> operator*(T scalar, vec<3, T> const& v);

    template<typename T>
    constexpr vec<3, T> operator*(vec<1, T> const& v1, vec<3, T> const& v2);

    template<typename T>
    constexpr vec<3, T> operator*(vec<3, T> const& v1, vec<3, T> const& v2);

    template<typename T>
    constexpr vec<3, T> operator/(vec<3, T> const& v, T scalar);

    template<typename T>
    constexpr vec<3, T> operator/(vec<3, T> const& v1, vec<1, T> const& v2);

    template<typename T>
    constexpr vec<3, T> operator/(T scalar, vec<3, T> const& v);

    template<typename T>
    constexpr vec<3, T> operator/(vec<1, T> const& v1, vec<3, T> const& v2);

    template<typename T>
    constexpr vec<3, T> operator/(vec<3, T> const& v1, vec<3, T> const& v2);


    // -- Boolean operators --

    template<typename T>
    constexpr bool operator==(vec<3, T> const& v1, vec<3, T> const& v2);

    template<typename T>
    constexpr bool operator!=(vec<3, T> const& v1, vec<3, T> const& v2);
    
}//namespace oom

#include "vec3_impl.hpp"
