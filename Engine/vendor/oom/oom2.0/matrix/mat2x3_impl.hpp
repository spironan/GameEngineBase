#pragma once

namespace oom
{
    // -- Constructors --
    
    template<typename T>
    constexpr mat<2, 3, T>::mat(mat<2, 3, T> const& m)
            : value{m.value[0], m.value[1]}
    {
    }

    template<typename T>
    constexpr mat<2, 3, T>::mat(T scalar)
            : value{col_type(scalar, 0, 0), col_type(0, scalar, 0)}
    {
    }

    template<typename T>
    constexpr mat<2, 3, T>::mat
    (
        T x0, T y0, T z0,
        T x1, T y1, T z1
    )
            : value{col_type(x0, y0, z0), col_type(x1, y1, z1)}
    {
    }

    template<typename T>
    constexpr mat<2, 3, T>::mat(col_type const& v0, col_type const& v1)
            : value{col_type(v0), col_type(v1)}
    {
    }

    // -- Conversion constructors --

    template<typename T>
    template<
        typename X1, typename Y1, typename Z1,
        typename X2, typename Y2, typename Z2>
    constexpr mat<2, 3, T>::mat
    (
        X1 x1, Y1 y1, Z1 z1,
        X2 x2, Y2 y2, Z2 z2
    )
            : value{col_type(x1, y1, z1), col_type(x2, y2, z2)}
    {
    }

    template<typename T>
    template<typename V1, typename V2>
    constexpr mat<2, 3, T>::mat(vec<3, V1> const& v1, vec<3, V2> const& v2)
            : value{col_type(v1), col_type(v2)}
    {
    }

    // -- Matrix conversions --

    template<typename T>
    template<typename U>
    constexpr mat<2, 3, T>::mat(mat<2, 3, U> const& m)
            : value{col_type(m[0]), col_type(m[1])}
    {
    }

    template<typename T>
    constexpr mat<2, 3, T>::mat(mat<2, 2, T> const& m)
            : value{col_type(m[0], 0), col_type(m[1], 0)}
    {
    }

    template<typename T>
    constexpr  mat<2, 3, T>::mat(mat<3, 3, T> const& m)
            : value{col_type(m[0]), col_type(m[1])}
    {
    }

    template<typename T>
    constexpr mat<2, 3, T>::mat(mat<4, 4, T> const& m)
        : value{col_type(m[0]), col_type(m[1])}
    {
    }

    template<typename T>
    constexpr mat<2, 3, T>::mat(mat<2, 4, T> const& m)
            : value{col_type(m[0]), col_type(m[1])}
    {
    }

    template<typename T>
    constexpr mat<2, 3, T>::mat(mat<3, 2, T> const& m)
            : value{col_type(m[0], 0), col_type(m[1], 0)}
    {
    }

    template<typename T>
    constexpr mat<2, 3, T>::mat(mat<3, 4, T> const& m)
            : value{col_type(m[0]), col_type(m[1])}
    {
    }

    template<typename T>
    constexpr mat<2, 3, T>::mat(mat<4, 2, T> const& m)
            : value{col_type(m[0], 0), col_type(m[1], 0)}
    {
    }

    template<typename T>
    constexpr mat<2, 3, T>::mat(mat<4, 3, T> const& m)
            : value{col_type(m[0]), col_type(m[1])}
    {
    }

    // -- Accesses --

    template<typename T>
    typename mat<2, 3, T>::col_type & mat<2, 3, T>::operator[](typename mat<2, 3, T>::length_type i)
    {
        assert(i < this->size());
        return this->value[i];
    }

    template<typename T>
    constexpr typename mat<2, 3, T>::col_type const& mat<2, 3, T>::operator[](typename mat<2, 3, T>::length_type i) const
    {
        assert(i < this->size());
        return this->value[i];
    }

    // -- Unary updatable operators --

