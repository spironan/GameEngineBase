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
