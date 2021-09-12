/************************************************************************************//*!
\file          AssetsManager.cpp
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 1, 2021
\brief         This file contains implementation of an Asset manager for loading engine allAssets


Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "AssetsManager.h"
#include <filesystem>

#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include "Utility/UniqueInstanceIDGenerator.h"

namespace engine
{
	//std::unordered_map<int32_t, Texture> TextureDatabase::m_nameToTexture;
	//std::unordered_map<ooRendererID, std::string> TextureDatabase::m_idToName; 

	std::unordered_map< utility::StringHash::size_type,std::shared_ptr<Asset> > AssetManager::m_loadedAssets;

	AssetRegistry AssetManager::m_registry;
	AssetMetadata AssetManager::m_nullMetadata{};

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


	void AssetManager::Init()
	{
		AssetImporter::Init();
		LoadAssetRegistry();
	}

	void AssetManager::Shutdown()
	{
		SerializeRegistry();
	}

	AssetHandle AssetManager::ImportAsset(const std::string& filepath)
	{
		auto hPath = utility::StringHash(filepath);
		if (m_registry.Contains(hPath))
		{
			return m_registry[hPath].Handle;
		}

		AssetType type = GetAssetTypeFromPath(filepath);
		if (type == AssetType::None)
		{
			return 0;
		}

		AssetMetadata metadata;
		metadata.Handle = hPath;
		metadata.FilePath = filepath;
		metadata.Type = type;
		m_registry[metadata.FilePath] = metadata;

		//store a named copy of the handle
		auto lastSlash = filepath.find_last_of("/") + 1;
		std::string assetName = filepath.substr(lastSlash, filepath.find_last_of(".") - lastSlash);
		m_registry.SetNamedHandle(assetName, metadata.Handle);

		return metadata.Handle;
	}

	void AssetManager::LoadAssetRegistry()
	{
		//TODO: Replace this with Project.h
		AssetManagerConfig registryDefaults;

		if (!std::filesystem::exists(registryDefaults.AssetRegistryPath.c_str()))
		{
			// Asset Registry does not exist. First time running program?
			return;
		}
		return;
		//LOG_ENGINE_INFO("project dir {0}", Project::GetProjectDirectory().string());

		//create dir
		//std::filesystem::path assetDir(registryDefaults.AssetDirectory);
		//if (!std::filesystem::exists(assetDir))
		//{
		//	std::filesystem::create_directories(assetDir);
		//}
		std::ifstream ifs;
		ifs.open(registryDefaults.AssetRegistryPath.c_str());
		if (!ifs.is_open())
		{
			LOG_CRITICAL("Asset Manager not able to open file");
			ENGINE_ASSERT(true);
		}
		rapidjson::IStreamWrapper isw(ifs);
		rapidjson::Document doc;
		doc.ParseStream(isw);
		//doc.Parse(registryDefaults.AssetRegistryPath.c_str());
		//ENGINE_ASSERT_MSG(!doc.IsObject(), "Error loading asset registry");
		auto x = doc.HasParseError();
		auto y = doc.GetParseError();
		ENGINE_ASSERT_MSG(!doc.HasParseError(), "Error reading asset registry, might be corrupted!");
		ENGINE_ASSERT_MSG(doc.HasMember("Assets"), "Error reading asset registry, might be corrupted!");
		ENGINE_ASSERT_MSG(doc["Assets"].IsArray(), "Error reading asset registry, might be corrupted!");
		auto& allAssets = doc["Assets"].GetArray();
		for (rapidjson::Value::ConstValueIterator assetsIter = allAssets.Begin(); assetsIter != allAssets.End(); ++assetsIter)
		{
			AssetMetadata  meta{};			
			for (rapidjson::Value::ConstMemberIterator handle = assetsIter->MemberBegin(); handle != assetsIter->MemberEnd(); ++handle)
			{
				meta.Handle = std::stoul(handle->name.GetString());
				auto& info = handle->value.GetArray();
				meta.Type = utility::AssetTypeFromString(info[0].GetString());
				meta.FilePath = info[1].GetString();				
			}
			m_registry[meta.Handle] = meta;

			//store a named copy of the handle
			auto lastSlash = meta.FilePath.find_last_of("/") + 1;
			std::string assetName = meta.FilePath.substr(lastSlash, meta.FilePath.find_last_of(".") - lastSlash);
			m_registry.SetNamedHandle(assetName, meta.Handle);

		}

		//for (rapidjson::SizeType assetsIndex = 0; assetsIndex < allAssets.Size(); ++assetsIndex)
		//{
		//	allAssets[assetsIndex];
		//	AssetMetadata  meta{};
		//	meta.Handle = std::stoul(allAssets[i].GetString());
		//	auto& asset = allAssets[i+1].GetArray();
		//	
		//	meta.Type = engine::utility::AssetTypeFromString(asset[0].GetString());
		//	meta.FilePath = asset[1].GetString();
		//	
		//	//LOG_ENGINE_INFO("Loading {0} Asset '{1}'", allAssets[i].GetString(), allAssets[i+1].GetString());
		//	m_registry[meta.Handle] = meta;
		//}
		// pick a path to store an asset registry. Kind of like a log book of all allAssets
		//check if exists

		//serialize the registry...

		//find each "node" of allAssets. This should be the metadata so we should have the filepath

			// If we cannot find filepath, most likely misplaced file. Loop through each token of their path to find best match

		// Report back any failed loading attempts

	}

	AssetMetadata& AssetManager::GetMetadata(AssetHandle handle)
	{
		for (auto& [filepath, metadata] : m_registry)
		{
			if (metadata.Handle == handle)
				return metadata;
		}

		return m_nullMetadata;
	}

	void AssetManager::SerializeRegistry()
	{
		// sort allAssets for file management. easier to search for arranged ID
		struct AssetRegistryEntry
		{
			std::string filePath;
			AssetType type;
		};
		std::map<AssetHandle, AssetRegistryEntry> sortedMap;
		for (auto& [filepath, metadata]: m_registry)
		{
			if (!std::filesystem::exists(metadata.FilePath))
				continue;

			sortedMap[metadata.Handle] = { metadata.FilePath.c_str(), metadata.Type };
		}

		//TODO : replace with Project.h
		AssetManagerConfig registryDefaults;

		std::ofstream stream(registryDefaults.AssetRegistryPath, std::ios::trunc);
		rapidjson::OStreamWrapper osw(stream);
		rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);

		writer.StartObject();
		writer.Key("Assets");
		writer.StartArray();
		for (auto& [handle, asset] : sortedMap)
		{
			writer.StartObject();
			writer.Key(std::to_string(handle).c_str());

			writer.StartArray();
			//setting the format here lets us have nice indentation
			//writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);

			writer.StartObject();
			writer.Key("Type");
			writer.String(utility::AssetTypeToString(asset.type)); // Type
			writer.EndObject();

			writer.StartObject();
			writer.Key("Filepath");
			writer.String(asset.filePath.c_str()); // path
			writer.EndObject();

			writer.EndArray();
			writer.EndObject();
			//writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);
		}
		writer.EndArray();
		writer.EndObject();

		ENGINE_ASSERT(writer.IsComplete());

		stream.close();
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
