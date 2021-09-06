/************************************************************************************//*!
\file          AssetMetadata.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          September 6, 2021
\brief         This file contains structure format for asset's Metadata. 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
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