    template<typename T>
    template<typename U>
    mat<2, 3, T>& mat<2, 3, T>::operator=(mat<2, 3, U> const& m)
    {
        this->value[0] = m[0];
        this->value[1] = m[1];
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<2, 3, T> & mat<2, 3, T>::operator+=(U s)
    {
        this->value[0] += s;
        this->value[1] += s;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<2, 3, T>& mat<2, 3, T>::operator+=(mat<2, 3, U> const& m)
    {
        this->value[0] += m[0];
        this->value[1] += m[1];
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<2, 3, T>& mat<2, 3, T>::operator-=(U s)
    {
        this->value[0] -= s;
        this->value[1] -= s;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<2, 3, T>& mat<2, 3, T>::operator-=(mat<2, 3, U> const& m)
    {
        this->value[0] -= m[0];
        this->value[1] -= m[1];
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<2, 3, T>& mat<2, 3, T>::operator*=(U s)
    {
        this->value[0] *= s;
        this->value[1] *= s;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<2, 3, T> & mat<2, 3, T>::operator/=(U s)
    {
        this->value[0] /= s;
        this->value[1] /= s;
        return *this;
    }

    // -- Increment and decrement operators --

    template<typename T>
    mat<2, 3, T> & mat<2, 3, T>::operator++()
    {
        ++this->value[0];
        ++this->value[1];
        return *this;
    }

    template<typename T>
    mat<2, 3, T> & mat<2, 3, T>::operator--()
    {
        --this->value[0];
        --this->value[1];
        return *this;
    }

    template<typename T>
    mat<2, 3, T> mat<2, 3, T>::operator++(int)
    {
        mat<2, 3, T> Result(*this);
        ++*this;
        return Result;
    }

    template<typename T>
    mat<2, 3, T> mat<2, 3, T>::operator--(int)
    {
        mat<2, 3, T> Result(*this);
        --*this;
        return Result;
    }

    // -- Unary arithmetic operators --

    template<typename T>
    mat<2, 3, T> operator+(mat<2, 3, T> const& m)
    {
        return m;
    }

    template<typename T>
    mat<2, 3, T> operator-(mat<2, 3, T> const& m)
    {
        return mat<2, 3, T>(
            -m[0],
            -m[1]);
    }

    // -- Binary arithmetic operators --

    template<typename T>
    mat<2, 3, T> operator+(mat<2, 3, T> const& m, T scalar)
    {
        return mat<2, 3, T>(
            m[0] + scalar,
            m[1] + scalar);
    }

    template<typename T>
    mat<2, 3, T> operator+(mat<2, 3, T> const& m1, mat<2, 3, T> const& m2)
    {
        return mat<2, 3, T>(
            m1[0] + m2[0],
            m1[1] + m2[1]);
    }

    template<typename T>
    mat<2, 3, T> operator-(mat<2, 3, T> const& m, T scalar)
    {
        return mat<2, 3, T>(
            m[0] - scalar,
            m[1] - scalar);
    }

    template<typename T>
    mat<2, 3, T> operator-(mat<2, 3, T> const& m1, mat<2, 3, T> const& m2)
    {
        return mat<2, 3, T>(
            m1[0] - m2[0],
            m1[1] - m2[1]);
    }

    template<typename T>
    mat<2, 3, T> operator*(mat<2, 3, T> const& m, T scalar)
    {
        return mat<2, 3, T>(
            m[0] * scalar,
            m[1] * scalar);
    }

    template<typename T>
    mat<2, 3, T> operator*(T scalar, mat<2, 3, T> const& m)
    {
        return mat<2, 3, T>(
            m[0] * scalar,
            m[1] * scalar);
    }

    template<typename T>
    typename mat<2, 3, T>::col_type operator*
    (
        mat<2, 3, T> const& m,
        typename mat<2, 3, T>::row_type const& v)
    {
        return typename mat<2, 3, T>::col_type(
            m[0][0] * v.x + m[1][0] * v.y,
            m[0][1] * v.x + m[1][1] * v.y,
            m[0][2] * v.x + m[1][2] * v.y);
    }

    template<typename T>
    typename mat<2, 3, T>::row_type operator*
    (
        typename mat<2, 3, T>::col_type const& v,
        mat<2, 3, T> const& m)
    {
        return typename mat<2, 3, T>::row_type(
            v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2],
            v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2]);
    }

    template<typename T>
    mat<2, 3, T> operator*(mat<2, 3, T> const& m1, mat<2, 2, T> const& m2)
    {
        return mat<2, 3, T>(
            m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1],
            m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1],
            m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1],
            m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1],
            m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1],
            m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1]);
    }

    template<typename T>
    mat<3, 3, T> operator*(mat<2, 3, T> const& m1, mat<3, 2, T> const& m2)
    {
        T SrcA00 = m1[0][0];
        T SrcA01 = m1[0][1];
        T SrcA02 = m1[0][2];
        T SrcA10 = m1[1][0];
        T SrcA11 = m1[1][1];
        T SrcA12 = m1[1][2];

        T SrcB00 = m2[0][0];
        T SrcB01 = m2[0][1];
        T SrcB10 = m2[1][0];
        T SrcB11 = m2[1][1];
        T SrcB20 = m2[2][0];
        T SrcB21 = m2[2][1];

        mat<3, 3, T> Result;
        Result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01;
        Result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01;
        Result[0][2] = SrcA02 * SrcB00 + SrcA12 * SrcB01;
        Result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11;
        Result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11;
        Result[1][2] = SrcA02 * SrcB10 + SrcA12 * SrcB11;
        Result[2][0] = SrcA00 * SrcB20 + SrcA10 * SrcB21;
        Result[2][1] = SrcA01 * SrcB20 + SrcA11 * SrcB21;
        Result[2][2] = SrcA02 * SrcB20 + SrcA12 * SrcB21;
        return Result;
    }

    template<typename T>
    mat<4, 3, T> operator*(mat<2, 3, T> const& m1, mat<4, 2, T> const& m2)
    {
        return mat<4, 3, T>(
            m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1],
            m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1],
            m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1],
            m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1],
            m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1],
            m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1],
            m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1],
            m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1],
            m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1],
            m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1],
            m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1],
            m1[0][2] * m2[3][0] + m1[1][2] * m2[3][1]);
    }

    template<typename T>
    mat<2, 3, T> operator/(mat<2, 3, T> const& m, T scalar)
    {
        return mat<2, 3, T>(
            m[0] / scalar,
            m[1] / scalar);
    }

    template<typename T>
    mat<2, 3, T> operator/(T scalar, mat<2, 3, T> const& m)
    {
        return mat<2, 3, T>(
            scalar / m[0],
            scalar / m[1]);
    }

    // -- Boolean operators --

    template<typename T>
    bool operator==(mat<2, 3, T> const& m1, mat<2, 3, T> const& m2)
    {
        return (m1[0] == m2[0]) && (m1[1] == m2[1]);
    }

    template<typename T>
    bool operator!=(mat<2, 3, T> const& m1, mat<2, 3, T> const& m2)
    {
        return (m1[0] != m2[0]) || (m1[1] != m2[1]);
    }
} //namespace oom
