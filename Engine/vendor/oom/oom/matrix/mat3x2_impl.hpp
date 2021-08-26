#pragma once

namespace oom
{
    // -- Constructors --

    template<typename T>
    constexpr mat<3, 2, T>::mat(mat<3, 2, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2])}
    {
    }

    template<typename T>
    constexpr mat<3, 2, T>::mat(T s)
            : value{col_type(s, 0), col_type(0, s), col_type(0, 0)}
    {
    }

    template<typename T>
    constexpr mat<3, 2, T>::mat
    (
        T x0, T y0,
        T x1, T y1,
        T x2, T y2
    )
            : value{col_type(x0, y0), col_type(x1, y1), col_type(x2, y2)}
    {
    }

    template<typename T>
    constexpr mat<3, 2, T>::mat(col_type const& v0, col_type const& v1, col_type const& v2)
            : value{col_type(v0), col_type(v1), col_type(v2)}
    {
    }

    // -- Conversion constructors --

    template<typename T>
    template<
        typename X0, typename Y0,
        typename X1, typename Y1,
        typename X2, typename Y2>
    constexpr mat<3, 2, T>::mat
    (
        X0 x0, Y0 y0,
        X1 x1, Y1 y1,
        X2 x2, Y2 y2
    )
            : value{col_type(x0, y0), col_type(x1, y1), col_type(x2, y2)}
    {
    }

    template<typename T>
    template<typename V0, typename V1, typename V2>
    constexpr mat<3, 2, T>::mat(vec<2, V0> const& v0, vec<2, V1> const& v1, vec<2, V2> const& v2)
            : value{col_type(v0), col_type(v1), col_type(v2)}
    {
    }

    // -- Matrix conversions --

    template<typename T>
    template<typename U>
    constexpr mat<3, 2, T>::mat(mat<3, 2, U> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2])}
    {
    }

    template<typename T>
    constexpr mat<3, 2, T>::mat(mat<2, 2, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(0)}
    {
    }

    template<typename T>
    constexpr mat<3, 2, T>::mat(mat<3, 3, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2])}
    {
    }

    template<typename T>
    constexpr mat<3, 2, T>::mat(mat<4, 4, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2])}
    {
    }

    template<typename T>
    constexpr mat<3, 2, T>::mat(mat<2, 3, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(0)}
    {
    }

    template<typename T>
    constexpr mat<3, 2, T>::mat(mat<2, 4, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(0)}
    {
    }

    template<typename T>
    constexpr mat<3, 2, T>::mat(mat<3, 4, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2])}
    {
    }

    template<typename T>
    constexpr mat<3, 2, T>::mat(mat<4, 2, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2])}
    {
    }

    template<typename T>
    constexpr mat<3, 2, T>::mat(mat<4, 3, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2])}
    {
    }

    // -- Accesses --

    template<typename T>
    typename mat<3, 2, T>::col_type & mat<3, 2, T>::operator[](typename mat<3, 2, T>::length_type i)
    {
        assert(i < this->size());
        return this->value[i];
    }

    template<typename T>
    constexpr typename mat<3, 2, T>::col_type const& mat<3, 2, T>::operator[](typename mat<3, 2, T>::length_type i) const
    {
        assert(i < this->size());
        return this->value[i];
    }

    // -- Unary updatable operators --

    template<typename T>
    template<typename U>
    mat<3, 2, T>& mat<3, 2, T>::operator=(mat<3, 2, U> const& m)
    {
        this->value[0] = m[0];
        this->value[1] = m[1];
        this->value[2] = m[2];
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<3, 2, T>& mat<3, 2, T>::operator+=(U s)
    {
        this->value[0] += s;
        this->value[1] += s;
        this->value[2] += s;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<3, 2, T>& mat<3, 2, T>::operator+=(mat<3, 2, U> const& m)
    {
        this->value[0] += m[0];
        this->value[1] += m[1];
        this->value[2] += m[2];
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<3, 2, T>& mat<3, 2, T>::operator-=(U s)
    {
        this->value[0] -= s;
        this->value[1] -= s;
        this->value[2] -= s;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<3, 2, T>& mat<3, 2, T>::operator-=(mat<3, 2, U> const& m)
    {
        this->value[0] -= m[0];
        this->value[1] -= m[1];
        this->value[2] -= m[2];
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<3, 2, T>& mat<3, 2, T>::operator*=(U s)
    {
        this->value[0] *= s;
        this->value[1] *= s;
        this->value[2] *= s;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<3, 2, T> & mat<3, 2, T>::operator/=(U s)
    {
        this->value[0] /= s;
        this->value[1] /= s;
        this->value[2] /= s;
        return *this;
    }

    // -- Increment and decrement operators --

    template<typename T>
    mat<3, 2, T>& mat<3, 2, T>::operator++()
    {
        ++this->value[0];
        ++this->value[1];
        ++this->value[2];
        return *this;
    }

    template<typename T>
    mat<3, 2, T>& mat<3, 2, T>::operator--()
    {
        --this->value[0];
        --this->value[1];
        --this->value[2];
        return *this;
    }

    template<typename T>
    mat<3, 2, T> mat<3, 2, T>::operator++(int)
    {
        mat<3, 2, T> Result(*this);
        ++*this;
        return Result;
    }

    template<typename T>
    mat<3, 2, T> mat<3, 2, T>::operator--(int)
    {
        mat<3, 2, T> Result(*this);
        --*this;
        return Result;
    }

    // -- Unary arithmetic operators --

    template<typename T>
    mat<3, 2, T> operator+(mat<3, 2, T> const& m)
    {
        return m;
    }

    template<typename T>
    mat<3, 2, T> operator-(mat<3, 2, T> const& m)
    {
        return mat<3, 2, T>(
            -m[0],
            -m[1],
            -m[2]);
    }

    // -- Binary arithmetic operators --

    template<typename T>
    mat<3, 2, T> operator+(mat<3, 2, T> const& m, T scalar)
    {
        return mat<3, 2, T>(
            m[0] + scalar,
            m[1] + scalar,
            m[2] + scalar);
    }

    template<typename T>
    mat<3, 2, T> operator+(mat<3, 2, T> const& m1, mat<3, 2, T> const& m2)
    {
        return mat<3, 2, T>(
            m1[0] + m2[0],
            m1[1] + m2[1],
            m1[2] + m2[2]);
    }

    template<typename T>
    mat<3, 2, T> operator-(mat<3, 2, T> const& m, T scalar)
    {
        return mat<3, 2, T>(
            m[0] - scalar,
            m[1] - scalar,
            m[2] - scalar);
    }

    template<typename T>
    mat<3, 2, T> operator-(mat<3, 2, T> const& m1, mat<3, 2, T> const& m2)
    {
        return mat<3, 2, T>(
            m1[0] - m2[0],
            m1[1] - m2[1],
            m1[2] - m2[2]);
    }

    template<typename T>
    mat<3, 2, T> operator*(mat<3, 2, T> const& m, T scalar)
    {
        return mat<3, 2, T>(
            m[0] * scalar,
            m[1] * scalar,
            m[2] * scalar);
    }

    template<typename T>
    mat<3, 2, T> operator*(T scalar, mat<3, 2, T> const& m)
    {
        return mat<3, 2, T>(
            m[0] * scalar,
            m[1] * scalar,
            m[2] * scalar);
    }

    template<typename T>
    typename mat<3, 2, T>::col_type operator*(mat<3, 2, T> const& m, typename mat<3, 2, T>::row_type const& v)
    {
        return typename mat<3, 2, T>::col_type(
            m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
            m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z);
    }

    template<typename T>
    typename mat<3, 2, T>::row_type operator*(typename mat<3, 2, T>::col_type const& v, mat<3, 2, T> const& m)
    {
        return typename mat<3, 2, T>::row_type(
            v.x * m[0][0] + v.y * m[0][1],
            v.x * m[1][0] + v.y * m[1][1],
            v.x * m[2][0] + v.y * m[2][1]);
    }

    template<typename T>
    mat<2, 2, T> operator*(mat<3, 2, T> const& m1, mat<2, 3, T> const& m2)
    {
        const T SrcA00 = m1[0][0];
        const T SrcA01 = m1[0][1];
        const T SrcA10 = m1[1][0];
        const T SrcA11 = m1[1][1];
        const T SrcA20 = m1[2][0];
        const T SrcA21 = m1[2][1];

        const T SrcB00 = m2[0][0];
        const T SrcB01 = m2[0][1];
        const T SrcB02 = m2[0][2];
        const T SrcB10 = m2[1][0];
        const T SrcB11 = m2[1][1];
        const T SrcB12 = m2[1][2];

        mat<2, 2, T> Result;
        Result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01 + SrcA20 * SrcB02;
        Result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01 + SrcA21 * SrcB02;
        Result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11 + SrcA20 * SrcB12;
        Result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11 + SrcA21 * SrcB12;
        return Result;
    }

    template<typename T>
    mat<3, 2, T> operator*(mat<3, 2, T> const& m1, mat<3, 3, T> const& m2)
    {
        return mat<3, 2, T>(
            m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
            m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
            m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
            m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
            m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2],
            m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2]);
    }

    template<typename T>
    mat<4, 2, T> operator*(mat<3, 2, T> const& m1, mat<4, 3, T> const& m2)
    {
        return mat<4, 2, T>(
            m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
            m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
            m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
            m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
            m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2],
            m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2],
            m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1] + m1[2][0] * m2[3][2],
            m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1] + m1[2][1] * m2[3][2]);
    }

    template<typename T>
    mat<3, 2, T> operator/(mat<3, 2, T> const& m, T scalar)
    {
        return mat<3, 2, T>(
            m[0] / scalar,
            m[1] / scalar,
            m[2] / scalar);
    }

    template<typename T>
    mat<3, 2, T> operator/(T scalar, mat<3, 2, T> const& m)
    {
        return mat<3, 2, T>(
            scalar / m[0],
            scalar / m[1],
            scalar / m[2]);
    }

    // -- Boolean operators --

    template<typename T>
    bool operator==(mat<3, 2, T> const& m1, mat<3, 2, T> const& m2)
    {
        return (m1[0] == m2[0]) && (m1[1] == m2[1]) && (m1[2] == m2[2]);
    }

    template<typename T>
    bool operator!=(mat<3, 2, T> const& m1, mat<3, 2, T> const& m2)
    {
        return (m1[0] != m2[0]) || (m1[1] != m2[1]) || (m1[2] != m2[2]);
    }
} //namespace oom
