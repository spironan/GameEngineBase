/************************************************************************************//*!
\file           Hash.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           June 14, 2021
\brief          Contains a utility hashing class that stores the various hashing algorithms
                currently only supports fnv-1a hash.


Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "Hash.h"

namespace engine{
namespace utility
{
    uint32_t Hash::GenerateFNV1aHash(const char* str)
    {
        static constexpr uint32_t FNV_PRIME     = 16777619u;
        static constexpr uint32_t OFFSET_BASIS  = 2166136261u;

        const size_t length = strlen(str) + 1;
        uint32_t hash = OFFSET_BASIS;
        for (size_t i = 0; i < length; ++i)
        {
            hash ^= *str++;
            hash *= FNV_PRIME;
        }
        return hash;
    }

    uint32_t Hash::GenerateFNV1aHash(const std::string& string)
    {
        return GenerateFNV1aHash(string.c_str());
    }
}}