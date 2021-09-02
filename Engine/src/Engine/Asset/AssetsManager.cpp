/************************************************************************************//*!
\file          AssetsManager.cpp
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 1, 2021
\brief         This file contains implementation of an Asset manager for loading engine assets


Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "AssetsManager.h"
#include <filesystem>

namespace engine
{
	//std::unordered_map<int32_t, Texture> TextureDatabase::m_nameToTexture;
	//std::unordered_map<ooRendererID, std::string> TextureDatabase::m_idToName; 

	std::unordered_map< utility::StringHash,std::shared_ptr<Asset> > AssetManager::m_loadedAssets;

	//void AssetsManager::SubscribeToTexture(void* objectpointer, std::function<void(ooTexID)> updateFunction, ooTexKey texKey)
	//{
	//	s_textureSubscribers.emplace_back(TextureSubscriber{ objectpointer,updateFunction,texKey });
	//}
	//void AssetsManager::RemoveSubscriber(void* address)
	//{
	//	for (size_t iter = 0; iter< s_textureSubscribers.size(); ++iter)
	//	{
	//		if (s_textureSubscribers[iter].userAddress == address)
	//		{
	//			std::swap(s_textureSubscribers[iter], s_textureSubscribers.back());
	//			s_textureSubscribers.pop_back();
	//			return;
	//		}
	//	}
	//}
	//void AssetsManager::UpdateSubscriber(void* userAddress,const ooTexKey key)
	//{
	//	auto& iter = s_textures.find(key);
	//	_ASSERT(iter != s_textures.end());

	//	for (auto& object : s_textureSubscribers)
	//	{
	//		if (object.userAddress == userAddress)
	//		{
	//			object.function(iter->second.textureID);
	//		}
	//	}
	//}
	//AssetsManager::ooTexKey AssetsManager::AddTexture(const std::string& str)
	//{
	//	ooTexKey key = utility::StringHash::GenerateFNV1aHash(str);

	//	TextureObject to;
	//	//some load texture code TODO: fill this up

	//	//add in width and height
	//	
	//	//process other info
	//	to.name = std::filesystem::path(str).stem().u8string();


	AssetHandle AssetManager::ImportAsset(const std::filesystem::path& filepath)
	{
		//std::filesystem::path path = std::filesystem::relative(filepath, Project::GetAssetDirectory());

		if(s_AssetRegistry.Contains(filepath))
			return s_AssetRegistry[filepath].Handle;

		AssetType type = GetAssetTypeFromPath(filepath);
		if (type == AssetType::None)
			return 0;

		return AssetHandle();
	}

	AssetHandle AssetManager::ImportAsset(const std::string& filepath)
	{
		auto hPath = utility::StringHash(filepath);
		if (m_loadedAssets.find(hPath) != m_loadedAssets.end())
		{
			return hPath;
		}

		AssetType type = GetAssetTypeFromPath(filepath);
		if (type == AssetType::None)
		{
			return 0;
		}

		AssetMetadata metadata;
		metadata.Handle = utility::StringHash(filepath);
		metadata.FilePath = filepath;
		metadata.Type = type;
		//s_AssetRegistry[metadata.FilePath] = metadata;

		return metadata.Handle;
	}



	AssetType AssetManager::GetAssetTypeFromPath(const std::filesystem::path& path)
	{
		return GetAssetTypeFromExtension(path.extension().string());
	}

	AssetType AssetManager::GetAssetTypeFromExtension(const std::string& extension)
	{
		std::string str = extension;
		std::transform(str.begin(), str.end(), str.begin(),
					   [](unsigned char c) { return std::tolower(c); });
		if (s_assetExtensionMap.find(str) == s_assetExtensionMap.end())
		{
			return AssetType::None;
		}

		return s_assetExtensionMap.at(str);

	}

	//	s_textures.emplace(key,to);
	//	return key;
	//}
	//void AssetsManager::RemoveTexture(ooTexKey texID)
	//{
	//	s_textures.erase(texID);
	//	//update objects linked to key
	//	for (auto& object : s_textureSubscribers)
	//	{
	//		if (texID == object.id)
	//			object.function(-1);//change to some invalid thing or placeholder
	//	}
	//}
	//void AssetsManager::UpdateTexture(const std::string& path)
	//{
	//	ooTexKey key = utility::StringHash::GenerateFNV1aHash(path);
	//	auto& iter = s_textures.find(key);
	//	if (iter == s_textures.end())//no exist cant update
	//		return;
	//	//reload texture TODO: fill this up
	//	//iter->second.textureID 
	//	
	//	//reassign texture id
	//	for (auto& object : s_textureSubscribers)
	//	{
	//		if (key == object.id)
	//			object.function(iter->second.textureID);//change to some invalid thing or placeholder
	//	}
	//}
}
