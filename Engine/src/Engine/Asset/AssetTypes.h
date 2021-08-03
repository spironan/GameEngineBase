/************************************************************************************//*!
\file          AssetTypes.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 1, 2021
\brief         File contains type specifics for engine assets 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
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