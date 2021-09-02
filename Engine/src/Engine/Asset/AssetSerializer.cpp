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
