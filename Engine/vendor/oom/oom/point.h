#pragma once

#include "fwd.h"

#include <cstddef>
#include <array>

namespace oom
{
    template<std::size_t Size, typename T>
    struct point
    {
        using value_type        = T;
        using reference         = value_type&;
        using const_value       = value_type const;
        using const_reference   = value_type const&;

        using size_type         = size_t;
        
        using container         = std::array<value_type, Size>;

        static constexpr size_type POINT2 = 2;
        static constexpr size_type POINT3 = 3;
        static constexpr size_type POINT4 = 4;
    private:
        container val;

    public:
        point() = default;
        ~point() = default;
        point(point const& copy) = default;
        point& operator=(point const& copy) = default;
        point& operator=(point&& move) = default;

        template<typename S = value_type,
            typename = std::enable_if_t<std::is_arithmetic_v<std::decay_t<S>>> >
        explicit point(S value) { for (size_type i = 0; i < Size; ++i) val[i] = static_cast<value_type>(value); };

        explicit point(container container) : val{ container } {}

        //homogenous promotion : last point is 1
        explicit point(std::array<value_type, Size - 1> container)
        {
            for (size_type i = 0; i < Size - 1; ++i)
                val[i] = container[i];
            val[Size - 1] = value_type{ 1 };
        }

        template <class ... Args,
            typename = std::enable_if_t<(std::is_arithmetic_v<std::decay_t<Args>>&& ...) && (sizeof...(Args) == Size)>>
        point(Args&&...args) : val { std::forward<value_type>(static_cast<value_type>(args))... } {}

        template<std::size_t U = Size>
        std::enable_if_t<U >= POINT2, reference> x() { return val[0]; }
        template <std::size_t U = Size>
        std::enable_if_t<U >= POINT2, const_reference> x() const { return val[0]; };

        template<std::size_t U = Size>
        std::enable_if_t<U >= POINT2, reference> y() { return val[1]; }
        template <std::size_t U = Size>
        std::enable_if_t<U >= POINT2, const_reference> y() const { return val[1]; };

        template<std::size_t U = Size>
        std::enable_if_t<U >= POINT3, reference> z() { return val[2]; }
        template <std::size_t U = Size>
        std::enable_if_t<U >= POINT3, const_reference> z() const { return val[2]; };

        template<std::size_t U = Size>
        std::enable_if_t<U >= POINT4, reference> w() { return val[3]; }
        template <std::size_t U = Size>
        std::enable_if_t<U >= POINT4, const_reference> w() const { return val[3]; };

        static constexpr size_type size() { return Size; }

        const_value operator[](size_type pos) const { return val[pos]; }
        reference operator[](size_type pos) { return val[pos]; }

        template<typename S = value_type>
        std::enable_if_t<std::is_floating_point_v<S>, bool>
        operator==(point<Size, S> const other) const
        {
            for (int i = 0; i < Size; ++i)
            {
                if (!approx_equal(val[i], other[i])) return false;
            }

            return true;
        }

        template<typename S = value_type>
        std::enable_if_t<!std::is_floating_point_v<S>, bool>
        operator==(point<Size, S> const other) const
        {
            for (int i = 0; i < Size; ++i)
            {
                if (val[i] != other.val[i]) return false;
            }
            return true;
        }

        bool operator!=(point<Size, value_type> const other) const
        {
            return !(*this == other);
        }

        vec<Size, value_type> operator-(point<Size, value_type> const other) const
        {
            vec<Size, value_type> out{ };
            for (int i = 0; i < Size; ++i)
            {
                out[i] = this->val[i] - other.val[i];
            }
            return out;
        }
        point<Size, value_type> operator-(vec<Size, value_type> const other) const
        {
            point<Size, value_type> result{ };
            for (int i = 0; i < Size; ++i)
            {
                result[i] = this->val[i] - other[i];
            }
            return result;
        }
        point<Size, value_type>& operator-=(vec<Size, value_type> const other)
        {
            *this = *this - other;
            return *this;
        }
        point<Size, value_type> operator+(vec<Size, value_type> const other) const
        {
            point<Size, value_type> result{ };
            for (int i = 0; i < Size; ++i)
            {
                result[i] = this->val[i] + other[i];
            }
            return result;
        }
        point<Size, value_type>& operator+=(vec<Size, value_type> const other)
        {
            *this = *this + other;
            return *this;
        }

        point<Size, value_type> operator-() const
        {
            point<Size, value_type> res{ *this };
            for (size_type i = 0; i < Size; ++i)
                res[i] *= value_type{ -1 };
            return res;
        }

        operator point<Size + 1, value_type>() const
        {
            point<Size + 1, value_type> result{ val };
            result[Size] = value_type{ 1 };
            return result;
        }

        explicit operator vec<Size, value_type>() const
        {
            return vec<Size, value_type>{ val };
        }
    };

    template<std::size_t Size, typename value_type>
    point<Size, value_type> operator+(vec<Size,value_type> vec, point<Size,value_type> point)
    {
        return point + vec;
    }

}