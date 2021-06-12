/*****************************************************************//**
\file   MemoryManager.h
\author Lim Guan Hui, l.guanhui , 2000552
\email  l.guanhui@digipen.edu
\date   7/6/2021
\brief  
This file contains a memory manager used to handle allocation and 
deallocation of memory. 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include <cstddef>
#include <utility>
#include "StackAllocator.h"
#include "PoolAllocator.h"
#include "BufferAllocator.h"

namespace engine
{

	class MemoryManager
	{
		using SA = StackAllocator;
		using PA = PoolAllocator;
		using Size = std::size_t;
		using U8 = uint8_t;
	public:
		static const Size BYTE;
		static const Size KB;
		static const Size MB;
		static const Size GB;
	public:
		MemoryManager();
		~MemoryManager();

		//available functions
		/*****************************************************************//**
		 * @brief Use this to request memory that is meant to be used throughout
		 * the duration of the program. Memory is automatically deallocated
		 * upon program termination.
		 *
		 * @param ...argList
		 * arguements to be used to initialise the 'type'
		 * this template function is specialised for. It runs the constructor
		 * with the arguments specified
		 *
		 * @return
		 * pointer to the object created of templated 'type'
		*********************************************************************/
		template <typename type, typename... args>
		static type* NewOnStack(args&&...);

		/*****************************************************************//**
		 * @brief Creates a new BufferAllocator object for use.
		 * 
		 * @param size maximum size of memory usable by this BufferAllocator
		 * @param alignment memory alignment used for this BufferAllocator
		 * @return BufferAllocator object created
		*********************************************************************/
		static BufferAllocator NewBufferAllocator(Size size, U8 const alignment = 16);


		//not to be used yet
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
		SA m_persistentAllocator;
		//PA m_poolAlloc;
		std::size_t m_total_size;

	};


	/*************************************************
	* Definitions
	**************************************************/
	template <typename type, typename... args>
	type* MemoryManager::NewOnStack(args&&... arguementList)
	{
		return GetInstance()->m_persistentAllocator.New<type>(
			std::forward<args>(arguementList)...);
	}
	/**
	 * not to be used for now.
	 */
	template<typename type, typename... args>
	type* MemoryManager::Allocate(args... arguementList)
	{
		return m_poolAlloc.Get();
	}
	/**
	 * not to be used for now.
	 */
	template<typename type>
	type* MemoryManager::AllocateArray(std::size_t num)
	{
		return NULL;
	}
	/**
	 * not to be used for now.
	 */
	template<typename type, typename... args>
	type* MemoryManager::AllocatePersistent(args... argList)
	{
		return m_persistentAlloc.New<type>(argList);
	}

}
