/*****************************************************************//**
\file   BufferAllocator.h
\author Lim Guan Hui, l.guanhui , 2000552
\email  l.guanhui@digipen.edu
\date   12/6/2021
\brief  
This file contains a single buffered allocator

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include <cstddef>
#include <cstdint>

namespace engine
{

	class BufferAllocator {
	public:
		using Size = std::size_t;
		using Marker = std::size_t;
		using U8 = uint8_t;
		using PtrInt = uintptr_t;
		using PtrDiff = std::ptrdiff_t;


		BufferAllocator() = delete;
		~BufferAllocator();
		BufferAllocator(const BufferAllocator&) = default;

		/**
			* \brief Grab properly aligned memory from the buffer allocator. You probably
			* want to `GetMarker()` before calling this if you want to free to that
			* marker
			*
			* \param size Number of bytes you want, in byte
			* \param alignment Alignment requirement of the memory, must be power of 2
			* and less than 128 \return pointer to the allocated memory
			* \return Pointer to allocated memory
			*/
		void* Alloc(Size size, U8 alignment);

		/**
			* \brief Create a new object on the buffer allocator. The constructor is
			* automatically called. The memory is 16 aligned by default. If you are using
			* this, you probably need to call the destructor on your own.
			* \tparam T type of object you want to create
			* \tparam args Arguments for the constructor
			* \return Pointer to new object
			*/
		template <typename T, typename... args>
		T* New(args...);

		template <typename T>
		T* NewArr(Size length, U8 alignment);

		/**
			* \brief Free the buffer allocator to a specific marker
			* \param marker Marker to free to
			*/
		void FreeToMarker(const Marker marker) { m_top = marker; }

		/**
			* \brief Clear the whole buffer allocator to its bottom. All memory will be
			* available for new allocations again
			*/
		void Clear() 
		{
			m_top = 0;
			std::memset(m_bottom, 0, m_totalSize);
		}

		/**
			* \brief Get the current marker position
			* \return
			*/
		Marker GetMarker() const { return m_top; };
		Size GetRemainingSize();

		static const U8 ALIGNMENT = 16;
	private:
		Marker m_top;
		Size m_totalSize;
		void* m_bottom;
		PtrInt m_bottomAddress;

		explicit BufferAllocator(const Size stackSize, void* data);


		friend class MemoryManager;
	};

	template <typename T, typename... args>
	T* BufferAllocator::New(args... argList) {
		void* mem = Alloc(sizeof(T), BufferAllocator::ALIGNMENT);
		if (!mem) return nullptr;
		return new (mem) T(argList...);
	}

	template <typename T>
	T* BufferAllocator::NewArr(Size length, const U8 alignment = BufferAllocator::ALIGNMENT) {
		void* alloc = Alloc(sizeof(T) * length, alignment);
		if (!alloc) return nullptr;
		char* allocAddress = static_cast<char*>(alloc);
		for (int i = 0; i < length; ++i) new (allocAddress + i * sizeof(T)) T;
		return static_cast<T*>(alloc);
	}

}