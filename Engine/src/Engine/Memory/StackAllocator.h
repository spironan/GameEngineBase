#pragma once
#include <cstddef>
#include <cstdint>

namespace engine
{

	class StackAllocator {
	public:
		using Size = std::size_t;
		using Marker = std::size_t;
		using U8 = uint8_t;
		using PtrInt = uintptr_t;
		using PtrDiff = std::ptrdiff_t;


		StackAllocator() = delete;
		explicit StackAllocator(const Size stackSize);
		//void Init(Size stackSize, void* starting_address);
		~StackAllocator();

		/**
			* \brief Grab properly aligned memory from the stack allocator. You probably
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
			* \brief Create a new object on the stack allocator. The constructor is
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
			* \brief Free the stack allocator to a specific marker
			* \param marker Marker to free to
			*/
		void FreeToMarker(const Marker marker) { m_top = marker; }

		/**
			* \brief Clear the whole stack allocator to its bottom. All memory will be
			* available for new allocations again
			*/
		void Clear() { m_top = 0; }

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
	};

	template <typename T, typename... args>
	T* StackAllocator::New(args... argList) {
		void* mem = Alloc(sizeof(T), StackAllocator::ALIGNMENT);
		//std::cout << "marker:" << GetMarker() << "\n";
		return new (mem) T(argList...);
	}

	template <typename T>
	T* StackAllocator::NewArr(Size length, const U8 alignment) {
		void* alloc = Alloc(sizeof(T) * length, alignment);
		char* allocAddress = static_cast<char*>(alloc);
		for (int i = 0; i < length; ++i) new (allocAddress + i * sizeof(T)) T;
		return static_cast<T*>(alloc);
	}

}