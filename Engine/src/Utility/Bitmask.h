/************************************************************************************//*!
\file           Bitmask.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 15, 2021
\brief          Contains a templated class that allows the use of type-safe enums classes
                as a bitmask.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <type_traits>

namespace Engine 
{
    /********************************************************************************//*!
        @brief    Allows enum classes to be used as a bitmask
    *//*********************************************************************************/
    template <typename option_type,
        // The line below ensures that bitmask can only be used with enums
        typename = typename std::enable_if_t<std::is_enum_v<option_type>>>
        class bitmask
    {
    public:
        /********************************************************************************//*!
            @brief     The type we'll use for storing the value of our bitmask
                    should be the same as the enum's underlying type.
        *//*********************************************************************************/
        using underlying_type = typename std::underlying_type<option_type>::type;
    public:

        /********************************************************************************//*!
            @brief     Default ctor creates a bitmask with no options selected.
        *//*********************************************************************************/
        constexpr bitmask() : mask(0) {}

        /********************************************************************************//*!
            @brief     Creates a bitmask with just one bit set.
                    This ctor is intentionally non-explicit, to allow for stuff like:
                    bitmask<enum> bitmask(enum::option1)
        *//*********************************************************************************/
        constexpr bitmask(option_type o) : mask(mask_value(o)) {}

        /********************************************************************************//*!
            @brief     Set the bit corresponding to the given option.
                    Functions the same as the bitwise | operator

            @return    Returns a bitmask of the resultant operation
        *//*********************************************************************************/
        constexpr bitmask operator|(option_type t)
        {
            return bitmask(mask | mask_value(t));
        }

        /********************************************************************************//*!
            @brief     Set the bit corresponding to the given option and appply to current
                    mask.
                    Functions the same as the bitwise |= operator

            @return    Returns a bitmask of the resultant operation
        *//*********************************************************************************/
        constexpr bitmask operator|=(option_type t)
        {
            mask = mask | mask_value(t);
            return bitmask(mask);
        }

        /********************************************************************************//*!
            @brief     Get the value of the bit corresponding to the given option.
                    Functions the same as the bitwise & operator

            @return    boolean determining if the current mask contains what was passed in.
        *//*********************************************************************************/
        constexpr bool operator&(option_type t)
        {
            return mask & mask_value(t);
        }

        /********************************************************************************//*!
            @brief     Get the value of the bit corresponding to the given option
                    and apply it to the current mask.
                    Functions the same as the bitwise &= operator

            @return    Returns a bitmask of the resultant operation
        *//*********************************************************************************/
        constexpr bitmask operator&=(option_type t)
        {
            mask = mask & mask_value(t);
            return bitmask(mask);
        }

        /********************************************************************************//*!
            @brief     Allows users who knows what they are doing to abstract
                    the mask underlying value via static_cast explicitly:
                    static_cast<bitmask<enum>::underlying_type>(bitmask)

            @return    Returns the underlying type of the mask value
        *//*********************************************************************************/
        explicit constexpr operator underlying_type () const { return mask; }

    private:

        /********************************************************************************//*!
            @brief     This method helps us avoid having to explicitly set enum values
                    to powers of two.
        *//*********************************************************************************/
        static constexpr underlying_type mask_value(option_type o)
        {
            return 1 << static_cast<underlying_type>(o);
        }

        /********************************************************************************//*!
            @brief     Private ctor to be used internally.
        *//*********************************************************************************/
        explicit constexpr bitmask(underlying_type o) : mask(o) {}

        underlying_type mask = 0;
    };

    /********************************************************************************//*!
     @brief     Creates a bitmask from two options, convenient for stuff like:
                bitmask(enum::flag1|enum::flag2|enum::flag3)
    *//*********************************************************************************/

    template <class option_type,
        typename = typename std::enable_if_t<std::is_enum_v<option_type>>>
        constexpr bitmask<option_type> operator|(option_type lhs, option_type rhs)
    {
        return bitmask<option_type>{lhs} | rhs;
    }
}
