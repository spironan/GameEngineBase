#pragma once

#include "../common.hpp"
#include "../geometric.hpp"

namespace oom
{
    // -- Explicit basic constructors --

    template<typename T>
    constexpr vec<1, T>::vec(T scalar)
        : x(scalar)
    {}

    // -- Conversion vector constructors --

    template<typename T>
    template<typename U>
    constexpr vec<1, T>::vec(vec<1, U> const& v)
        : x(static_cast<T>(v.x))
    {}

    template<typename T>
    template<typename U>
    constexpr vec<1, T>::vec(vec<2, U> const& v)
        : x(static_cast<T>(v.x))
    {}

    template<typename T>
    template<typename U>
    constexpr vec<1, T>::vec(vec<3, U> const& v)
        : x(static_cast<T>(v.x))
    {}

    template<typename T>
    template<typename U>
    constexpr vec<1, T>::vec(vec<4, U> const& v)
        : x(static_cast<T>(v.x))
    {}

    // -- Component accesses --

    template<typename T>
    constexpr T & vec<1, T>::operator[](typename vec<1, T>::length_type)
    {
        return x;
    }

    template<typename T>
    constexpr T const& vec<1, T>::operator[](typename vec<1, T>::length_type) const
    {
        return x;
    }

    // -- Unary arithmetic operators --

    template<typename T>
    template<typename U>
    constexpr vec<1, T> & vec<1, T>::operator=(vec<1, U> const& v)
    {
        this->x = static_cast<T>(v.x);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<1, T> & vec<1, T>::operator+=(U scalar)
    {
        this->x += static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<1, T> & vec<1, T>::operator+=(vec<1, U> const& v)
    {
        this->x += static_cast<T>(v.x);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<1, T> & vec<1, T>::operator-=(U scalar)
    {
        this->x -= static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<1, T> & vec<1, T>::operator-=(vec<1, U> const& v)
    {
        this->x -= static_cast<T>(v.x);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<1, T> & vec<1, T>::operator*=(U scalar)
    {
        this->x *= static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<1, T> & vec<1, T>::operator*=(vec<1, U> const& v)
    {
        this->x *= static_cast<T>(v.x);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<1, T> & vec<1, T>::operator/=(U scalar)
    {
        this->x /= static_cast<T>(scalar);
        return *this;
    }

    template<typename T>
    template<typename U>
    constexpr vec<1, T> & vec<1, T>::operator/=(vec<1, U> const& v)
    {
        this->x /= static_cast<T>(v.x);
        return *this;
    }

    // -- Increment and decrement operators --

    template<typename T>
    constexpr vec<1, T> & vec<1, T>::operator++()
    {
        ++this->x;
        return *this;
    }

    template<typename T>
    constexpr vec<1, T> & vec<1, T>::operator--()
    {
        --this->x;
        return *this;
    }

    template<typename T>
    constexpr vec<1, T> vec<1, T>::operator++(int)
    {
        vec<1, T> Result(*this);
        ++*this;
        return Result;
    }

    template<typename T>
    constexpr vec<1, T> vec<1, T>::operator--(int)
    {
        vec<1, T> Result(*this);
        --*this;
        return Result;
    }

    // -- helper functions -- 

    template<typename T>
    T vec<1, T>::Dot(vec<1, T> const& other) const
    {
        return dot(*this, other);
    };

    template<typename T>
    T vec<1, T>::Length2() const
    {
        return length2(*this);
    }

    template<typename T>
    T vec<1, T>::Distance2(vec<1, T> const& other) const
    {
        return distance2(*this, other);
    }

    // -- Unary constant operators --

    template<typename T>
    constexpr vec<1, T> operator+(vec<1, T> const& v)
    {
        return v;
    }

    template<typename T>
    constexpr vec<1, T> operator-(vec<1, T> const& v)
    {
        return vec<1, T>(
            -v.x);
    }

    // -- Binary arithmetic operators --

    template<typename T>
    constexpr vec<1, T> operator+(vec<1, T> const& v, T scalar)
    {
        return vec<1, T>(
            v.x + scalar);
    }

    template<typename T>
    constexpr vec<1, T> operator+(T scalar, vec<1, T> const& v)
    {
        return vec<1, T>(
            scalar + v.x);
    }

    template<typename T>
    constexpr vec<1, T> operator+(vec<1, T> const& v1, vec<1, T> const& v2)
    {
        return vec<1, T>(
            v1.x + v2.x);
    }

    //operator-
    template<typename T>
    constexpr vec<1, T> operator-(vec<1, T> const& v, T scalar)
    {
        return vec<1, T>(
            v.x - scalar);
    }

    template<typename T>
    constexpr vec<1, T> operator-(T scalar, vec<1, T> const& v)
    {
        return vec<1, T>(
            scalar - v.x);
    }

    template<typename T>
    constexpr vec<1, T> operator-(vec<1, T> const& v1, vec<1, T> const& v2)
    {
        return vec<1, T>(
            v1.x - v2.x);
    }

    template<typename T>
    constexpr vec<1, T> operator*(vec<1, T> const& v, T scalar)
    {
        return vec<1, T>(
            v.x * scalar);
    }

    template<typename T>
    constexpr vec<1, T> operator*(T scalar, vec<1, T> const& v)
    {
        return vec<1, T>(
            scalar * v.x);
    }

    template<typename T>
    constexpr vec<1, T> operator*(vec<1, T> const& v1, vec<1, T> const& v2)
    {
        return vec<1, T>(
            v1.x * v2.x);
    }

    template<typename T>
    constexpr vec<1, T> operator/(vec<1, T> const& v, T scalar)
    {
        return vec<1, T>(
            v.x / scalar);
    }

    template<typename T>
    constexpr vec<1, T> operator/(T scalar, vec<1, T> const& v)
    {
        return vec<1, T>(
            scalar / v.x);
    }

    template<typename T>
    constexpr vec<1, T> operator/(vec<1, T> const& v1, vec<1, T> const& v2)
    {
        return vec<1, T>(
            v1.x / v2.x);
    }


    // -- Boolean operators --

    template<typename T>
    constexpr bool operator==(vec<1, T> const& v1, vec<1, T> const& v2)
    {
        return equals(v1.x, v2.x);
    }

    template<typename T>
    constexpr bool operator!=(vec<1, T> const& v1, vec<1, T> const& v2)
    {
        return !(v1 == v2);
    }

    

}//namespace oom
