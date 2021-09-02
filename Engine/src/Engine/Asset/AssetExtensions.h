#pragma once
#include <unordered_map>

#include "Engine/Asset/AssetTypes.h"

namespace engine{


	inline static std::unordered_map<std::string, AssetType> s_assetExtensionMap =
	{
		{ ".png", AssetType::Texture },
		{ ".jpg", AssetType::Texture },
		{ ".jpeg", AssetType::Texture }
	};
}
