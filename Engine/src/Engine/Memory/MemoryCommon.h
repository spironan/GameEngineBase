#pragma once
#include <cstdio>
#include <cstdlib>
#include <new>
void* operator new (std::size_t size);
void* operator new (std::size_t size, const char* filename, int line);

void operator delete(void* ptr) noexcept;


#define OONEW new(__FILE__, __LINE__)