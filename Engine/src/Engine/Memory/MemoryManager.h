#pragma once
#include <cstddef>
#include "StackAllocator.h"
#include "PoolAllocator.h"
class MemoryManager
{
	using SA = StackAllocator;
	using PA = PoolAllocator;
	using Size = std::size_t;
public:
	static const Size BYTE;
	static const Size KB;
	static const Size MB;
	static const Size GB;
public:
	MemoryManager() = delete;
	MemoryManager(Size mem_size, Size stack_size, Size pool_chunk_size);
	~MemoryManager();

	template<typename type, typename... args>
	type* Allocate(args... argList);
	template<typename type>
	type* AllocateArray(std::size_t num);

	template<typename type, typename... args>
	type* AllocatePersistent(args... argList);

	void Clear();
private:
	static MemoryManager* GetInstance();
	static MemoryManager* instance;
	SA m_stackAlloc;
	PA m_poolAlloc;
	std::size_t m_total_size;
	SA m_persistentAlloc;
};


/*************************************************
* Definitions
**************************************************/
template<typename type, typename... args>
inline type* MemoryManager::Allocate(args... argList)
{
	return m_poolAlloc.Get();
}

template<typename type>
inline type* MemoryManager::AllocateArray(std::size_t num)
{
	return NULL;
}

template<typename type, typename... args>
inline type* MemoryManager::AllocatePersistent(args... argList)
{
	return m_persistentAlloc.New<type>(argList);
}
