#pragma once

#include <memory>

#include "Engine/Core/Assert.h"

namespace engine
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float	:return sizeof(float);
			case ShaderDataType::Float2	:return sizeof(float) * 2;
			case ShaderDataType::Float3	:return sizeof(float) * 3;
			case ShaderDataType::Float4	:return sizeof(float) * 4;
			case ShaderDataType::Mat3	:return sizeof(float) * 3 * 3;
			case ShaderDataType::Mat4	:return sizeof(float) * 4 * 4;
			case ShaderDataType::Int	:return sizeof(int);
			case ShaderDataType::Int2	:return sizeof(int) * 2;
			case ShaderDataType::Int3	:return sizeof(int) * 3;
			case ShaderDataType::Int4	:return sizeof(int) * 4;
			case ShaderDataType::Bool	:return sizeof(bool);
		}

		ENGINE_ASSERT_MSG(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct VertexBufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		VertexBufferElement() = default;

		VertexBufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float		:return 1;
				case ShaderDataType::Float2		:return 2;
				case ShaderDataType::Float3		:return 3;
				case ShaderDataType::Float4		:return 4;
				case ShaderDataType::Mat3		:return 3 * 3;
				case ShaderDataType::Mat4		:return 4 * 4;
				case ShaderDataType::Int		:return 1;
				case ShaderDataType::Int2		:return 2;
				case ShaderDataType::Int3		:return 3;
				case ShaderDataType::Int4		:return 4;
				case ShaderDataType::Bool		:return 1;
			}

			ENGINE_ASSERT_MSG(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	enum class VertexBufferUsage
	{
		None = 0, Static = 1, Dynamic = 2
	};

class VertexBuffer
{


	virtual unsigned int GetSize() const = 0;
	//virtual RendererID GetRendererID() const = 0;

	static std::shared_ptr<VertexBuffer> Create(uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
};

}// end namespace engine
