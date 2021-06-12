#include "pch.h"
#include "BufferAllocator.h"
#include "Engine/Core/Base.h"

namespace engine
{

	BufferAllocator::BufferAllocator(const Size stackSize, void* data)
		: m_top(0), m_totalSize(stackSize) {
		m_bottom = data;
		if (!m_bottom) throw std::bad_alloc{};
		m_bottomAddress = reinterpret_cast<PtrInt>(m_bottom);
	}

	BufferAllocator::~BufferAllocator() {
	}

	void* BufferAllocator::Alloc(const Size size, const U8 alignment) {
		const bool valid = alignment >= 8 && alignment <= 128 &&
			(alignment & (alignment - 1)) == 0;  // power of 2
		if (valid == false)
			throw std::invalid_argument{ "alignment not satisfied" };

		PtrInt startingAddress = m_bottomAddress + m_top;
		PtrInt misAlignment = startingAddress & (alignment - 1);
		PtrDiff adjustment = alignment - misAlignment;
		// if misAlignment = 0 don't shift the address by its alignment
		adjustment = adjustment & (alignment - 1);
		PtrInt alignedAddress = startingAddress + adjustment;

		Marker newTop = m_top + size + adjustment;
		if (newTop > m_totalSize) { //if exceeded total memory available
			throw std::overflow_error{ "BufferAllocator::Alloc => Not enough memory" };
		}

		m_top = newTop;
		return reinterpret_cast<void*>(alignedAddress);
	}

	BufferAllocator::Size BufferAllocator::GetRemainingSize()
	{
		return m_totalSize - m_top;
	}

}