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

#include <memory>
#include "Engine/Core/Assert.h"


namespace engine
{
	using ooRendererID = uint32_t;


	enum class AssetType : uint16_t
	{
		None = 0,
		Texture = 1,
		RenderTarget = 2,
	};

	namespace utility
	{

		inline AssetType AssetTypeFromString(const std::string& assetType)
		{
			if (assetType == "None")        return AssetType::None;
			if (assetType == "Texture")     return AssetType::Texture;
			if (assetType == "RenderTarget")    return AssetType::RenderTarget;

			ENGINE_ASSERT_MSG(false, "Unknown Asset Type");
			return AssetType::None;
		}

		inline const char* AssetTypeToString(AssetType assetType)
		{
			switch (assetType)
			{
				case AssetType::None:        return "None";
				case AssetType::Texture:     return "Texture";
				case AssetType::RenderTarget:return "RenderTarget";
			}

			ENGINE_ASSERT_MSG(false, "Unknown Asset Type");
			return "None";
		}

	}
	
}