#pragma once
#include <iostream>
#include <vector>
#include <functional>

namespace engine
{
    class PoolAllocator {
        using Size = std::size_t;
        using U8 = uint8_t;
        using PtrInt = uintptr_t;
        using PtrDiff = std::ptrdiff_t;
        using AllocFP = std::function<void* (std::size_t)>;
        using FreeFP = std::function<void(void*)>;
    public:
        PoolAllocator();
        explicit PoolAllocator(Size chunkSize, Size count, Size increment);
        explicit PoolAllocator(Size chunkSize, Size count, Size increment, void* head);
        ~PoolAllocator();
        void Init(Size chunkSize, Size count, Size increment, void* head);
        void* Get();
        void Free(void*);
        void SetAlloc(AllocFP fp);
        void SetFree(FreeFP fp);
        /*****************************************************************//**
         * @brief Sets if pool expands when maximum size is reached.
         * if set to false, pool returns null pointer when maximum size is
         * reached
         *
         * @param expand true or false depending on if expanding the pool
         * size is required or not
        *********************************************************************/
        void EnableExpansion(bool expand);
    private:
        union PoolNode {
            PoolNode* next;
            explicit PoolNode(PoolNode* next) { this->next = next; }
        };

        void InitializeNodes(void* memHead, Size count);
        void Expand();

        Size m_capacity{};
        Size m_chunkSize{};
        Size m_increment{};
        PoolNode* m_head{};
        void* m_memHead{};
        std::vector<void*> m_additionalMemory;
        AllocFP m_alloc = [](std::size_t sz)-> void* {
            return std::malloc(sz); };
        FreeFP m_free = [](void* ptr)-> void {
            std::free(ptr); };
        bool m_expandPool = true;
    };

}
