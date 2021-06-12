#include "pch.h"
#include "Engine/Core/Assert.h"
#include "MemoryManager.h"

namespace engine
{
	const MemoryManager::Size MemoryManager::BYTE = 1;
	const MemoryManager::Size MemoryManager::KB = 1024 * BYTE;
	const MemoryManager::Size MemoryManager::MB = 1024 * KB;
	const MemoryManager::Size MemoryManager::GB = 1024 * MB;

	MemoryManager* MemoryManager::instance;
	static MemoryManager g_instance;

	MemoryManager::MemoryManager() :
		m_persistentAllocator(2 * MemoryManager::GB),
		//m_poolAlloc(),
		m_total_size(2 * MemoryManager::GB)
	{
		//ENGINE_ASSERT_MSG(instance != nullptr, "MemoryManager instance already exists");
		if (instance != nullptr)
		{
			std::cout << "MemoryManager instance already exists" << std::endl;
			throw std::bad_alloc{};
			return;
		}

		instance = this;
		std::cout << "MemoryManager initialized" << std::endl;
	}

	MemoryManager::~MemoryManager()
	{
		std::cout << "MemoryManager destroyed" << std::endl;
	}

	void MemoryManager::Clear()
	{
	}

	MemoryManager* MemoryManager::GetInstance()
	{
		//ENGINE_ASSERT_MSG(instance, "MemoryManager instance is not created, please initialize it before use.");

		if (instance == nullptr)
		{
			std::cout << "MemoryManager instance is not created, please initialize it before use." << std::endl;
			throw std::bad_alloc{};
		}
		return instance;
	}

}