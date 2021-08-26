#pragma once

#include "../vector/vec2.hpp"
#include "../vector/vec4.hpp"
#include <limits>
#include <cstddef>

namespace oom
{
    template<typename T>
    struct mat<4, 2, T>
    {
        using col_type          = vec<2, T>;
        using row_type          = vec<4, T>;
        using type              = mat<4, 2, T>;
        using transpose_type    = mat<2, 4, T>;
        using value_type        = T;

    private:
        col_type value[4];

    public:
        // -- Accesses --

        using length_type = std::size_t;
        static constexpr length_type size() { return 4; }

        col_type & operator[](length_type i);
        constexpr col_type const& operator[](length_type i) const;

        // -- Constructors --

        constexpr mat() = default;
        constexpr mat(mat<4, 2, T> const& m);

        explicit constexpr mat(T scalar);
        constexpr mat(
            T x0, T y0,
            T x1, T y1,
            T x2, T y2,
            T x3, T y3);
        constexpr mat(
            col_type const& v0,
            col_type const& v1,
            col_type const& v2,
            col_type const& v3);

        // -- Conversions --

        template<
            typename X0, typename Y0,
            typename X1, typename Y1,
            typename X2, typename Y2,
            typename X3, typename Y3>
        constexpr mat(
            X0 x0, Y0 y0,
            X1 x1, Y1 y1,
            X2 x2, Y2 y2,
            X3 x3, Y3 y3);

        template<typename V1, typename V2, typename V3, typename V4>
        constexpr mat(
            vec<2, V1> const& v1,
            vec<2, V2> const& v2,
            vec<2, V3> const& v3,
            vec<2, V4> const& v4);

        // -- Matrix conversions --

        template<typename U>
        explicit constexpr mat(mat<4, 2, U> const& m);

        explicit constexpr mat(mat<2, 2, T> const& x);
        explicit constexpr mat(mat<3, 3, T> const& x);
        explicit constexpr mat(mat<4, 4, T> const& x);
        explicit constexpr mat(mat<2, 3, T> const& x);
        explicit constexpr mat(mat<3, 2, T> const& x);
        explicit constexpr mat(mat<2, 4, T> const& x);
        explicit constexpr mat(mat<4, 3, T> const& x);
        explicit constexpr mat(mat<3, 4, T> const& x);

        // -- Unary arithmetic operators --

        template<typename U>
        mat<4, 2, T> & operator=(mat<4, 2, U> const& m);
        template<typename U>
        mat<4, 2, T> & operator+=(U s);
        template<typename U>
        mat<4, 2, T> & operator+=(mat<4, 2, U> const& m);
        template<typename U>
        mat<4, 2, T> & operator-=(U s);
        template<typename U>
        mat<4, 2, T> & operator-=(mat<4, 2, U> const& m);
        template<typename U>
        mat<4, 2, T> & operator*=(U s);
        template<typename U>
        mat<4, 2, T> & operator/=(U s);

        // -- Increment and decrement operators --

        mat<4, 2, T> & operator++ ();
        mat<4, 2, T> & operator-- ();
        mat<4, 2, T> operator++(int);
        mat<4, 2, T> operator--(int);
        
        // -- Helper Functions --

        static constexpr mat<4, 2, T> zero() { return mat<4, 2, T>{0}; }

        void SetToZero() { *this = zero(); }

        mat<2, 4, T> Transposed() const { return transposed(*this); }
    };

    // -- Unary operators --

    template<typename T>
    mat<4, 2, T> operator+(mat<4, 2, T> const& m);

    template<typename T>
    mat<4, 2, T> operator-(mat<4, 2, T> const& m);

    // -- Binary operators --

    template<typename T>
    mat<4, 2, T> operator+(mat<4, 2, T> const& m, T scalar);

    template<typename T>
    mat<4, 2, T> operator+(mat<4, 2, T> const& m1, mat<4, 2, T> const& m2);

    template<typename T>
    mat<4, 2, T> operator-(mat<4, 2, T> const& m, T scalar);

    template<typename T>
    mat<4, 2, T> operator-(mat<4, 2, T> const& m1,	mat<4, 2, T> const& m2);

    template<typename T>
    mat<4, 2, T> operator*(mat<4, 2, T> const& m, T scalar);

    template<typename T>
    mat<4, 2, T> operator*(T scalar, mat<4, 2, T> const& m);

    template<typename T>
    typename mat<4, 2, T>::col_type operator*(mat<4, 2, T> const& m, typename mat<4, 2, T>::row_type const& v);

    template<typename T>
    typename mat<4, 2, T>::row_type operator*(typename mat<4, 2, T>::col_type const& v, mat<4, 2, T> const& m);

    template<typename T>
    mat<2, 2, T> operator*(mat<4, 2, T> const& m1, mat<2, 4, T> const& m2);

    template<typename T>
    mat<3, 2, T> operator*(mat<4, 2, T> const& m1, mat<3, 4, T> const& m2);

    template<typename T>
    mat<4, 2, T> operator*(mat<4, 2, T> const& m1, mat<4, 4, T> const& m2);

    template<typename T>
    mat<4, 2, T> operator/(mat<4, 2, T> const& m, T scalar);

    template<typename T>
    mat<4, 2, T> operator/(T scalar, mat<4, 2, T> const& m);

    // -- Boolean operators --

    template<typename T>
    bool operator==(mat<4, 2, T> const& m1, mat<4, 2, T> const& m2);

    template<typename T>
    bool operator!=(mat<4, 2, T> const& m1, mat<4, 2, T> const& m2);
}//namespace oom

#include "mat4x2_impl.hpp"
