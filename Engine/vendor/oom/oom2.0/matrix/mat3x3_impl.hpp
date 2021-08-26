#pragma once

namespace oom
{
    // -- Constructors --

    template<typename T>
    constexpr mat<3, 3, T>::mat(mat<3, 3, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2])}
    {
    }

    template<typename T>
    constexpr mat<3, 3, T>::mat(T s)
            : value{col_type(s, 0, 0), col_type(0, s, 0), col_type(0, 0, s)}
    {
    }

    template<typename T>
    constexpr mat<3, 3, T>::mat
    (
        T x0, T y0, T z0,
        T x1, T y1, T z1,
        T x2, T y2, T z2
    )
            : value{col_type(x0, y0, z0), col_type(x1, y1, z1), col_type(x2, y2, z2)}
    {
    }

    template<typename T>
    constexpr mat<3, 3, T>::mat(col_type const& v0, col_type const& v1, col_type const& v2)
            : value{col_type(v0), col_type(v1), col_type(v2)}
    {
    }

    // -- Conversion constructors --

    template<typename T>
    template<
        typename X1, typename Y1, typename Z1,
        typename X2, typename Y2, typename Z2,
        typename X3, typename Y3, typename Z3>
    constexpr mat<3, 3, T>::mat
    (
        X1 x1, Y1 y1, Z1 z1,
        X2 x2, Y2 y2, Z2 z2,
        X3 x3, Y3 y3, Z3 z3
    )
            : value{col_type(x1, y1, z1), col_type(x2, y2, z2), col_type(x3, y3, z3)}
    {
    }

    template<typename T>
    template<typename V1, typename V2, typename V3>
    constexpr mat<3, 3, T>::mat(vec<3, V1> const& v1, vec<3, V2> const& v2, vec<3, V3> const& v3)
            : value{col_type(v1), col_type(v2), col_type(v3)}
    {
    }

    // -- Matrix conversions --

    template<typename T>
    template<typename U>
    constexpr mat<3, 3, T>::mat(mat<3, 3, U> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2])}
    {
    }

    template<typename T>
    constexpr mat<3, 3, T>::mat(mat<2, 2, T> const& m)
            : value{col_type(m[0], 0), col_type(m[1], 0), col_type(0, 0, 1)}
    {
    }

    template<typename T>
    constexpr mat<3, 3, T>::mat(mat<4, 4, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2])}
    {
    }

    template<typename T>
    constexpr mat<3, 3, T>::mat(mat<2, 3, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(0, 0, 1)}
    {
    }

    template<typename T>
    constexpr mat<3, 3, T>::mat(mat<3, 2, T> const& m)
            : value{col_type(m[0], 0), col_type(m[1], 0), col_type(m[2], 1)}
    {
    }

    template<typename T>
    constexpr mat<3, 3, T>::mat(mat<2, 4, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(0, 0, 1)}
    {
    }

    template<typename T>
    constexpr mat<3, 3, T>::mat(mat<4, 2, T> const& m)
            : value{col_type(m[0], 0), col_type(m[1], 0), col_type(m[2], 1)}
    {
    }

    template<typename T>
    constexpr mat<3, 3, T>::mat(mat<3, 4, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2])}
    {
    }

    template<typename T>
    constexpr mat<3, 3, T>::mat(mat<4, 3, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2])}
    {
    }

    // -- Accesses --

    template<typename T>
    typename mat<3, 3, T>::col_type & mat<3, 3, T>::operator[](typename mat<3, 3, T>::length_type i)
    {
        assert(i < this->size());
        return this->value[i];
    }

    template<typename T>
    constexpr typename mat<3, 3, T>::col_type const& mat<3, 3, T>::operator[](typename mat<3, 3, T>::length_type i) const
    {
        assert(i < this->size());
        return this->value[i];
    }

    // -- Unary updatable operators --

    template<typename T>
    template<typename U>
    mat<3, 3, T> & mat<3, 3, T>::operator=(mat<3, 3, U> const& m)
    {
        this->value[0] = m[0];
        this->value[1] = m[1];
        this->value[2] = m[2];
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<3, 3, T> & mat<3, 3, T>::operator+=(U s)
    {
        this->value[0] += s;
        this->value[1] += s;
        this->value[2] += s;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<3, 3, T> & mat<3, 3, T>::operator+=(mat<3, 3, U> const& m)
    {
        this->value[0] += m[0];
        this->value[1] += m[1];
        this->value[2] += m[2];
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<3, 3, T> & mat<3, 3, T>::operator-=(U s)
    {
        this->value[0] -= s;
        this->value[1] -= s;
        this->value[2] -= s;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<3, 3, T> & mat<3, 3, T>::operator-=(mat<3, 3, U> const& m)
    {
        this->value[0] -= m[0];
        this->value[1] -= m[1];
        this->value[2] -= m[2];
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<3, 3, T> & mat<3, 3, T>::operator*=(U s)
    {
        this->value[0] *= s;
        this->value[1] *= s;
        this->value[2] *= s;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<3, 3, T> & mat<3, 3, T>::operator*=(mat<3, 3, U> const& m)
    {
        return (*this = *this * m);
    }

    template<typename T>
    template<typename U>
    mat<3, 3, T> & mat<3, 3, T>::operator/=(U s)
    {
        this->value[0] /= s;
        this->value[1] /= s;
        this->value[2] /= s;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<3, 3, T> & mat<3, 3, T>::operator/=(mat<3, 3, U> const& m)
    {
        return *this *= inverse(m);
    }

    // -- Increment and decrement operators --

    template<typename T>
    mat<3, 3, T> & mat<3, 3, T>::operator++()
    {
        ++this->value[0];
        ++this->value[1];
        ++this->value[2];
        return *this;
    }

    template<typename T>
    mat<3, 3, T> & mat<3, 3, T>::operator--()
    {
        --this->value[0];
        --this->value[1];
        --this->value[2];
        return *this;
    }

    template<typename T>
    mat<3, 3, T> mat<3, 3, T>::operator++(int)
    {
        mat<3, 3, T> Result(*this);
        ++*this;
        return Result;
    }

    template<typename T>
    mat<3, 3, T> mat<3, 3, T>::operator--(int)
    {
        mat<3, 3, T> Result(*this);
        --*this;
        return Result;
    }

    // -- Unary arithmetic operators --

    template<typename T>
    mat<3, 3, T> operator+(mat<3, 3, T> const& m)
    {
        return m;
    }

    template<typename T>
    mat<3, 3, T> operator-(mat<3, 3, T> const& m)
    {
        return mat<3, 3, T>(
            -m[0],
            -m[1],
            -m[2]);
    }

    // -- Binary arithmetic operators --

    template<typename T>
    mat<3, 3, T> operator+(mat<3, 3, T> const& m, T scalar)
    {
        return mat<3, 3, T>(
            m[0] + scalar,
            m[1] + scalar,
            m[2] + scalar);
    }

    template<typename T>
    mat<3, 3, T> operator+(T scalar, mat<3, 3, T> const& m)
    {
        return mat<3, 3, T>(
            m[0] + scalar,
            m[1] + scalar,
            m[2] + scalar);
    }

    template<typename T>
    mat<3, 3, T> operator+(mat<3, 3, T> const& m1, mat<3, 3, T> const& m2)
    {
        return mat<3, 3, T>(
            m1[0] + m2[0],
            m1[1] + m2[1],
            m1[2] + m2[2]);
    }

    template<typename T>
    mat<3, 3, T> operator-(mat<3, 3, T> const& m, T scalar)
    {
        return mat<3, 3, T>(
            m[0] - scalar,
            m[1] - scalar,
            m[2] - scalar);
    }

    template<typename T>
    mat<3, 3, T> operator-(T scalar, mat<3, 3, T> const& m)
    {
        return mat<3, 3, T>(
            scalar - m[0],
            scalar - m[1],
            scalar - m[2]);
    }

    template<typename T>
    mat<3, 3, T> operator-(mat<3, 3, T> const& m1, mat<3, 3, T> const& m2)
    {
        return mat<3, 3, T>(
            m1[0] - m2[0],
            m1[1] - m2[1],
            m1[2] - m2[2]);
    }

    template<typename T>
    mat<3, 3, T> operator*(mat<3, 3, T> const& m, T scalar)
    {
        return mat<3, 3, T>(
            m[0] * scalar,
            m[1] * scalar,
            m[2] * scalar);
    }

    template<typename T>
    mat<3, 3, T> operator*(T scalar, mat<3, 3, T> const& m)
    {
        return mat<3, 3, T>(
            m[0] * scalar,
            m[1] * scalar,
            m[2] * scalar);
    }

    template<typename T>
    typename mat<3, 3, T>::col_type operator*(mat<3, 3, T> const& m, typename mat<3, 3, T>::row_type const& v)
    {
        return typename mat<3, 3, T>::col_type(
            m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
            m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
            m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z);
    }

    template<typename T>
    typename mat<3, 3, T>::row_type operator*(typename mat<3, 3, T>::col_type const& v, mat<3, 3, T> const& m)
    {
        return typename mat<3, 3, T>::row_type(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
    }

    template<typename T>
    mat<3, 3, T> operator*(mat<3, 3, T> const& m1, mat<3, 3, T> const& m2)
    {
        T const SrcA00 = m1[0][0];
        T const SrcA01 = m1[0][1];
        T const SrcA02 = m1[0][2];
        T const SrcA10 = m1[1][0];
        T const SrcA11 = m1[1][1];
        T const SrcA12 = m1[1][2];
        T const SrcA20 = m1[2][0];
        T const SrcA21 = m1[2][1];
        T const SrcA22 = m1[2][2];

        T const SrcB00 = m2[0][0];
        T const SrcB01 = m2[0][1];
        T const SrcB02 = m2[0][2];
        T const SrcB10 = m2[1][0];
        T const SrcB11 = m2[1][1];
        T const SrcB12 = m2[1][2];
        T const SrcB20 = m2[2][0];
        T const SrcB21 = m2[2][1];
        T const SrcB22 = m2[2][2];

        mat<3, 3, T> Result;
        Result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01 + SrcA20 * SrcB02;
        Result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01 + SrcA21 * SrcB02;
        Result[0][2] = SrcA02 * SrcB00 + SrcA12 * SrcB01 + SrcA22 * SrcB02;
        Result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11 + SrcA20 * SrcB12;
        Result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11 + SrcA21 * SrcB12;
        Result[1][2] = SrcA02 * SrcB10 + SrcA12 * SrcB11 + SrcA22 * SrcB12;
        Result[2][0] = SrcA00 * SrcB20 + SrcA10 * SrcB21 + SrcA20 * SrcB22;
        Result[2][1] = SrcA01 * SrcB20 + SrcA11 * SrcB21 + SrcA21 * SrcB22;
        Result[2][2] = SrcA02 * SrcB20 + SrcA12 * SrcB21 + SrcA22 * SrcB22;
        return Result;
    }

    template<typename T>
    mat<2, 3, T> operator*(mat<3, 3, T> const& m1, mat<2, 3, T> const& m2)
    {
        return mat<2, 3, T>(
            m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
            m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
            m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2],
            m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
            m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
            m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2]);
    }

    template<typename T>
    mat<4, 3, T> operator*(mat<3, 3, T> const& m1, mat<4, 3, T> const& m2)
    {
        return mat<4, 3, T>(
            m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
            m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
            m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2],
            m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
            m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
            m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2],
            m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2],
            m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2],
            m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2],
            m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1] + m1[2][0] * m2[3][2],
            m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1] + m1[2][1] * m2[3][2],
            m1[0][2] * m2[3][0] + m1[1][2] * m2[3][1] + m1[2][2] * m2[3][2]);
    }

    template<typename T>
    mat<3, 3, T> operator/(mat<3, 3, T> const& m,	T scalar)
    {
        return mat<3, 3, T>(
            m[0] / scalar,
            m[1] / scalar,
            m[2] / scalar);
    }

    template<typename T>
    mat<3, 3, T> operator/(T scalar, mat<3, 3, T> const& m)
    {
        return mat<3, 3, T>(
            scalar / m[0],
            scalar / m[1],
            scalar / m[2]);
    }

    template<typename T>
    typename mat<3, 3, T>::col_type operator/(mat<3, 3, T> const& m, typename mat<3, 3, T>::row_type const& v)
    {
        return  inverse(m) * v;
    }

    template<typename T>
    typename mat<3, 3, T>::row_type operator/(typename mat<3, 3, T>::col_type const& v, mat<3, 3, T> const& m)
    {
        return v * inverse(m);
    }

    template<typename T>
    mat<3, 3, T> operator/(mat<3, 3, T> const& m1, mat<3, 3, T> const& m2)
    {
        mat<3, 3, T> m1_copy(m1);
        return m1_copy /= m2;
    }

    // -- Boolean operators --

    template<typename T>
    constexpr bool operator==(mat<3, 3, T> const& m1, mat<3, 3, T> const& m2)
    {
        return (m1[0] == m2[0]) && (m1[1] == m2[1]) && (m1[2] == m2[2]);
    }

    template<typename T>
    bool operator!=(mat<3, 3, T> const& m1, mat<3, 3, T> const& m2)
    {
        return (m1[0] != m2[0]) || (m1[1] != m2[1]) || (m1[2] != m2[2]);
    }
} //namespace oom
