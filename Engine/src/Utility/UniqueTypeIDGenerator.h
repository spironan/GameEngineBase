/************************************************************************************//*!
\file           UniqueTypeIDGenerator.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jul 07, 2021
\brief          Static utility class that uses templates to generate a unique type ID
                for each set of unique class combinations.

                A new ID is generated on Compile-Time if the function has not been
                called by that specific combination before otherwise the function would
                return the ID that was previously generated.
 
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <type_traits>

namespace engine{
namespace utility
{
    /********************************************************************************//*!
     @brief    Static utility class that uses templates to generate a unique type ID
               for each set of unique class combinations.

               A new ID is generated on Compile-Time if the function has not been
               called by that specific combination before otherwise the function would
               return the ID that was previously generated

     @tparam        Base
             Base class that will be used for matching with the derived type.
    *//*********************************************************************************/
    template <typename Base, typename ID = std::size_t>
    class UniqueTypeID final
    {
    public:

        //ensures that value_type is of an unsigned integral type
        using value_type = std::enable_if<std::is_unsigned<ID>::value, ID>::type;

        /* C++11 style */
        /****************************************************************************//*!
         @brief    Generates a value of this unique pairing of
                   derived and base class combination

         @tparam       Derived
             The derived class of the Base Template.
             Will not compile if its not derived

         @return   Value that can be used as a key for this unique combination of
                   derived, base relationship.
        *//*****************************************************************************/
        template<typename Derived>
        inline static typename std::enable_if<std::is_base_of<Base, Derived>::value, value_type>::type GetID() noexcept
        {
            static const value_type value = GetID();
            return value;
        }

    private:
        /****************************************************************************//*!
         @brief    GetID returns the same ID everytime for the same combination
                   that is called. This function generates an id of 0 if the base class
                   was never used before. The id can be used to identify that combination
                   of derived/base relationship.

         @return   Value to identify this unique derive/base class combination
        *//*****************************************************************************/
        inline static value_type GetID() noexcept
        {
            static value_type id = 0;
            return id++;
        }
    };
}} // namespace engine::utility