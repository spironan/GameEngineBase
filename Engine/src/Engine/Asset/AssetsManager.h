/************************************************************************************//*!
\file          AssetsManager.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 1, 2021
\brief         This file contains API of an Asset manager for loading engine assets 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once
#include <map>
#include <functional>
#include <filesystem>

#include "AssetTypes.h"
#include "Asset.h"
#include "Engine/Asset/AssetExtensions.h"
#include "Engine/Asset/AssetMetadata.h"
#include "Engine/Asset/AssetRegistry.h"
#include "Engine/Asset/AssetSerializer.h"
#include "Utility/Hash.h"
#include "TextureLoader.h"
namespace engine
{
	class AssetManager
	{
	public:

		static AssetHandle ImportAsset(const std::string& filepath);

		//static bool ReloadData(AssetHandle assetHandle);

		static AssetMetadata& GetMetadata(AssetHandle handle);

		template<typename T>
		static std::shared_ptr<T> GetAsset(AssetHandle assetHandle)
		{
			auto& metadata = GetMetadata(assetHandle);

			std::shared_ptr<Asset> asset = nullptr;
			if (!metadata.IsDataLoaded)
			{
				auto ts = TextureSerializer(); //temporary
				metadata.IsDataLoaded = ts.TryLoadData(metadata, asset);
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
			return GetAsset<T>(m_assetRegistry.GetNamedHandle(utility::StringHash(name)));
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
					all.push_back(iter.second);
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
		static AssetRegistry m_assetRegistry;
		static AssetMetadata m_nullMetadata;
	};

	class TextureFileWatcher
	{
		// TODO: check for file changes and re-load the files
	};
	
	//class TextureDatabase
	//{
	//public:
	//	/*********************************************************************************//*! 
	//	 * \brief Registers a texture asset to the database.
	//	 * 
	//	 * \param name Name of the texture
	//	 * \param tex Texture
	//	*//**********************************************************************************/
	//	static void AddTexture(const std::string& name, Texture tex)
	//	{
	//		m_nameToTexture[engine::utility::StringHash(name)] = tex;
	//		m_idToName[tex.id] = name;
	//	}

	//	/*********************************************************************************//*!
	//	\brief    Registers a raw texture ID to the database
	//	 
	//	\param    name Name of the texture
	//	\param    id raw id of texture
	//	*//**********************************************************************************/
	//	static void AddTexture(const std::string& name, ooRendererID id)
	//	{
	//		Texture tex{};
	//		tex.id = id;
	//		tex.name = name;
	//		m_nameToTexture[engine::utility::StringHash(name)] = tex;
	//		m_idToName[tex.id] = name;
	//	}

	//	/*********************************************************************************//*!
	//	\brief    Updates the manager with a new texture of an existing entry
	//	 
	//	\param    name Existing entry
	//	\param    id new ID
	//	*//**********************************************************************************/
	//	static void UpdateTexture(engine::utility::StringHash name, ooRendererID id) { m_nameToTexture[name].id = id; }

	//	/*********************************************************************************//*!
	//	\brief    De-registers a texture from the manager
	//	 
	//	\param    name Name of the texture to de-register
	//	*//**********************************************************************************/
	//	static void RemoveTexture(engine::utility::StringHash name) { m_nameToTexture[name].id = 0; }

	//	/*********************************************************************************//*!
	//	\brief    Gets the Texture asset stored using a name
	//	 
	//	\param    name Name of the texture
	//	\return   Texture asset
	//	*//**********************************************************************************/
	//	static Texture GetTexture(engine::utility::StringHash name) { return m_nameToTexture[name]; }

	//	/*********************************************************************************//*!
	//	\brief    Gets a texture asset's name using its texture ID
	//	 
	//	\param    id texture ID
	//	\return   name of the texture
	//	*//**********************************************************************************/
	//	static const std::string& GetName(ooRendererID id) { return m_idToName[id]; }

	//	// TODO:
	//	static std::vector<std::pair<Texture, std::string>> GetAllTextures()
	//	{ 
	//		std::vector<std::pair<Texture, std::string>> vec;
	//		for (auto& iter : m_nameToTexture)
	//		{
	//			vec.emplace_back(std::make_pair(iter.second, m_idToName[iter.second.id]));
	//		}
	//		return vec;
	//	};

	//private:
	//	static std::unordered_map<int32_t, Texture> m_nameToTexture;
	//	static std::unordered_map<ooRendererID, std::string> m_idToName;
	//};

//class AssetsManager
//{
//public:
//	using ooTexID = uint64_t;
//	using ooTexKey = utility::StringHash::size_type;
//
//	static void SubscribeToTexture(void* userAddress,std::function<void(ooTexID)> updateFunction, ooTexKey texKey);
//	static void RemoveSubscriber(void* userAddress);
//	static void UpdateSubscriber(void* userAddress,const ooTexKey key);
//	static ooTexKey AddTexture(const std::string& path);
//	static void RemoveTexture(ooTexKey texID);
//	static void UpdateTexture(const std::string& path);
//
//
//public:
//	struct TextureObject
//	{
//		std::string name;
//		ooTexID textureID;
//		float width, height;
//	};
//	struct TextureSubscriber
//	{
//		void* userAddress;
//		std::function<void(ooTexID)> function;
//		ooTexKey id;
//	};
//private:
//	static std::map<ooTexKey, TextureObject> s_textures;
//	static std::vector< TextureSubscriber > s_textureSubscribers;
//};

}


