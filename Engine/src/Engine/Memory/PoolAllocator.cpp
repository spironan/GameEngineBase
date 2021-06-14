#include "pch.h"
#include "PoolAllocator.h"
#include <assert.h>

namespace engine
{
    //default constructor
    PoolAllocator::PoolAllocator()
    {
    }
    /*****************************************************************//**
     * @brief explicit Constructor for the PoolAllocator class.
     *
     * @param chunkSize the fixed size of each individual chunk
     * @param count how many chunks are
     * @param increment how many chunks are added at a time when the
     * current pool reaches the maximum size
    *********************************************************************/
    PoolAllocator::PoolAllocator(const Size chunkSize, const Size count,
        const Size increment) :
        m_capacity(count),
        m_chunkSize(chunkSize),
        m_increment(increment)
    {
        assert(increment != 0);
        // Using PoolAllocator for chunkSize smaller than pointer will not
        // work because of the memory optimization technique
        auto t = sizeof(PoolNode*);
        assert(chunkSize > sizeof(PoolNode*));

        m_memHead = m_alloc(chunkSize * count);
        InitializeNodes(m_memHead, count);
    }

    PoolAllocator::PoolAllocator(Size chunkSize, Size count, Size increment, void* head)
    {
        assert(increment != 0);
        // Using PoolAllocator for chunkSize smaller than pointer will not
        // work because of the memory optimization technique
        auto t = sizeof(PoolNode*);
        assert(chunkSize > sizeof(PoolNode*));

        m_memHead = head;
        InitializeNodes(m_memHead, count);
    }

    PoolAllocator::~PoolAllocator() {
        m_free(m_memHead);
        for (void* memory : m_additionalMemory)
        {
            m_free(memory);
        }
    }

    void PoolAllocator::Init(Size chunkSize, Size count, Size increment, void* head)
    {
        assert(increment != 0);
        // Using PoolAllocator for chunkSize smaller than pointer will not
        // work because of the memory optimization technique
        auto t = sizeof(PoolNode*);
        assert(chunkSize > sizeof(PoolNode*));

        m_memHead = head;
        InitializeNodes(m_memHead, count);
    }

    void* PoolAllocator::Get()
    {
        if (m_head == nullptr)
        {
            Expand();
        }

        void* ret = m_head;
        m_head = m_head->next;
        memset(ret, 0x0, sizeof(PoolNode));  // rest is set during free process
        return ret;
    }

    void PoolAllocator::Free(void* mem)
    {
        memset(mem, 0x0, m_chunkSize);
        PoolNode* newHead = new (mem) PoolNode{ m_head };
        m_head = newHead;
    }

    void PoolAllocator::SetAlloc(AllocFP fp)
    {
        m_alloc = fp;
    }

    void PoolAllocator::SetFree(FreeFP fp)
    {
        m_free = fp;
    }

    void PoolAllocator::EnableExpansion(bool expand)
    {
        m_expandPool = expand;
    }

    void PoolAllocator::InitializeNodes(void* memHead, const Size count)
    {
        m_head = new (memHead) PoolNode{ nullptr };
        PoolNode* cur = m_head;
        PtrInt address = reinterpret_cast<PtrInt>(memHead);
        for (Size i = 1; i < count; ++i) {
            address += m_chunkSize;
            PoolNode* node = new (reinterpret_cast<void*>(address)) PoolNode{ nullptr };
            cur->next = node;
            cur = cur->next;
        }
    }

    void PoolAllocator::Expand()
    {
        if (m_expandPool == false) return;
        void* newMemHead = m_alloc(m_chunkSize * m_increment);
        InitializeNodes(newMemHead, m_increment);
        m_additionalMemory.emplace_back(newMemHead);
        m_capacity += m_increment;
        std::cout << "Pool allocator increased by " << m_increment << std::endl;
    }

}