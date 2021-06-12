#include "pch.h"
#include "StackAllocator.h"
#include "Engine/Core/Base.h"

namespace engine
{

	StackAllocator::StackAllocator(const Size stackSize)
		: m_top(0), m_totalSize(stackSize) {
		m_bottom = std::malloc(stackSize);
		if (!m_bottom) throw std::bad_alloc{};
		m_bottomAddress = reinterpret_cast<PtrInt>(m_bottom);
		//std::cout << "marker:" << GetMarker() << "\n";
	}

	//void StackAllocator::Init(Size stackSize, void* starting_address)
	//{
	//	m_totalSize = stackSize;
	//	m_bottom = starting_address;
	//	m_bottomAddress = reinterpret_cast<PtrInt>(m_bottom);
	//}


	StackAllocator::~StackAllocator() {
		std::free(m_bottom);
	}

	void* StackAllocator::Alloc(const Size size, const U8 alignment) {
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
			throw std::overflow_error{ "StackAllocator::Alloc => Not enough memory" };
		}

		m_top = newTop;
		return reinterpret_cast<void*>(alignedAddress);
	}

	StackAllocator::Size StackAllocator::GetRemainingSize()
	{
		return m_totalSize - m_top;
	}

}