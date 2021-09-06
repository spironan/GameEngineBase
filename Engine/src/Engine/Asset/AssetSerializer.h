/************************************************************************************//*!
\file          AssetSerializer.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          September 6, 2021
\brief         This file contains interface class of IAssetSerializer. This class will be extended to follow same format for
				loading and serialization of assets.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <memory>

#include "Asset.h"
#include "Engine/Asset/AssetMetadata.h"

namespace engine{

	class IAssetSerializer
	{
	public:
		virtual void Serialize(const AssetMetadata& metadata, const  std::shared_ptr<Asset>& asset) const = 0;
		virtual bool TryLoadData(const AssetMetadata& metadata, std::shared_ptr<Asset>& asset) const = 0;
	};

	class TextureSerializer : public IAssetSerializer
	{
	public:
		virtual void Serialize(const AssetMetadata& metadata, const std::shared_ptr<Asset>& asset) const override {}
		virtual bool TryLoadData(const AssetMetadata& metadata, std::shared_ptr<Asset>& asset) const override;
	};

}
