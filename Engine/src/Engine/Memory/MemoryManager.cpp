#include "pch.h"
#include "Engine/Core/Assert.h"
#include "MemoryManager.h"
const MemoryManager::Size MemoryManager::BYTE = 1;
const MemoryManager::Size MemoryManager::KB = 1024 * BYTE;
const MemoryManager::Size MemoryManager::MB = 1024 * KB;
const MemoryManager::Size MemoryManager::GB = 1024 * MB;
MemoryManager* MemoryManager::instance;
static MemoryManager g_instance(2 * MemoryManager::GB,
	1 * MemoryManager::GB,
	8 * MemoryManager::BYTE);
MemoryManager::MemoryManager(Size mem_size,
	Size stack_size, 
	Size pool_chunk_size) :
	m_stackAlloc(mem_size),
	m_poolAlloc(),
	m_persistentAlloc(),
	m_total_size(mem_size)
{
	m_persistentAlloc.Init(stack_size,m_stackAlloc.Alloc(stack_size, 8));
	m_poolAlloc.Init(pool_chunk_size,
		m_stackAlloc.GetRemainingSize() / pool_chunk_size,
		m_stackAlloc.GetRemainingSize() / pool_chunk_size,
		m_stackAlloc.Alloc(m_stackAlloc.GetRemainingSize(), 8));

	ENGINE_ASSERT_MSG(!instance, "MemoryManager instance already exists");
	instance = this;
}

MemoryManager::~MemoryManager()
{
}

void MemoryManager::Clear()
{
}

MemoryManager* MemoryManager::GetInstance()
{
	ENGINE_ASSERT_MSG(instance, "MemoryManager instance is not created");
	return instance;
}

