#pragma once

#include "Engine/Asset/Asset.h"

namespace engine
{

	struct AssetMetadata
	{
		AssetHandle Handle = 0;
		AssetType Type;

		std::string FilePath; //can consider std::filesystem::path
		bool IsDataLoaded = false;

		bool IsValid() const { return Handle != 0; }
	};
}