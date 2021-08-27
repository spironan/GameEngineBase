#pragma once

#include "../vector/vec3.hpp"
#include <limits>
#include <cstddef>

namespace oom
{
    template<typename T>
    struct mat<3, 3, T>
    {
        using col_type          = vec<3, T>;
        using row_type          = vec<3, T>;
        using type              = mat<3, 3, T>;
        using transpose_type    = mat<3, 3, T>;
        using value_type        = T;

    private:
        col_type value[3];

    public:
        // -- Accesses --

        using length_type = std::size_t;
        static constexpr length_type size() { return 3; }

        col_type & operator[](length_type i);
        constexpr col_type const& operator[](length_type i) const;

        // -- Constructors --

        constexpr mat() = default;
        constexpr mat(mat<3, 3, T> const& m);

        explicit constexpr mat(T scalar);
        constexpr mat(
            T x0, T y0, T z0,
            T x1, T y1, T z1,
            T x2, T y2, T z2);
        constexpr mat(
            col_type const& v0,
            col_type const& v1,
            col_type const& v2);

        // -- Conversions --

        template<
            typename X1, typename Y1, typename Z1,
            typename X2, typename Y2, typename Z2,
            typename X3, typename Y3, typename Z3>
        constexpr mat(
            X1 x1, Y1 y1, Z1 z1,
            X2 x2, Y2 y2, Z2 z2,
            X3 x3, Y3 y3, Z3 z3);

        template<typename V1, typename V2, typename V3>
        constexpr mat(
            vec<3, V1> const& v1,
            vec<3, V2> const& v2,
            vec<3, V3> const& v3);

        // -- Matrix conversions --

        template<typename U>
        OOM_EXPLICIT constexpr mat(mat<3, 3, U> const& m);

        OOM_EXPLICIT constexpr mat(mat<2, 2, T> const& x);
        OOM_EXPLICIT constexpr mat(mat<4, 4, T> const& x);
        OOM_EXPLICIT constexpr mat(mat<2, 3, T> const& x);
        OOM_EXPLICIT constexpr mat(mat<3, 2, T> const& x);
        OOM_EXPLICIT constexpr mat(mat<2, 4, T> const& x);
        OOM_EXPLICIT constexpr mat(mat<4, 2, T> const& x);
        OOM_EXPLICIT constexpr mat(mat<3, 4, T> const& x);
        OOM_EXPLICIT constexpr mat(mat<4, 3, T> const& x);

        // -- Unary arithmetic operators --

        template<typename U>
        mat<3, 3, T> & operator=(mat<3, 3, U> const& m);
        template<typename U>
        mat<3, 3, T> & operator+=(U s);
        template<typename U>
        mat<3, 3, T> & operator+=(mat<3, 3, U> const& m);
        template<typename U>
        mat<3, 3, T> & operator-=(U s);
        template<typename U>
        mat<3, 3, T> & operator-=(mat<3, 3, U> const& m);
        template<typename U>
        mat<3, 3, T> & operator*=(U s);
        template<typename U>
        mat<3, 3, T> & operator*=(mat<3, 3, U> const& m);
        template<typename U>
        mat<3, 3, T> & operator/=(U s);
        template<typename U>
        mat<3, 3, T> & operator/=(mat<3, 3, U> const& m);

        // -- Increment and decrement operators --

        mat<3, 3, T> & operator++();
        mat<3, 3, T> & operator--();
        mat<3, 3, T> operator++(int);
        mat<3, 3, T> operator--(int);


        // -- Helper Functions --

        static constexpr mat<3, 3, T> zero() { return mat<3, 3, T>{0}; }

        static constexpr mat<3, 3, T> identity()
        {
            return mat<3, 3, T>
            {
                {1, 0, 0},
                {0, 1, 0},
                {0, 0, 1}
            };
        };

        void SetToZero() { *this = zero(); }
        void SetToIdentity() { *this = identity(); }
        mat<3, 3, T> Transposed() const { return transposed(*this); }
        mat<3, 3, T> Transpose() { return transpose(*this); }
    };

    // -- Unary operators --

    template<typename T>
    mat<3, 3, T> operator+(mat<3, 3, T> const& m);

    template<typename T>
    mat<3, 3, T> operator-(mat<3, 3, T> const& m);

    // -- Binary operators --

    template<typename T>
    mat<3, 3, T> operator+(mat<3, 3, T> const& m, T scalar);

    template<typename T>
    mat<3, 3, T> operator+(T scalar, mat<3, 3, T> const& m);

    template<typename T>
    mat<3, 3, T> operator+(mat<3, 3, T> const& m1, mat<3, 3, T> const& m2);

    template<typename T>
    mat<3, 3, T> operator-(mat<3, 3, T> const& m, T scalar);

    template<typename T>
    mat<3, 3, T> operator-(T scalar, mat<3, 3, T> const& m);

    template<typename T>
    mat<3, 3, T> operator-(mat<3, 3, T> const& m1, mat<3, 3, T> const& m2);

    template<typename T>
    mat<3, 3, T> operator*(mat<3, 3, T> const& m, T scalar);

    template<typename T>
    mat<3, 3, T> operator*(T scalar, mat<3, 3, T> const& m);

    template<typename T>
    typename mat<3, 3, T>::col_type operator*(mat<3, 3, T> const& m, typename mat<3, 3, T>::row_type const& v);

    template<typename T>
    typename mat<3, 3, T>::row_type operator*(typename mat<3, 3, T>::col_type const& v, mat<3, 3, T> const& m);

    template<typename T>
    mat<3, 3, T> operator*(mat<3, 3, T> const& m1, mat<3, 3, T> const& m2);

    template<typename T>
    mat<2, 3, T> operator*(mat<3, 3, T> const& m1, mat<2, 3, T> const& m2);

    template<typename T>
    mat<4, 3, T> operator*(mat<3, 3, T> const& m1, mat<4, 3, T> const& m2);

    template<typename T>
    mat<3, 3, T> operator/(mat<3, 3, T> const& m, T scalar);

    template<typename T>
    mat<3, 3, T> operator/(T scalar, mat<3, 3, T> const& m);

    template<typename T>
    typename mat<3, 3, T>::col_type operator/(mat<3, 3, T> const& m, typename mat<3, 3, T>::row_type const& v);

    template<typename T>
    typename mat<3, 3, T>::row_type operator/(typename mat<3, 3, T>::col_type const& v, mat<3, 3, T> const& m);

    template<typename T>
    mat<3, 3, T> operator/(mat<3, 3, T> const& m1, mat<3, 3, T> const& m2);

    // -- Boolean operators --

    template<typename T>
    constexpr bool operator==(mat<3, 3, T> const& m1, mat<3, 3, T> const& m2);

    template<typename T>
    bool operator!=(mat<3, 3, T> const& m1, mat<3, 3, T> const& m2);
}//namespace oom

#include "mat3x3_impl.hpp"
