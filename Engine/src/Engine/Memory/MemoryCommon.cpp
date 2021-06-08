/*****************************************************************//**
\file   MemoryCommon.cpp
\author Lim Guan Hui, l.guanhui , 2000552
\email  l.guanhui@digipen.edu
\date   5/6/2021
\brief  
This file contains definitions for the global new operator overloads
defined in the header.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#include "pch.h"
#include "MemoryCommon.h"
#include "Engine/Core/Base.h"
//#include "MemoryManager.h"

void* operator new (std::size_t size)
{
    //std::printf("global new called, size = %zu\n", size);
    if (size == 0)
        ++size; // avoid std::malloc(0) which may return nullptr on success

    if (void* ptr = std::malloc(size))
        return ptr;

    throw std::bad_alloc{};
}


void* operator new (std::size_t size, const char* filename, int line) {
    void* ptr = std::malloc(size);
    //std::cout << "size: " << size << " filename: " << filename << " line: " << line << std::endl;
    LOG_ENGINE_INFO("size: {0} filename: {1} line: {2}", size, filename, line);
    return ptr;
}

void operator delete(void* ptr) noexcept
{
    std::free(ptr);
}
