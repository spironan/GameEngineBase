/************************************************************************************//*!
\file          AssetSerializer.cpp
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          September 6, 2021
\brief         This file contains concrete implementations of the IAssetSerializers. 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "AssetSerializer.h"

bool engine::TextureSerializer::TryLoadData(const AssetMetadata& metadata, std::shared_ptr<Asset>& asset) const
{
	asset = Texture2D::Create(metadata.FilePath);
	asset->SetHandle(metadata.Handle);

	bool result = std::dynamic_pointer_cast<Texture2D>(asset)->Loaded();

	//if (!result)
	//	asset->SetFlag(AssetFlag::Invalid, true);

	return result;
}
