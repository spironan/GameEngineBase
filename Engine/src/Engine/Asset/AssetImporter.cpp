/************************************************************************************//*!
\file          AssetImporter.cpp
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          September 6, 2021
\brief         This file contains implementation of different asset serializers.
				This allows us to use templated functions to find out Asset types
				and provide the importer for that particular asset.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"

#include<memory>

#include "AssetImporter.h"
#include "AssetsManager.h"
#include "AssetSerializer.h"

namespace engine
{

std::unordered_map<AssetType, std::unique_ptr<IAssetSerializer>> AssetImporter::m_serializers;

void AssetImporter::Init()
{
	//these will delete themselves at the end of the program.
	m_serializers[AssetType::Texture] = std::make_unique<TextureSerializer>();
	//m_serializers[AssetType::RenderTarget] = TextureSerializer();
}


void AssetImporter::Serialize(const AssetMetadata& metadata, const std::shared_ptr<Asset>& asset)
{
	ENGINE_ASSERT_MSG(!m_serializers.empty(), "AsseterImporter::Init not called! AssetManager never Initialized");
	if (m_serializers.find(metadata.Type) == m_serializers.end())
	{
		LOG_ENGINE_WARN("Serializer of type {0} not implemented!", metadata.FilePath);
		return;
	}
	m_serializers[asset->GetAssetType()]->Serialize(metadata, asset);
}

void AssetImporter::Serialize(const std::shared_ptr<Asset>& asset)
{
	ENGINE_ASSERT_MSG(!m_serializers.empty(), "AsseterImporter::Init not called! AssetManager never Initialized");
	const AssetMetadata& metadata = AssetManager::GetMetadata(asset->GetHandle());
	m_serializers[asset->GetAssetType()]->Serialize(metadata, asset);
}

bool AssetImporter::TryLoadData(const AssetMetadata& metadata, std::shared_ptr<Asset>& asset)
{
	ENGINE_ASSERT_MSG(!m_serializers.empty(), "AsseterImporter::Init not called! AssetManager never Initialized");
	if (m_serializers.find(metadata.Type) == m_serializers.end())
	{
		LOG_ENGINE_WARN("Serializer of type {0} not implemented!", metadata.FilePath);
		return false;
	}

	return m_serializers[metadata.Type]->TryLoadData(metadata, asset);
}

}