/************************************************************************************//*!
\file           UUID.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Sept 09, 2021
\brief          File describing a Universally Unique Identifier
                "UUID" (universally unique identifier) or GUID is (usually) a 128-bit integer
                used to "uniquely" identify information.
                What is a GUID? http://guid.one/guid
                Mordern Solution to UUID with standard library :
                https://stackoverflow.com/questions/24365331/how-can-i-generate-uuid-in-c-without-using-boost-library
                The implementation is not truly a 128-bit integer randomness
                but using a 64-bit random device right now. Clash rate should be extremely low even then
                for a scope that isnt incredibly huge.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Base.h"

namespace engine
{
    class UUID
    {
    public:
        

        UUID();
        UUID(UUID const& other) = default;

        //Conversion constructor
        UUID(uint64_t uuid) : m_uuid{ uuid } {};

        operator uint64_t() { return m_uuid; }
        operator const uint64_t() const { return m_uuid; }

    private:
        uint64_t m_uuid;
    };
}
