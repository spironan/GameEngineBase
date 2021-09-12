/************************************************************************************//*!
\file          AssetsManager.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 1, 2021
\brief         This file contains API of an Asset manager for loading engine assets.
				This includes maintainence of metadata.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once
#include <map>
#include <functional>
#include <filesystem>

#include "Engine/Project/Project.h"
#include "AssetTypes.h"
#include "Asset.h"
#include "Engine/Asset/AssetExtensions.h"
#include "Engine/Asset/AssetMetadata.h"
#include "Engine/Asset/AssetRegistry.h"
#include "Engine/Asset/AssetSerializer.h"
#include "Engine/Asset/AssetImporter.h"
#include "Utility/Hash.h"

namespace engine
{
	struct AssetManagerConfig
	{
		std::string AssetDirectory = "Assets/";
		std::string AssetRegistryPath = "Assets/AssetRegistry.oor";

		std::string MeshPath = "Assets/Meshes/";
		std::string MeshSourcePath = "Assets/Meshes/Source/";
	};


	class AssetManager
	{
	public:

		static void Init();
		static void Shutdown();

		// Import an asset into the project. This creates an AssetMetadata of the asset.
		static AssetHandle ImportAsset(const std::string& filepath);

		// This loads the entire asset registry inside our project folder.
		// Information includes the existing metadata that was serialized previously
		// ALWAYS LOAD THE REGISTRY BEFORE PROCESSING DIRECTORY
		static void LoadAssetRegistry();
		static void SerializeRegistry();

		//static bool ReloadData(AssetHandle assetHandle);

		static AssetMetadata& GetMetadata(AssetHandle handle);

		template<typename T>
		static std::shared_ptr<T> GetAsset(AssetHandle assetHandle)
		{
			auto& metadata = GetMetadata(assetHandle);

			std::shared_ptr<Asset> asset = nullptr;
			if (!metadata.IsDataLoaded)
			{
				metadata.IsDataLoaded = AssetImporter::TryLoadData(metadata, asset);
				if (!metadata.IsDataLoaded)
				{
					return nullptr;
				}

				m_loadedAssets[assetHandle] = asset;
			}
			
			asset =	m_loadedAssets[assetHandle];

			return std::dynamic_pointer_cast<T>(asset);
		}

		template<typename T>
		static std::shared_ptr<T> GetAsset(utility::StringHash assetHandle)
		{
			return GetAsset<T>(AssetHandle(assetHandle));
		}

		template<typename T>
		static std::shared_ptr<T> GetNamedAsset(const std::string& name)
		{
			return GetAsset<T>(m_registry.GetNamedHandle(utility::StringHash(name)));
		}
		/*template<typename T>
		static std::shared_ptr<T> GetAsset(const std::string& filepath)
		{
			auto hPath = utility::StringHash(filepath);
			std::shared_ptr<Asset> asset = nullptr;
			if (m_loadedAssets.find(hPath) != m_loadedAssets.end())
			{
				return (asset = m_loadedAssets[hPath]);
			}

			return asset;
			return GetAsset<T>(GetAssetHandleFromFilePath(fp));
		}*/

		template<typename T>
		static std::vector<std::shared_ptr<T>> GetAll()
		{
			std::vector<std::shared_ptr<T>> all;
			for (auto& iter : m_loadedAssets)
			{
				if (iter.second->GetAssetType() == T::GetStaticType())
				{
					all.push_back(std::reinterpret_pointer_cast<T>(iter.second));
				}
			}
			return all;
		}

		static AssetType GetAssetTypeFromPath(const std::filesystem::path& path);
		static AssetType GetAssetTypeFromExtension(const std::string& extension);

		//static bool FileExists(AssetMetadata& metadata)
		//{
		//	return FileSystem::Exists(Project::GetActive()->GetAssetDirectory() / metadata.FilePath);
		//}
	private:
		static std::unordered_map<utility::StringHash::size_type, std::shared_ptr<Asset>> m_loadedAssets;
		static AssetRegistry m_registry;
		static AssetMetadata m_nullMetadata;
	};

	class TextureFileWatcher
	{
		// TODO: check for file changes and re-load the files
	};
	

}


