#pragma once

namespace oom
{
    // -- Constructors --

    template<typename T>
    constexpr mat<4, 3, T>::mat(mat<4, 3, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2]), col_type(m[3])}
    {
    }

    template<typename T>
    constexpr mat<4, 3, T>::mat(T const& s)
            : value{col_type(s, 0, 0), col_type(0, s, 0), col_type(0, 0, s), col_type(0, 0, 0)}
    {
    }

    template<typename T>
    constexpr mat<4, 3, T>::mat
    (
        T const& x0, T const& y0, T const& z0,
        T const& x1, T const& y1, T const& z1,
        T const& x2, T const& y2, T const& z2,
        T const& x3, T const& y3, T const& z3
    )
            : value{col_type(x0, y0, z0), col_type(x1, y1, z1), col_type(x2, y2, z2), col_type(x3, y3, z3)}
    {
    }

    template<typename T>
    constexpr mat<4, 3, T>::mat(col_type const& v0, col_type const& v1, col_type const& v2, col_type const& v3)
            : value{col_type(v0), col_type(v1), col_type(v2), col_type(v3)}
    {
    }

    // -- Conversion constructors --

    template<typename T>
    template<
        typename X0, typename Y0, typename Z0,
        typename X1, typename Y1, typename Z1,
        typename X2, typename Y2, typename Z2,
        typename X3, typename Y3, typename Z3>
    constexpr mat<4, 3, T>::mat
    (
        X0 const& x0, Y0 const& y0, Z0 const& z0,
        X1 const& x1, Y1 const& y1, Z1 const& z1,
        X2 const& x2, Y2 const& y2, Z2 const& z2,
        X3 const& x3, Y3 const& y3, Z3 const& z3
    )
            : value{col_type(x0, y0, z0), col_type(x1, y1, z1), col_type(x2, y2, z2), col_type(x3, y3, z3)}
    {
    }

    template<typename T>
    template<typename V1, typename V2, typename V3, typename V4>
    constexpr mat<4, 3, T>::mat(vec<3, V1> const& v1, vec<3, V2> const& v2, vec<3, V3> const& v3, vec<3, V4> const& v4)
            : value{col_type(v1), col_type(v2), col_type(v3), col_type(v4)}
    {
    }

    // -- Matrix conversions --

    template<typename T>
    template<typename U>
    constexpr mat<4, 3, T>::mat(mat<4, 3, U> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2]), col_type(m[3])}
    {
    }

    template<typename T>
    constexpr mat<4, 3, T>::mat(mat<2, 2, T> const& m)
            : value{col_type(m[0], 0), col_type(m[1], 0), col_type(0, 0, 1), col_type(0)}
    {
    }

    template<typename T>
    constexpr mat<4, 3, T>::mat(mat<3, 3, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2]), col_type(0)}
    {
    }

    template<typename T>
    constexpr mat<4, 3, T>::mat(mat<4, 4, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2]), col_type(m[3])}
    {
    }

    template<typename T>
    constexpr mat<4, 3, T>::mat(mat<2, 3, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(0, 0, 1), col_type(0)}
    {
    }

    template<typename T>
    constexpr mat<4, 3, T>::mat(mat<3, 2, T> const& m)
            : value{col_type(m[0], 0), col_type(m[1], 0), col_type(m[2], 1), col_type(0)}
    {
    }

    template<typename T>
    constexpr mat<4, 3, T>::mat(mat<2, 4, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(0, 0, 1), col_type(0)}
    {
    }

    template<typename T>
    constexpr mat<4, 3, T>::mat(mat<4, 2, T> const& m)
            : value{col_type(m[0], 0), col_type(m[1], 0), col_type(m[2], 1), col_type(m[3], 0)}
    {
    }

    template<typename T>
    constexpr mat<4, 3, T>::mat(mat<3, 4, T> const& m)
            : value{col_type(m[0]), col_type(m[1]), col_type(m[2]), col_type(0)}
    {
    }

    // -- Accesses --

    template<typename T>
    typename mat<4, 3, T>::col_type & mat<4, 3, T>::operator[](typename mat<4, 3, T>::length_type i)
    {
        assert(i < this->size());
        return this->value[i];
    }

    template<typename T>
    constexpr typename mat<4, 3, T>::col_type const& mat<4, 3, T>::operator[](typename mat<4, 3, T>::length_type i) const
    {
        assert(i < this->size());
        return this->value[i];
    }

    // -- Unary updatable operators --

    template<typename T>
    template<typename U>
    mat<4, 3, T>& mat<4, 3, T>::operator=(mat<4, 3, U> const& m)
    {
        this->value[0] = m[0];
        this->value[1] = m[1];
        this->value[2] = m[2];
        this->value[3] = m[3];
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<4, 3, T> & mat<4, 3, T>::operator+=(U s)
    {
        this->value[0] += s;
        this->value[1] += s;
        this->value[2] += s;
        this->value[3] += s;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<4, 3, T> & mat<4, 3, T>::operator+=(mat<4, 3, U> const& m)
    {
        this->value[0] += m[0];
        this->value[1] += m[1];
        this->value[2] += m[2];
        this->value[3] += m[3];
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<4, 3, T> & mat<4, 3, T>::operator-=(U s)
    {
        this->value[0] -= s;
        this->value[1] -= s;
        this->value[2] -= s;
        this->value[3] -= s;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<4, 3, T> & mat<4, 3, T>::operator-=(mat<4, 3, U> const& m)
    {
        this->value[0] -= m[0];
        this->value[1] -= m[1];
        this->value[2] -= m[2];
        this->value[3] -= m[3];
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<4, 3, T> & mat<4, 3, T>::operator*=(U s)
    {
        this->value[0] *= s;
        this->value[1] *= s;
        this->value[2] *= s;
        this->value[3] *= s;
        return *this;
    }

    template<typename T>
    template<typename U>
    mat<4, 3, T> & mat<4, 3, T>::operator/=(U s)
    {
        this->value[0] /= s;
        this->value[1] /= s;
        this->value[2] /= s;
        this->value[3] /= s;
        return *this;
    }

    // -- Increment and decrement operators --

    template<typename T>
    mat<4, 3, T> & mat<4, 3, T>::operator++()
    {
        ++this->value[0];
        ++this->value[1];
        ++this->value[2];
        ++this->value[3];
        return *this;
    }

    template<typename T>
    mat<4, 3, T> & mat<4, 3, T>::operator--()
    {
        --this->value[0];
        --this->value[1];
        --this->value[2];
        --this->value[3];
        return *this;
    }

    template<typename T>
    mat<4, 3, T> mat<4, 3, T>::operator++(int)
    {
        mat<4, 3, T> Result(*this);
        ++*this;
        return Result;
    }

    template<typename T>
    mat<4, 3, T> mat<4, 3, T>::operator--(int)
    {
        mat<4, 3, T> Result(*this);
        --*this;
        return Result;
    }

    // -- Unary arithmetic operators --

    template<typename T>
    mat<4, 3, T> operator+(mat<4, 3, T> const& m)
    {
        return m;
    }

    template<typename T>
    mat<4, 3, T> operator-(mat<4, 3, T> const& m)
    {
        return mat<4, 3, T>(
            -m[0],
            -m[1],
            -m[2],
            -m[3]);
    }

    // -- Binary arithmetic operators --

    template<typename T>
    mat<4, 3, T> operator+(mat<4, 3, T> const& m, T const& s)
    {
        return mat<4, 3, T>(
            m[0] + s,
            m[1] + s,
            m[2] + s,
            m[3] + s);
    }

    template<typename T>
    mat<4, 3, T> operator+(mat<4, 3, T> const& m1, mat<4, 3, T> const& m2)
    {
        return mat<4, 3, T>(
            m1[0] + m2[0],
            m1[1] + m2[1],
            m1[2] + m2[2],
            m1[3] + m2[3]);
    }

    template<typename T>
    mat<4, 3, T> operator-(mat<4, 3, T> const& m, T const& s)
    {
        return mat<4, 3, T>(
            m[0] - s,
            m[1] - s,
            m[2] - s,
            m[3] - s);
    }

    template<typename T>
    mat<4, 3, T> operator-(mat<4, 3, T> const& m1, mat<4, 3, T> const& m2)
    {
        return mat<4, 3, T>(
            m1[0] - m2[0],
            m1[1] - m2[1],
            m1[2] - m2[2],
            m1[3] - m2[3]);
    }

    template<typename T>
    mat<4, 3, T> operator*(mat<4, 3, T> const& m, T const& s)
    {
        return mat<4, 3, T>(
            m[0] * s,
            m[1] * s,
            m[2] * s,
            m[3] * s);
    }

    template<typename T>
    mat<4, 3, T> operator*(T const& s, mat<4, 3, T> const& m)
    {
        return mat<4, 3, T>(
            m[0] * s,
            m[1] * s,
            m[2] * s,
            m[3] * s);
    }

    template<typename T>
    typename mat<4, 3, T>::col_type operator*
    (
        mat<4, 3, T> const& m,
        typename mat<4, 3, T>::row_type const& v)
    {
        return typename mat<4, 3, T>::col_type(
            m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w,
            m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w,
            m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w);
    }

    template<typename T>
    typename mat<4, 3, T>::row_type operator*
    (
        typename mat<4, 3, T>::col_type const& v,
        mat<4, 3, T> const& m)
    {
        return typename mat<4, 3, T>::row_type(
            v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2],
            v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2],
            v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2],
            v.x * m[3][0] + v.y * m[3][1] + v.z * m[3][2]);
    }

    template<typename T>
    mat<2, 3, T> operator*(mat<4, 3, T> const& m1, mat<2, 4, T> const& m2)
    {
        return mat<2, 3, T>(
            m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3],
            m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3],
            m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2] + m1[3][2] * m2[0][3],
            m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3],
            m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3],
            m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2] + m1[3][2] * m2[1][3]);
    }

    template<typename T>
    mat<3, 3, T> operator*(mat<4, 3, T> const& m1, mat<3, 4, T> const& m2)
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
        T const SrcA30 = m1[3][0];
        T const SrcA31 = m1[3][1];
        T const SrcA32 = m1[3][2];

        T const SrcB00 = m2[0][0];
        T const SrcB01 = m2[0][1];
        T const SrcB02 = m2[0][2];
        T const SrcB03 = m2[0][3];
        T const SrcB10 = m2[1][0];
        T const SrcB11 = m2[1][1];
        T const SrcB12 = m2[1][2];
        T const SrcB13 = m2[1][3];
        T const SrcB20 = m2[2][0];
        T const SrcB21 = m2[2][1];
        T const SrcB22 = m2[2][2];
        T const SrcB23 = m2[2][3];

        mat<3, 3, T> Result;
        Result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01 + SrcA20 * SrcB02 + SrcA30 * SrcB03;
        Result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01 + SrcA21 * SrcB02 + SrcA31 * SrcB03;
        Result[0][2] = SrcA02 * SrcB00 + SrcA12 * SrcB01 + SrcA22 * SrcB02 + SrcA32 * SrcB03;
        Result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11 + SrcA20 * SrcB12 + SrcA30 * SrcB13;
        Result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11 + SrcA21 * SrcB12 + SrcA31 * SrcB13;
        Result[1][2] = SrcA02 * SrcB10 + SrcA12 * SrcB11 + SrcA22 * SrcB12 + SrcA32 * SrcB13;
        Result[2][0] = SrcA00 * SrcB20 + SrcA10 * SrcB21 + SrcA20 * SrcB22 + SrcA30 * SrcB23;
        Result[2][1] = SrcA01 * SrcB20 + SrcA11 * SrcB21 + SrcA21 * SrcB22 + SrcA31 * SrcB23;
        Result[2][2] = SrcA02 * SrcB20 + SrcA12 * SrcB21 + SrcA22 * SrcB22 + SrcA32 * SrcB23;
        return Result;
    }

    template<typename T>
    mat<4, 3, T> operator*(mat<4, 3, T> const& m1, mat<4, 4, T> const& m2)
    {
        return mat<4, 3, T>(
            m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3],
            m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3],
            m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2] + m1[3][2] * m2[0][3],
            m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3],
            m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3],
            m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2] + m1[3][2] * m2[1][3],
            m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2] + m1[3][0] * m2[2][3],
            m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2] + m1[3][1] * m2[2][3],
            m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2] + m1[3][2] * m2[2][3],
            m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1] + m1[2][0] * m2[3][2] + m1[3][0] * m2[3][3],
            m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1] + m1[2][1] * m2[3][2] + m1[3][1] * m2[3][3],
            m1[0][2] * m2[3][0] + m1[1][2] * m2[3][1] + m1[2][2] * m2[3][2] + m1[3][2] * m2[3][3]);
    }

    template<typename T>
    mat<4, 3, T> operator/(mat<4, 3, T> const& m, T const& s)
    {
        return mat<4, 3, T>(
            m[0] / s,
            m[1] / s,
            m[2] / s,
            m[3] / s);
    }

    template<typename T>
    mat<4, 3, T> operator/(T const& s, mat<4, 3, T> const& m)
    {
        return mat<4, 3, T>(
            s / m[0],
            s / m[1],
            s / m[2],
            s / m[3]);
    }

    // -- Boolean operators --

    template<typename T>
    bool operator==(mat<4, 3, T> const& m1, mat<4, 3, T> const& m2)
    {
        return (m1[0] == m2[0]) && (m1[1] == m2[1]) && (m1[2] == m2[2]) && (m1[3] == m2[3]);
    }

    template<typename T>
    bool operator!=(mat<4, 3, T> const& m1, mat<4, 3, T> const& m2)
    {
        return (m1[0] != m2[0]) || (m1[1] != m2[1]) || (m1[2] != m2[2]) || (m1[3] != m2[3]);
    }
} //namespace oom
