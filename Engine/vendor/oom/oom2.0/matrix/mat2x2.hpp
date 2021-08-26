#pragma once

#include "../vector/vec2.hpp"
#include <limits>
#include <cstddef>

namespace oom
{
    template<typename T>
    struct mat<2, 2, T>
    {
        using col_type          = vec<2, T>;
        using row_type          = vec<2, T>;
        using type              = mat<2, 2, T>;
        using transpose_type    = mat<2, 2, T>;
        using value_type        = T ;

    private:
        col_type value[2];

    public:
        // -- Accesses --

        using length_type = std::size_t;
        static constexpr length_type size() { return 2; }

        col_type & operator[](length_type i);
        constexpr col_type const& operator[](length_type i) const;

        // -- Constructors --

        constexpr mat() = default;
        
        constexpr mat(mat<2, 2, T> const& m);

        explicit constexpr mat(T scalar);
        constexpr mat(
            T const& x1, T const& y1,
            T const& x2, T const& y2);
        constexpr mat(
            col_type const& v1,
            col_type const& v2);

        // -- Conversions --

        template<typename U, typename V, typename M, typename N>
        constexpr mat(
            U const& x1, V const& y1,
            M const& x2, N const& y2);

        template<typename U, typename V>
        constexpr mat(
            vec<2, U> const& v1,
            vec<2, V> const& v2);

        // -- Matrix conversions --

        template<typename U>
        explicit constexpr mat(mat<2, 2, U> const& m);

        explicit constexpr mat(mat<3, 3, T> const& x);
        explicit constexpr mat(mat<4, 4, T> const& x);
        explicit constexpr mat(mat<2, 3, T> const& x);
        explicit constexpr mat(mat<3, 2, T> const& x);
        explicit constexpr mat(mat<2, 4, T> const& x);
        explicit constexpr mat(mat<4, 2, T> const& x);
        explicit constexpr mat(mat<3, 4, T> const& x);
        explicit constexpr mat(mat<4, 3, T> const& x);

        // -- Unary arithmetic operators --

        template<typename U>
        mat<2, 2, T> & operator=(mat<2, 2, U> const& m);
        template<typename U>
        mat<2, 2, T> & operator+=(U s);
        template<typename U>
        mat<2, 2, T> & operator+=(mat<2, 2, U> const& m);
        template<typename U>
        mat<2, 2, T> & operator-=(U s);
        template<typename U>
        mat<2, 2, T> & operator-=(mat<2, 2, U> const& m);
        template<typename U>
        mat<2, 2, T> & operator*=(U s);
        template<typename U>
        mat<2, 2, T> & operator*=(mat<2, 2, U> const& m);
        template<typename U>
        mat<2, 2, T> & operator/=(U s);
        template<typename U>
        mat<2, 2, T> & operator/=(mat<2, 2, U> const& m);

        // -- Increment and decrement operators --

        mat<2, 2, T> & operator++ ();
        mat<2, 2, T> & operator-- ();
        mat<2, 2, T> operator++(int);
        mat<2, 2, T> operator--(int);

        // -- Helper Functions --

        static constexpr mat<2, 2, T> zero() { return mat<2, 2, T>{0}; }

        static constexpr mat<2, 2, T> identity()
        {
            return mat<2, 2, T>
            {
                {1, 0},
                {0, 1}
            };
        };

        void SetToZero()        { *this = zero(); }
        void SetToIdentity()    { *this = identity(); }
        mat<2, 2, T> Transposed() const { return transposed(*this); }
        mat<2, 2, T> Transpose()        { return transpose(*this); }

    };

    // -- Unary operators --

    template<typename T>
    mat<2, 2, T> operator+(mat<2, 2, T> const& m);

    template<typename T>
    mat<2, 2, T> operator-(mat<2, 2, T> const& m);

    // -- Binary operators --

    template<typename T>
    mat<2, 2, T> operator+(mat<2, 2, T> const& m, T scalar);

    template<typename T>
    mat<2, 2, T> operator+(T scalar, mat<2, 2, T> const& m);

    template<typename T>
    mat<2, 2, T> operator+(mat<2, 2, T> const& m1, mat<2, 2, T> const& m2);

    template<typename T>
    mat<2, 2, T> operator-(mat<2, 2, T> const& m, T scalar);

    template<typename T>
    mat<2, 2, T> operator-(T scalar, mat<2, 2, T> const& m);

    template<typename T>
    mat<2, 2, T> operator-(mat<2, 2, T> const& m1, mat<2, 2, T> const& m2);

    template<typename T>
    mat<2, 2, T> operator*(mat<2, 2, T> const& m, T scalar);

    template<typename T>
    mat<2, 2, T> operator*(T scalar, mat<2, 2, T> const& m);

    template<typename T>
    typename mat<2, 2, T>::col_type operator*(mat<2, 2, T> const& m, typename mat<2, 2, T>::row_type const& v);

    template<typename T>
    typename mat<2, 2, T>::row_type operator*(typename mat<2, 2, T>::col_type const& v, mat<2, 2, T> const& m);

    template<typename T>
    mat<2, 2, T> operator*(mat<2, 2, T> const& m1, mat<2, 2, T> const& m2);

    template<typename T>
    mat<3, 2, T> operator*(mat<2, 2, T> const& m1, mat<3, 2, T> const& m2);

    template<typename T>
    mat<4, 2, T> operator*(mat<2, 2, T> const& m1, mat<4, 2, T> const& m2);

    template<typename T>
    mat<2, 2, T> operator/(mat<2, 2, T> const& m, T scalar);

    template<typename T>
    mat<2, 2, T> operator/(T scalar, mat<2, 2, T> const& m);

    template<typename T>
    typename mat<2, 2, T>::col_type operator/(mat<2, 2, T> const& m, typename mat<2, 2, T>::row_type const& v);

    template<typename T>
    typename mat<2, 2, T>::row_type operator/(typename mat<2, 2, T>::col_type const& v, mat<2, 2, T> const& m);

    template<typename T>
    mat<2, 2, T> operator/(mat<2, 2, T> const& m1, mat<2, 2, T> const& m2);

    // -- Boolean operators --

    template<typename T>
    bool operator==(mat<2, 2, T> const& m1, mat<2, 2, T> const& m2);

    template<typename T>
    bool operator!=(mat<2, 2, T> const& m1, mat<2, 2, T> const& m2);
} //namespace oom

#include "mat2x2_impl.hpp"
