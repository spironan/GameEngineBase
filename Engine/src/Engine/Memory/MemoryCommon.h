/*****************************************************************//**
\file   MemoryCommon.h
\author Lim Guan Hui, l.guanhui , 2000552
\email  l.guanhui@digipen.edu
\date   5/6/2021
\brief  
This header contains the new operator overloads to enable memory 
management in the game engine.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include <cstdio>
#include <cstdlib>
#include <new>
#include "MemoryManager.h"
void* operator new (std::size_t size);
void* operator new (std::size_t size, const char* filename, int line);

void operator delete(void* ptr) noexcept;


#define OONEW new(__FILE__, __LINE__)