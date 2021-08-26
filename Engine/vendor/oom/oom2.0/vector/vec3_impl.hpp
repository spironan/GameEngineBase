#pragma once

#include "../common.hpp"
#include "../geometric.hpp"

namespace oom
{
    // -- Explicit basic constructors --

    template<typename T>
    constexpr vec<3, T>::vec(T scalar)
        : x(scalar), y(scalar), z(scalar)
    {}

    template <typename T>
    constexpr vec<3, T>::vec(T _x, T _y, T _z)
        : x(_x), y(_y), z(_z)
    {}

    // -- Conversion scalar constructors --

    template<typename T>
    template<typename U>
    constexpr vec<3, T>::vec(vec<1, U> const& v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.x))
        , z(static_cast<T>(v.x))
    {}

    template<typename T>
    template<typename X, typename Y, typename Z>
    constexpr vec<3, T>::vec(X _x, Y _y, Z _z)
        : x(static_cast<T>(_x))
        , y(static_cast<T>(_y))
        , z(static_cast<T>(_z))
    {}

    template<typename T>
    template<typename X, typename Y, typename Z>
    constexpr vec<3, T>::vec(vec<1, X> const& _x, Y _y, Z _z)
        : x(static_cast<T>(_x.x))
        , y(static_cast<T>(_y))
        , z(static_cast<T>(_z))
    {}

    template<typename T>
    template<typename X, typename Y, typename Z>
    constexpr vec<3, T>::vec(X _x, vec<1, Y> const& _y, Z _z)
        : x(static_cast<T>(_x))
        , y(static_cast<T>(_y.x))
        , z(static_cast<T>(_z))
    {}

    template<typename T>
    template<typename X, typename Y, typename Z>
    constexpr vec<3, T>::vec(vec<1, X> const& _x, vec<1, Y> const& _y, Z _z)
        : x(static_cast<T>(_x.x))
        , y(static_cast<T>(_y.x))
        , z(static_cast<T>(_z))
    {}

    template<typename T>
    template<typename X, typename Y, typename Z>
    constexpr vec<3, T>::vec(X _x, Y _y, vec<1, Z> const& _z)
        : x(static_cast<T>(_x))
        , y(static_cast<T>(_y))
        , z(static_cast<T>(_z.x))
    {}

    template<typename T>
    template<typename X, typename Y, typename Z>
    constexpr vec<3, T>::vec(vec<1, X> const& _x, Y _y, vec<1, Z> const& _z)
        : x(static_cast<T>(_x.x))
        , y(static_cast<T>(_y))
        , z(static_cast<T>(_z.x))
    {}

    template<typename T>
    template<typename X, typename Y, typename Z>
    constexpr vec<3, T>::vec(X _x, vec<1, Y> const& _y, vec<1, Z> const& _z)
        : x(static_cast<T>(_x))
        , y(static_cast<T>(_y.x))
        , z(static_cast<T>(_z.x))
    {}

    template<typename T>
    template<typename X, typename Y, typename Z>
    constexpr vec<3, T>::vec(vec<1, X> const& _x, vec<1, Y> const& _y, vec<1, Z> const& _z)
        : x(static_cast<T>(_x.x))
        , y(static_cast<T>(_y.x))
        , z(static_cast<T>(_z.x))
    {}

    // -- Conversion vector constructors --

    template<typename T>
    template<typename A, typename B>
    constexpr vec<3, T>::vec(vec<2, A> const& _xy, B _z)
        : x(static_cast<T>(_xy.x))
        , y(static_cast<T>(_xy.y))
        , z(static_cast<T>(_z))
    {}

    template<typename T>
    template<typename A, typename B>
    constexpr vec<3, T>::vec(vec<2, A> const& _xy, vec<1, B> const& _z)
        : x(static_cast<T>(_xy.x))
        , y(static_cast<T>(_xy.y))
        , z(static_cast<T>(_z.x))
    {}

    template<typename T>
    template<typename A, typename B>
    constexpr vec<3, T>::vec(A _x, vec<2, B> const& _yz)
        : x(static_cast<T>(_x))
        , y(static_cast<T>(_yz.x))
        , z(static_cast<T>(_yz.y))
    {}

    template<typename T>
    template<typename A, typename B>
    constexpr vec<3, T>::vec(vec<1, A> const& _x, vec<2, B> const& _yz)
        : x(static_cast<T>(_x.x))
        , y(static_cast<T>(_yz.x))
        , z(static_cast<T>(_yz.y))
    {}

    template<typename T>
    template<typename U>
    constexpr vec<3, T>::vec(vec<3, U> const& v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.y))
        , z(static_cast<T>(v.z))
    {}

    template<typename T>
    template<typename U>
    constexpr vec<3, T>::vec(vec<4, U> const& v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.y))
        , z(static_cast<T>(v.z))
    {}

    // -- Component accesses --

    template<typename T>
    constexpr T & vec<3, T>::operator[](typename vec<3, T>::length_type i)
    {
        assert(i >= 0 && i < this->size());
        switch(i)
        {
        default:
            case 0:
        return x;
            case 1:
        return y;
            case 2:
        return z;
        }
    }

    template<typename T>
    constexpr T const& vec<3, T>::operator[](typename vec<3, T>::length_type i) const
    {
        assert(i >= 0 && i < this->size());
        switch(i)
        {
        default:
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        }
    }

    // -- Unary arithmetic operators --

    template<typename T>
    template<typename U>
    constexpr vec<3, T>& vec<3, T>::operator=(vec<3, U> const& v)
    {
        this->x = static_cast<T>(v.x);
        this->y = static_cast<T>(v.y);
        this->z = static_cast<T>(v.z);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<3, T> & vec<3, T>::operator+=(U scalar)
    {
        this->x += static_cast<T>(scalar);
        this->y += static_cast<T>(scalar);
        this->z += static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<3, T> & vec<3, T>::operator+=(vec<1, U> const& v)
    {
        this->x += static_cast<T>(v.x);
        this->y += static_cast<T>(v.x);
        this->z += static_cast<T>(v.x);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<3, T> & vec<3, T>::operator+=(vec<3, U> const& v)
    {
        this->x += static_cast<T>(v.x);
        this->y += static_cast<T>(v.y);
        this->z += static_cast<T>(v.z);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<3, T> & vec<3, T>::operator-=(U scalar)
    {
        this->x -= static_cast<T>(scalar);
        this->y -= static_cast<T>(scalar);
        this->z -= static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<3, T> & vec<3, T>::operator-=(vec<1, U> const& v)
    {
        this->x -= static_cast<T>(v.x);
        this->y -= static_cast<T>(v.x);
        this->z -= static_cast<T>(v.x);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<3, T> & vec<3, T>::operator-=(vec<3, U> const& v)
    {
        this->x -= static_cast<T>(v.x);
        this->y -= static_cast<T>(v.y);
        this->z -= static_cast<T>(v.z);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<3, T> & vec<3, T>::operator*=(U scalar)
    {
        this->x *= static_cast<T>(scalar);
        this->y *= static_cast<T>(scalar);
        this->z *= static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<3, T> & vec<3, T>::operator*=(vec<1, U> const& v)
    {
        this->x *= static_cast<T>(v.x);
        this->y *= static_cast<T>(v.x);
        this->z *= static_cast<T>(v.x);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<3, T> & vec<3, T>::operator*=(vec<3, U> const& v)
    {
        this->x *= static_cast<T>(v.x);
        this->y *= static_cast<T>(v.y);
        this->z *= static_cast<T>(v.z);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<3, T> & vec<3, T>::operator/=(U v)
    {
        this->x /= static_cast<T>(v);
        this->y /= static_cast<T>(v);
        this->z /= static_cast<T>(v);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<3, T> & vec<3, T>::operator/=(vec<1, U> const& v)
    {
        this->x /= static_cast<T>(v.x);
        this->y /= static_cast<T>(v.x);
        this->z /= static_cast<T>(v.x);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<3, T> & vec<3, T>::operator/=(vec<3, U> const& v)
    {
        this->x /= static_cast<T>(v.x);
        this->y /= static_cast<T>(v.y);
        this->z /= static_cast<T>(v.z);
        return *this;
    }

    // -- Increment and decrement operators --

    template<typename T>
    constexpr vec<3, T> & vec<3, T>::operator++()
    {
        ++this->x;
        ++this->y;
        ++this->z;
        return *this;
    }

    template<typename T>
    constexpr vec<3, T> & vec<3, T>::operator--()
    {
        --this->x;
        --this->y;
        --this->z;
        return *this;
    }

    template<typename T>
    constexpr vec<3, T> vec<3, T>::operator++(int)
    {
        vec<3, T> Result(*this);
        ++*this;
        return Result;
    }

    template<typename T>
    constexpr vec<3, T> vec<3, T>::operator--(int)
    {
        vec<3, T> Result(*this);
        --*this;
        return Result;
    }

    // -- helper functions -- 

    template<typename T>
    T vec<3, T>::Dot(vec<3, T> const& other) const
    {
        return dot(*this, other);
    };

    template<typename T>
    T vec<3, T>::SignedArea(vec<3, T> const& other) const
    {
        return signed_area(*this, other);
    }
    
    template<typename T>
    vec<3, T> vec<3, T>::Cross(vec<3, T> const& other) const
    {
        return cross(*this, other);
    }

    template<typename T>
    T vec<3, T>::Length2() const
    {
        return length2(*this);
    }

    template<typename T>
    T vec<3, T>::Length() const
    {
        return length(*this);
    }

    template<typename T>
    T vec<3, T>::Distance2(vec<3, T> const& other) const
    {
        return distance2(*this, other);
    }

    template<typename T>
    T vec<3, T>::Distance(vec<3, T> const& other) const
    {
        return distance(*this, other);
    }

    template<typename T>
    vec<3, T> vec<3, T>::Normalized() const
    {
        return normalized(*this);
    }

    template<typename T>
    vec<3, T>& vec<3, T>::Normalize()
    {
        return normalize(*this);
    }

    // -- Unary arithmetic operators --

    template<typename T>
    constexpr vec<3, T> operator+(vec<3, T> const& v)
    {
        return v;
    }

    template<typename T>
    constexpr vec<3, T> operator-(vec<3, T> const& v)
    {
        return vec<3, T>(
            -v.x,
            -v.y,
            -v.z);
    }

    // -- Binary arithmetic operators --

    template<typename T>
    constexpr vec<3, T> operator+(vec<3, T> const& v, T scalar)
    {
        return vec<3, T>(
            v.x + scalar,
            v.y + scalar,
            v.z + scalar);
    }

    template<typename T>
    constexpr vec<3, T> operator+(vec<3, T> const& v, vec<1, T> const& scalar)
    {
        return vec<3, T>(
            v.x + scalar.x,
            v.y + scalar.x,
            v.z + scalar.x);
    }

    template<typename T>
    constexpr vec<3, T> operator+(T scalar, vec<3, T> const& v)
    {
        return vec<3, T>(
            scalar + v.x,
            scalar + v.y,
            scalar + v.z);
    }

    template<typename T>
    constexpr vec<3, T> operator+(vec<1, T> const& scalar, vec<3, T> const& v)
    {
        return vec<3, T>(
            scalar.x + v.x,
            scalar.x + v.y,
            scalar.x + v.z);
    }

    template<typename T>
    constexpr vec<3, T> operator+(vec<3, T> const& v1, vec<3, T> const& v2)
    {
        return vec<3, T>(
            v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z);
    }

    template<typename T>
    constexpr vec<3, T> operator-(vec<3, T> const& v, T scalar)
    {
        return vec<3, T>(
            v.x - scalar,
            v.y - scalar,
            v.z - scalar);
    }

    template<typename T>
    constexpr vec<3, T> operator-(vec<3, T> const& v, vec<1, T> const& scalar)
    {
        return vec<3, T>(
            v.x - scalar.x,
            v.y - scalar.x,
            v.z - scalar.x);
    }

    template<typename T>
    constexpr vec<3, T> operator-(T scalar, vec<3, T> const& v)
    {
        return vec<3, T>(
            scalar - v.x,
            scalar - v.y,
            scalar - v.z);
    }

    template<typename T>
    constexpr vec<3, T> operator-(vec<1, T> const& scalar, vec<3, T> const& v)
    {
        return vec<3, T>(
            scalar.x - v.x,
            scalar.x - v.y,
            scalar.x - v.z);
    }

    template<typename T>
    constexpr vec<3, T> operator-(vec<3, T> const& v1, vec<3, T> const& v2)
    {
        return vec<3, T>(
            v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z);
    }

    template<typename T>
    constexpr vec<3, T> operator*(vec<3, T> const& v, T scalar)
    {
        return vec<3, T>(
            v.x * scalar,
            v.y * scalar,
            v.z * scalar);
    }

    template<typename T>
    constexpr vec<3, T> operator*(vec<3, T> const& v, vec<1, T> const& scalar)
    {
        return vec<3, T>(
            v.x * scalar.x,
            v.y * scalar.x,
            v.z * scalar.x);
    }

    template<typename T>
    constexpr vec<3, T> operator*(T scalar, vec<3, T> const& v)
    {
        return vec<3, T>(
            scalar * v.x,
            scalar * v.y,
            scalar * v.z);
    }

    template<typename T>
    constexpr vec<3, T> operator*(vec<1, T> const& scalar, vec<3, T> const& v)
    {
        return vec<3, T>(
            scalar.x * v.x,
            scalar.x * v.y,
            scalar.x * v.z);
    }

    template<typename T>
    constexpr vec<3, T> operator*(vec<3, T> const& v1, vec<3, T> const& v2)
    {
        return vec<3, T>(
            v1.x * v2.x,
            v1.y * v2.y,
            v1.z * v2.z);
    }

    template<typename T>
    constexpr vec<3, T> operator/(vec<3, T> const& v, T scalar)
    {
        return vec<3, T>(
            v.x / scalar,
            v.y / scalar,
            v.z / scalar);
    }

    template<typename T>
    constexpr vec<3, T> operator/(vec<3, T> const& v, vec<1, T> const& scalar)
    {
        return vec<3, T>(
            v.x / scalar.x,
            v.y / scalar.x,
            v.z / scalar.x);
    }

    template<typename T>
    constexpr vec<3, T> operator/(T scalar, vec<3, T> const& v)
    {
        return vec<3, T>(
            scalar / v.x,
            scalar / v.y,
            scalar / v.z);
    }

    template<typename T>
    constexpr vec<3, T> operator/(vec<1, T> const& scalar, vec<3, T> const& v)
    {
        return vec<3, T>(
            scalar.x / v.x,
            scalar.x / v.y,
            scalar.x / v.z);
    }

    template<typename T>
    constexpr vec<3, T> operator/(vec<3, T> const& v1, vec<3, T> const& v2)
    {
        return vec<3, T>(
            v1.x / v2.x,
            v1.y / v2.y,
            v1.z / v2.z);
    }


    // -- Boolean operators --

    template<typename T>
    constexpr bool operator==(vec<3, T> const& v1, vec<3, T> const& v2)
    {
        return equals(v1.x, v2.x) && equals(v1.y, v2.y) && equals(v1.z, v2.z);
    }

    template<typename T>
    constexpr bool operator!=(vec<3, T> const& v1, vec<3, T> const& v2)
    {
        return !(v1 == v2);
    }

}//namespace oom
