#pragma once

#include <memory>
#include <stdint.h>

#include "Engine/Memory/MemoryCommon.h"
#include "Engine/Core/Assert.h"
#include "Engine/Core/Log.h"

namespace engine
{

	struct Buffer
	{
		void* Data;
		uint32_t Size;

		Buffer()
			: Data(nullptr), Size(0)
		{
		}

		Buffer(void* data, uint32_t size)
			: Data(data), Size(size)
		{
		}

		static Buffer Copy(const void* data, uint32_t size)
		{
			Buffer buffer;
			buffer.Allocate(size);
			memcpy(buffer.Data, data, size);
			return buffer;
		}

		void Allocate(uint32_t size)
		{
			delete[] Data;
			Data = nullptr;

			if (size == 0)
				return;

			Data = new char[size];
			Size = size;
		}

		void Release()
		{
			delete[] Data;
			Data = nullptr;
			Size = 0;
		}

		void ZeroInitialize()
		{
			if (Data)
				memset(Data, 0, Size);
		}

		template<typename T>
		T& Read(uint32_t offset = 0)
		{
			return *(T*)((byte*)Data + offset);
		}

		char* ReadBytes(uint32_t size, uint32_t offset)
		{
			ENGINE_ASSERT_MSG((offset + size <= Size), "Buffer overflow!");
			char* buffer = OONEW char[size];
			memcpy(buffer, (char*)Data + offset, size);
			return buffer;
		}

		void Write(void* data, uint32_t size, uint32_t offset = 0)
		{
			ENGINE_ASSERT_MSG((offset + size <= Size), "Buffer overflow!");
			memcpy((char*)Data + offset, data, size);
		}

		operator bool() const
		{
			return Data;
		}

		char& operator[](int index)
		{
			return ((char*)Data)[index];
		}

		char operator[](int index) const
		{
			return ((char*)Data)[index];
		}

		template<typename T>
		T* As()
		{
			return (T*)Data;
		}

		inline uint32_t GetSize() const { return Size; }
	};

}

