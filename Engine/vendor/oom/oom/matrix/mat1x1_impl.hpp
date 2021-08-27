#pragma once

namespace oom
{
    // -- Constructors --

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<1, 1, T> const& m)
        : value{ m[0] }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(T scalar)
        : value{ col_type(scalar) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat
    (
        T const& x
    )
        : value{ col_type(x0) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(col_type const& v0)
        : value{ v0 }
    {
    }

    // -- Conversion constructors --

    template<typename T>
    template<typename U>
    constexpr mat<1, 1, T>::mat
    (
        U const& x
    )
        : value{ col_type(static_cast<T>(x)) }
    {
    }

    template<typename T>
    template<typename V1>
    constexpr mat<1, 1, T>::mat(vec<1, V1> const& v1)
        : value{ col_type(v1) }
    {
    }

    // -- mat1x1 matrix conversions --

    template<typename T>
    template<typename U>
    constexpr mat<1, 1, T>::mat(mat<1, 1, U> const& m)
        : value{ col_type(m[0]) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<2, 2, T> const& m)
        : value{ col_type(m[0]) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<3, 3, T> const& m)
        : value{ col_type(m[0]) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<4, 4, T> const& m)
        : value{ col_type(m[0]) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<1, 2, T> const& m)
        : value{ col_type(m[0]) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<2, 1, T> const& m)
        : value{ col_type(m[0]) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<1, 3, T> const& m)
        : value{ col_type(m[0]) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<3, 1, T> const& m)
        : value{ col_type(m[0]) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<1, 4, T> const& m)
        : value{ col_type(m[0]) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<4, 1, T> const& m)
        : value{ col_type(m[0]) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<2, 3, T> const& m)
        : value{ col_type(m[0]) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<3, 2, T> const& m)
        : value{ col_type(m[0]) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<2, 4, T> const& m)
        : value{ col_type(m[0]) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<4, 2, T> const& m)
        : value{ col_type(m[0]) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<3, 4, T> const& m)
        : value{ col_type(m[0]) }
    {
    }

    template<typename T>
    constexpr mat<1, 1, T>::mat(mat<4, 3, T> const& m)
        : value{ col_type(m[0]) }
    {
    }

    // -- Accesses --

    template<typename T>
    typename mat<1, 1, T>::col_type& mat<1, 1, T>::operator[](typename mat<1, 1, T>::length_type i)
    {
        assert(i < this->size());
        return this->value[i];
    }

    template<typename T>
    constexpr typename mat<1, 1, T>::col_type const& mat<1, 1, T>::operator[](typename mat<1, 1, T>::length_type i) const
    {
        assert(i < this->size());
        return this->value[i];
    }

    // -- Unary updatable operators --

    template<typename T>
    template<typename U>
    mat<1, 1, T>& mat<1, 1, T>::operator=(mat<1, 1, U> const& m)
    {
        this->value[0] = m[0];
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<1, 1, T>& mat<1, 1, T>::operator+=(U scalar)
    {
        this->value[0] += scalar;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<1, 1, T>& mat<1, 1, T>::operator+=(mat<1, 1, U> const& m)
    {
        this->value[0] += m[0];
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<1, 1, T>& mat<1, 1, T>::operator-=(U scalar)
    {
        this->value[0] -= scalar;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<1, 1, T>& mat<1, 1, T>::operator-=(mat<1, 1, U> const& m)
    {
        this->value[0] -= m[0];
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<1, 1, T>& mat<1, 1, T>::operator*=(U scalar)
    {
        this->value[0] *= scalar;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<1, 1, T>& mat<1, 1, T>::operator*=(mat<1, 1, U> const& m)
    {
        return (*this = *this * m);
    }

    template<typename T>
    template<typename U>
    mat<1, 1, T>& mat<1, 1, T>::operator/=(U scalar)
    {
        this->value[0] /= scalar;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<1, 1, T>& mat<1, 1, T>::operator/=(mat<1, 1, U> const& m)
    {
        return *this *= inverse(m);
    }

    // -- Increment and decrement operators --

    template<typename T>
    mat<1, 1, T>& mat<1, 1, T>::operator++()
    {
        ++this->value[0];
        return *this;
    }

    template<typename T>
    mat<1, 1, T>& mat<1, 1, T>::operator--()
    {
        --this->value[0];
        return *this;
    }

    template<typename T>
    mat<1, 1, T> mat<1, 1, T>::operator++(int)
    {
        mat<1, 1, T> Result(*this);
        ++* this;
        return Result;
    }

    template<typename T>
    mat<1, 1, T> mat<1, 1, T>::operator--(int)
    {
        mat<1, 1, T> Result(*this);
        --* this;
        return Result;
    }

    // -- Unary arithmetic operators --

    template<typename T>
    mat<1, 1, T> operator+(mat<1, 1, T> const& m)
    {
        return m;
    }

    template<typename T>
    mat<1, 1, T> operator-(mat<1, 1, T> const& m)
    {
        return mat<1, 1, T>(
            -m[0]);
    }

    // -- Binary arithmetic operators --

    template<typename T>
    mat<1, 1, T> operator+(mat<1, 1, T> const& m, T scalar)
    {
        return mat<1, 1, T>(
            m[0] + scalar);
    }

    template<typename T>
    mat<1, 1, T> operator+(T scalar, mat<1, 1, T> const& m)
    {
        return mat<1, 1, T>(
            m[0] + scalar);
    }

    template<typename T>
    mat<1, 1, T> operator+(mat<1, 1, T> const& m1, mat<1, 1, T> const& m2)
    {
        return mat<1, 1, T>(
            m1[0] + m2[0]);
    }

    template<typename T>
    mat<1, 1, T> operator-(mat<1, 1, T> const& m, T scalar)
    {
        return mat<1, 1, T>(
            m[0] - scalar);
    }

    template<typename T>
    mat<1, 1, T> operator-(T scalar, mat<1, 1, T> const& m)
    {
        return mat<1, 1, T>(
            scalar - m[0]);
    }

    template<typename T>
    mat<1, 1, T> operator-(mat<1, 1, T> const& m1, mat<1, 1, T> const& m2)
    {
        return mat<1, 1, T>(
            m1[0] - m2[0]);
    }

    template<typename T>
    mat<1, 1, T> operator*(mat<1, 1, T> const& m, T scalar)
    {
        return mat<1, 1, T>(
            m[0] * scalar);
    }

    template<typename T>
    mat<1, 1, T> operator*(T scalar, mat<1, 1, T> const& m)
    {
        return mat<1, 1, T>(
            m[0] * scalar);
    }

    template<typename T>
    typename mat<1, 1, T>::col_type operator*
        (
            mat<1, 1, T> const& m,
            typename mat<1, 1, T>::row_type const& v
            )
    {
        return vec<2, T>(
            m[0][0] * v.x + m[1][0] * v.y);
    }

    template<typename T>
    typename mat<1, 1, T>::row_type operator*
        (
            typename mat<1, 1, T>::col_type const& v,
            mat<1, 1, T> const& m
            )
    {
        return vec<2, T>(
            v.x * m[0][0] + v.y * m[0][1]);
    }

    template<typename T>
    mat<1, 1, T> operator*(mat<1, 1, T> const& m1, mat<1, 1, T> const& m2)
    {
        return mat<1, 1, T>(
            m1[0][0] * m2[0][0]);
    }

    template<typename T>
    mat<3, 2, T> operator*(mat<1, 1, T> const& m1, mat<3, 2, T> const& m2)
    {
        return mat<3, 2, T>(
            m1[0][0] * m2[0][0]);
    }

    template<typename T>
    mat<4, 2, T> operator*(mat<1, 1, T> const& m1, mat<4, 2, T> const& m2)
    {
        return mat<4, 2, T>(
            m1[0][0] * m2[0][0]);
    }

    template<typename T>
    mat<1, 1, T> operator/(mat<1, 1, T> const& m, T scalar)
    {
        return mat<1, 1, T>(
            m[0] / scalar);
    }

    template<typename T>
    mat<1, 1, T> operator/(T scalar, mat<1, 1, T> const& m)
    {
        return mat<1, 1, T>(
            scalar / m[0]);
    }

    template<typename T>
    typename mat<1, 1, T>::col_type operator/(mat<1, 1, T> const& m, typename mat<1, 1, T>::row_type const& v)
    {
        return inverse(m) * v;
    }

    template<typename T>
    typename mat<1, 1, T>::row_type operator/(typename mat<1, 1, T>::col_type const& v, mat<1, 1, T> const& m)
    {
        return v * inverse(m);
    }

    template<typename T>
    mat<1, 1, T> operator/(mat<1, 1, T> const& m1, mat<1, 1, T> const& m2)
    {
        mat<1, 1, T> m1_copy(m1);
        return m1_copy /= m2;
    }

    // -- Boolean operators --

    template<typename T>
    bool operator==(mat<1, 1, T> const& m1, mat<1, 1, T> const& m2)
    {
        return (m1[0] == m2[0]);
    }

    template<typename T>
    bool operator!=(mat<1, 1, T> const& m1, mat<1, 1, T> const& m2)
    {
        return (m1[0] != m2[0]);
    }

} //namespace oom
