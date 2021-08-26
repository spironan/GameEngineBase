#pragma once

#include <cassert>
#include "../common.hpp"
#include "../geometric.hpp"

namespace oom
{
    // -- Explicit basic constructors --

    template<typename T>
    constexpr vec<2, T>::vec(T scalar)
        : x(scalar), y(scalar)
    {}

    template<typename T>
    constexpr vec<2, T>::vec(T _x, T _y)
        : x(_x), y(_y)
    {}

    // -- Conversion scalar constructors --

    template<typename T>
    template<typename U>
    constexpr vec<2, T>::vec(vec<1, U> const& v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.x))
    {}

    template<typename T>
    template<typename A, typename B>
    constexpr vec<2, T>::vec(A _x, B _y)
        : x(static_cast<T>(_x))
        , y(static_cast<T>(_y))
    {}

    template<typename T>
    template<typename A, typename B>
    constexpr vec<2, T>::vec(vec<1, A> const& _x, B _y)
        : x(static_cast<T>(_x.x))
        , y(static_cast<T>(_y))
    {}

    template<typename T>
    template<typename A, typename B>
    constexpr vec<2, T>::vec(A _x, vec<1, B> const& _y)
        : x(static_cast<T>(_x))
        , y(static_cast<T>(_y.x))
    {}

    template<typename T>
    template<typename A, typename B>
    constexpr vec<2, T>::vec(vec<1, A> const& _x, vec<1, B> const& _y)
        : x(static_cast<T>(_x.x))
        , y(static_cast<T>(_y.x))
    {}

    // -- Conversion vector constructors --

    template<typename T>
    template<typename U>
    constexpr vec<2, T>::vec(vec<2, U> const& v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.y))
    {}

    template<typename T>
    template<typename U>
    constexpr vec<2, T>::vec(vec<3, U> const& v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.y))
    {}

    template<typename T>
    template<typename U>
    constexpr vec<2, T>::vec(vec<4, U> const& v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.y))
    {}

    // -- Component accesses --

    template<typename T>
    constexpr T & vec<2, T>::operator[](typename vec<2, T>::length_type i)
    {
        assert(i >= 0 && i < this->size());
        switch(i)
        {
        default:
        case 0:
            return x;
        case 1:
            return y;
        }
    }

    template<typename T>
    constexpr T const& vec<2, T>::operator[](typename vec<2, T>::length_type i) const
    {
        assert(i >= 0 && i < this->size());
        switch(i)
        {
        default:
        case 0:
            return x;
        case 1:
            return y;
        }
    }

    // -- Unary arithmetic operators --

    template<typename T>
    template<typename U>
    constexpr vec<2, T> & vec<2, T>::operator=(vec<2, U> const& v)
    {
        this->x = static_cast<T>(v.x);
        this->y = static_cast<T>(v.y);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<2, T> & vec<2, T>::operator+=(U scalar)
    {
        this->x += static_cast<T>(scalar);
        this->y += static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<2, T> & vec<2, T>::operator+=(vec<1, U> const& v)
    {
        this->x += static_cast<T>(v.x);
        this->y += static_cast<T>(v.x);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<2, T> & vec<2, T>::operator+=(vec<2, U> const& v)
    {
        this->x += static_cast<T>(v.x);
        this->y += static_cast<T>(v.y);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<2, T> & vec<2, T>::operator-=(U scalar)
    {
        this->x -= static_cast<T>(scalar);
        this->y -= static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<2, T> & vec<2, T>::operator-=(vec<1, U> const& v)
    {
        this->x -= static_cast<T>(v.x);
        this->y -= static_cast<T>(v.x);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<2, T> & vec<2, T>::operator-=(vec<2, U> const& v)
    {
        this->x -= static_cast<T>(v.x);
        this->y -= static_cast<T>(v.y);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<2, T> & vec<2, T>::operator*=(U scalar)
    {
        this->x *= static_cast<T>(scalar);
        this->y *= static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<2, T> & vec<2, T>::operator*=(vec<1, U> const& v)
    {
        this->x *= static_cast<T>(v.x);
        this->y *= static_cast<T>(v.x);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<2, T> & vec<2, T>::operator*=(vec<2, U> const& v)
    {
        this->x *= static_cast<T>(v.x);
        this->y *= static_cast<T>(v.y);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<2, T> & vec<2, T>::operator/=(U scalar)
    {
        this->x /= static_cast<T>(scalar);
        this->y /= static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<2, T> & vec<2, T>::operator/=(vec<1, U> const& v)
    {
        this->x /= static_cast<T>(v.x);
        this->y /= static_cast<T>(v.x);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<2, T> & vec<2, T>::operator/=(vec<2, U> const& v)
    {
        this->x /= static_cast<T>(v.x);
        this->y /= static_cast<T>(v.y);
        return *this;
    }

    // -- Increment and decrement operators --

    template<typename T>
    constexpr vec<2, T> & vec<2, T>::operator++()
    {
        ++this->x;
        ++this->y;
        return *this;
    }

    template<typename T>
    constexpr vec<2, T> & vec<2, T>::operator--()
    {
        --this->x;
        --this->y;
        return *this;
    }

    template<typename T>
    constexpr vec<2, T> vec<2, T>::operator++(int)
    {
        vec<2, T> Result(*this);
        ++*this;
        return Result;
    }

    template<typename T>
    constexpr vec<2, T> vec<2, T>::operator--(int)
    {
        vec<2, T> Result(*this);
        --*this;
        return Result;
    }

    // -- helper functions -- 

    template<typename T>
    T vec<2, T>::Dot(vec<2, T> const& other) const
    {
        return dot(*this, other);
    };

    template<typename T>
    T vec<2, T>::Cross(vec<2, T> const& other) const
    {
        return cross(*this, other);
    }

    template<typename T>
    T vec<2, T>::Length2() const
    {
        return length2(*this);
    }

    template<typename T>
    T vec<2, T>::Length() const
    {
        return length(*this);
    }

    template<typename T>
    T vec<2, T>::Distance2(vec<2, T> const& other) const
    {
        return distance2(*this, other);
    }

    template<typename T>
    T vec<2, T>::Distance(vec<2, T> const& other) const
    {
        return distance(*this, other);
    }

    template<typename T>
    vec<2, T> vec<2, T>::Normalized() const
    {
        return normalized(*this);
    }

    template<typename T>
    vec<2, T>& vec<2, T>::Normalize()
    {
        return normalize(*this);
    }

    // -- Unary arithmetic operators --

    template<typename T>
    constexpr vec<2, T> operator+(vec<2, T> const& v)
    {
        return v;
    }

    template<typename T>
    constexpr vec<2, T> operator-(vec<2, T> const& v)
    {
        return vec<2, T>(
            -v.x,
            -v.y);
    }

    // -- Binary arithmetic operators --

    template<typename T>
    constexpr vec<2, T> operator+(vec<2, T> const& v, T scalar)
    {
        return vec<2, T>(
            v.x + scalar,
            v.y + scalar);
    }

    template<typename T>
    constexpr vec<2, T> operator+(vec<2, T> const& v1, vec<1, T> const& v2)
    {
        return vec<2, T>(
            v1.x + v2.x,
            v1.y + v2.x);
    }

    template<typename T>
    constexpr vec<2, T> operator+(T scalar, vec<2, T> const& v)
    {
        return vec<2, T>(
            scalar + v.x,
            scalar + v.y);
    }

    template<typename T>
    constexpr vec<2, T> operator+(vec<1, T> const& v1, vec<2, T> const& v2)
    {
        return vec<2, T>(
            v1.x + v2.x,
            v1.x + v2.y);
    }

    template<typename T>
    constexpr vec<2, T> operator+(vec<2, T> const& v1, vec<2, T> const& v2)
    {
        return vec<2, T>(
            v1.x + v2.x,
            v1.y + v2.y);
    }

    template<typename T>
    constexpr vec<2, T> operator-(vec<2, T> const& v, T scalar)
    {
        return vec<2, T>(
            v.x - scalar,
            v.y - scalar);
    }

    template<typename T>
    constexpr vec<2, T> operator-(vec<2, T> const& v1, vec<1, T> const& v2)
    {
        return vec<2, T>(
            v1.x - v2.x,
            v1.y - v2.x);
    }

    template<typename T>
    constexpr vec<2, T> operator-(T scalar, vec<2, T> const& v)
    {
        return vec<2, T>(
            scalar - v.x,
            scalar - v.y);
    }

    template<typename T>
    constexpr vec<2, T> operator-(vec<1, T> const& v1, vec<2, T> const& v2)
    {
        return vec<2, T>(
            v1.x - v2.x,
            v1.x - v2.y);
    }

    template<typename T>
    constexpr vec<2, T> operator-(vec<2, T> const& v1, vec<2, T> const& v2)
    {
        return vec<2, T>(
            v1.x - v2.x,
            v1.y - v2.y);
    }

    template<typename T>
    constexpr vec<2, T> operator*(vec<2, T> const& v, T scalar)
    {
        return vec<2, T>(
            v.x * scalar,
            v.y * scalar);
    }

    template<typename T>
    constexpr vec<2, T> operator*(vec<2, T> const& v1, vec<1, T> const& v2)
    {
        return vec<2, T>(
            v1.x * v2.x,
            v1.y * v2.x);
    }

    template<typename T>
    constexpr vec<2, T> operator*(T scalar, vec<2, T> const& v)
    {
        return vec<2, T>(
            scalar * v.x,
            scalar * v.y);
    }

    template<typename T>
    constexpr vec<2, T> operator*(vec<1, T> const& v1, vec<2, T> const& v2)
    {
        return vec<2, T>(
            v1.x * v2.x,
            v1.x * v2.y);
    }

    template<typename T>
    constexpr vec<2, T> operator*(vec<2, T> const& v1, vec<2, T> const& v2)
    {
        return vec<2, T>(
            v1.x * v2.x,
            v1.y * v2.y);
    }

    template<typename T>
    constexpr vec<2, T> operator/(vec<2, T> const& v, T scalar)
    {
        return vec<2, T>(
            v.x / scalar,
            v.y / scalar);
    }

    template<typename T>
    constexpr vec<2, T> operator/(vec<2, T> const& v1, vec<1, T> const& v2)
    {
        return vec<2, T>(
            v1.x / v2.x,
            v1.y / v2.x);
    }

    template<typename T>
    constexpr vec<2, T> operator/(T scalar, vec<2, T> const& v)
    {
        return vec<2, T>(
            scalar / v.x,
            scalar / v.y);
    }

    template<typename T>
    constexpr vec<2, T> operator/(vec<1, T> const& v1, vec<2, T> const& v2)
    {
        return vec<2, T>(
            v1.x / v2.x,
            v1.x / v2.y);
    }

    template<typename T>
    constexpr vec<2, T> operator/(vec<2, T> const& v1, vec<2, T> const& v2)
    {
        return vec<2, T>(
            v1.x / v2.x,
            v1.y / v2.y);
    }

    // -- Boolean operators --

    template<typename T>
    constexpr bool operator==(vec<2, T> const& v1, vec<2, T> const& v2)
    {
        return equals(v1.x, v2.x) && equals(v1.y, v2.y);
    }

    template<typename T>
    constexpr bool operator!=(vec<2, T> const& v1, vec<2, T> const& v2)
    {
        return !(v1 == v2);
    }

}//namespace oom
