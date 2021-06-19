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
    class Hash
    {
    public:

        /****************************************************************************//*!
         @brief Implementations the FNV-1a hashing algorithm.
                The fnv-1a implementation provides better avalanche characteristics
                against the fnv-1.
                The FNV hash was designed for fast hash tableand checksum use, not cryptography.
                Find out more
                https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function

         @return returns the hashed fnv-1a output.
        *//*****************************************************************************/
        static uint32_t GenerateFNV1aHash(const char* str);
        static uint32_t GenerateFNV1aHash(const std::string& string);
    };
}}