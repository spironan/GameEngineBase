/************************************************************************************//*!
\file           Hash.h
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
#pragma once

#include <string>

namespace engine {
namespace utility
{ 
    /********************************************************************************//*!
     @brief Contains a utility hashing class that stores the various hashing algorithms
            currently only supports fnv-1a hash.
    *//*********************************************************************************/
    struct StringHash
    {
    public:

        constexpr size_t const_strlen(const char* s)
        {
            size_t size = 0;
            while (s[size]) { size++; };
            return size;
        }
        /****************************************************************************//*!
         @brief Implementations the FNV-1a hashing algorithm.
                The fnv-1a implementation provides better avalanche characteristics
                against the fnv-1.
                The FNV hash was designed for fast hash tableand checksum use, not cryptography.
                Find out more
                https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function

         @return returns the hashed fnv-1a output.
        *//*****************************************************************************/
        constexpr uint32_t GenerateFNV1aHash(const char* str)
        {
            // Also C++ does not like static constexpr
            constexpr uint32_t FNV_PRIME = 16777619u;
            constexpr uint32_t OFFSET_BASIS = 2166136261u;

            const size_t length = const_strlen(str) + 1;
            uint32_t hash = OFFSET_BASIS;
            for (size_t i = 0; i < length; ++i)
            {
                hash ^= *str++;
                hash *= FNV_PRIME;
            }
            return hash;
        }
        constexpr uint32_t GenerateFNV1aHash(const std::string& string)
        {
            return GenerateFNV1aHash(string.c_str());
        }

        

        uint32_t computedHash;

        constexpr StringHash(uint32_t hash) noexcept : computedHash(hash) {}

        constexpr StringHash(const char* s) noexcept : computedHash(0)
        {
            computedHash = GenerateFNV1aHash(s);
        }
        constexpr StringHash(std::string_view s)noexcept : computedHash(0)
        {
            computedHash = GenerateFNV1aHash(s.data());
        }
        StringHash(const StringHash& other) = default;

        constexpr operator uint32_t()noexcept { return computedHash; }
    };

}}