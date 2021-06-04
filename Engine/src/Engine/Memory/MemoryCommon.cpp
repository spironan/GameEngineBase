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
