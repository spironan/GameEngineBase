#pragma once
namespace engine
{
	using ooTexID = uint32_t;

	/**
	 * Data structure for handling Texture assets.
	*/
	struct Texture //: public Asset
	{
		ooTexID id{};
		int32_t width{}, height{};
		int32_t bitDepth{};
		std::string name{};
	};
}