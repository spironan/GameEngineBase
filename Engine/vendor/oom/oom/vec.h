#pragma once

#include "fwd.h"

#include <array>

namespace oom
{
    template<std::size_t Size, typename T>
    struct vec
    {
        using value_type        = T;
        using const_value       = value_type const;
        using const_reference   = value_type const&;
        using reference         = value_type &;
        using const_reference   = value_type const&;
        
        using size_type         = size_t;

        using container         = std::array<value_type, Size>;

        static constexpr size_type VEC2 = 2;
        static constexpr size_type VEC3 = 3;
        static constexpr size_type VEC4 = 4;

    private:
        container val;

    public:

        static constexpr size_type size() { return Size; }
        
        //static constexpr vec zero() { return vec{} };

        vec() = default;
        ~vec() = default;
        vec(vec const& copy) = default;
        vec& operator=(vec const& copy) = default;
        vec& operator=(vec&& move) = default;

        template<typename S = value_type,
            typename = std::enable_if_t<std::is_arithmetic_v<std::decay_t<S>>> >
        explicit vec(S value) { for (size_type i = 0; i < Size; ++i) val[i] = static_cast<value_type>(value); };

        explicit vec(container container) : val{ container } {}

        // convert vector that is one dimension lower to its current dimension : homogeneous conversion, last element will be 0
        explicit vec(vec<Size - 1, value_type> vector)
        {
            for (size_type i = 0; i < Size - 1; ++i)
                val[i] = vector[i];
            val[Size - 1] = value_type{ 0 };
        }
        
        // convert array(vector) that is one dimension lower to its current dimension : homogeneous conversion, last element will be 0
        explicit vec(std::array<value_type, Size - 1> container) 
        { 
            for (size_type i = 0; i < Size - 1; ++i)
                val[i] = container[i];
            val[Size - 1] = value_type{0};
        }

        // convert point to a vector one dimension higher : homogeneous conversion, last element will be 1
        vec(point<Size - 1, value_type> const& point)
        {
            for (size_type i = 0; i < Size - 1; ++i)
                val[i] = point[i];
            val[Size - 1] = value_type{ 1 };
        }

        template <class ... Args, 
            typename = std::enable_if_t<(std::is_arithmetic_v<std::decay_t<Args>>&& ...) && (sizeof...(Args) == Size)>>
        vec(Args&&...args) : val{ std::forward<value_type>(static_cast<value_type>(args))... } {}

        template<std::size_t U = Size>
        std::enable_if_t<U >= VEC2, reference> x() { return val[0]; }
        template <std::size_t U = Size>
        std::enable_if_t<U >= VEC2, const_reference> x() const { return val[0]; };

        template<std::size_t U = Size>
        std::enable_if_t<U >= VEC2, reference> y() { return val[1]; }
        template <std::size_t U = Size>
        std::enable_if_t<U >= VEC2, const_reference> y() const { return val[1]; };

        template<std::size_t U = Size>
        std::enable_if_t<U >= VEC3, reference> z() { return val[2]; }
        template <std::size_t U = Size>
        std::enable_if_t<U >= VEC3, const_reference> z() const { return val[2]; };

        template<std::size_t U = Size>
        std::enable_if_t<U >= VEC4, reference> w() { return val[3]; }
        template <std::size_t U = Size>
        std::enable_if_t<U >= VEC4, const_reference> w() const { return val[3]; };


        const_value operator[](size_type pos) const { return val[pos]; }

        reference operator[](size_type pos) { return val[pos]; }

        template<typename S = value_type>
        std::enable_if_t<std::is_floating_point_v<S>, bool> operator==(vec<Size, S> const& other) const
        {
            for (size_type i = 0; i < Size; ++i)
            {
                if (!maths_utils::approx_equal(val[i], other.val[i])) return false;
            }

            return true;
        }

        template<typename S = value_type>
        std::enable_if_t<!std::is_floating_point_v<S>, bool> operator==(vec<Size, S>  const& other) const
        {
            for (size_type i = 0; i < Size; ++i)
            {
                if (val[i] != other.val[i]) return false;
            }

            return true;
        }

        bool operator!=(vec<Size, value_type> const& other) const
        {
            return !(*this == other);
        }

        vec<Size, value_type> operator+(vec<Size, value_type> const& other) const
        {
            vec<Size, value_type> out{ *this };
            for (size_type i = 0; i < Size; ++i)
            {
                out[i] += other.val[i];
            }
            return out;
        }

        vec<Size, value_type>& operator+=(vec<Size, value_type> const& other)
        {
            *this = *this + other;
            return *this;
        }

        vec<Size, value_type> operator*(const_value scalar) const
        {
            vec<Size, value_type> out{*this};
            for (size_type i = 0; i < Size; ++i)
            {
                out[i] *= scalar;
            }
            return out;
        }

        vec<Size, value_type>& operator*=(const_value scalar)
        {
            *this = *this * scalar;
            return *this;
        }

        vec<Size, value_type> operator/(const_value scalar) const
        {
            vec<Size, value_type> out{ *this };
            for (size_type i = 0; i < Size; ++i)
            {
                out[i] /= scalar;
            }
            return out;
        }

        vec<Size, value_type>& operator/=(const_value scalar)
        {
            *this = *this / scalar;
            return *this;
        }

        vec<Size, value_type> operator-(vec<Size, value_type> const& other) const
        {
            vec<Size, value_type> out{ *this };
            for (size_type i = 0; i < Size; ++i)
            {
                out[i] -= other.val[i];
            }
            return out;
        }

        vec<Size, value_type>& operator-=(vec<Size, value_type> const& other)
        {
            *this = *this - other;
            return *this;
        }

        vec<Size, value_type> operator-() const
        {
            vec<Size, value_type> res{ *this };
            for (size_type i = 0; i < Size; ++i)
                res[i] *= value_type{ -1 };
            return res;
        }

        point<Size, value_type> ConvertToPoint() const
        {
            return point<Size, value_type> { val };
        }

        // implicit cast to become a point3
        explicit operator point<Size, value_type>() const
        {
            return ConvertToPoint();
        };

        // implicit homogenous conversion
        operator vec<Size + 1, value_type>() const
        {
            return vec<Size + 1, value_type>{ val };
        }

        value_type Dot(vec<Size, value_type> const& other) const
        {
            value_type result {0};
            for (size_type i = 0; i < Size; ++i)
            {
                result += this->val[i] * other[i];
            }
            return result;
        }

        template<std::size_t U = Size>
        std::enable_if_t<U == VEC2, float> Cross(vec<VEC2, value_type> const& other) const noexcept
        {
            return val[0] * other.val[1] - val[1] * other.val[0];
        }

        template<std::size_t U = Size>
        std::enable_if_t<U == VEC3, vec<VEC3, value_type>> Cross(vec<VEC3, value_type> const& other) const noexcept
        {
            vec<VEC3, value_type> result{};

            result[0] = val[1] * other.val[2] - val[2] * other.val[1];
            result[1] = val[2] * other.val[0] - val[0] * other.val[2];
            result[2] = val[0] * other.val[1] - val[1] * other.val[0];

            return result;
        }

        value_type LengthSquared() const 
        {
            return Dot(*this);
        }

        value_type Length() const
        {
            return std::sqrtf(LengthSquared());
        }

        vec<Size, value_type> Normalized() const
        {
            return *this / Length();
        }

        vec<Size, value_type>& Normalize()
        {
            *this = Normalized();
            return *this;
        }

    };

}