#pragma once
#include <map>
#include <functional>
#include "AssetTypes.h"
#include "Utility/Hash.h"
#include "TextureLoader.h"
namespace engine
{
	class TextureFileWatcher
	{
		// TODO: check for file changes and re-load the files
	};
	
	class TextureDatabase
	{
	public:
		/*********************************************************************************//*! 
		 * \brief Registers a texture asset to the database.
		 * 
		 * \param name Name of the texture
		 * \param tex Texture
		*//**********************************************************************************/
		static void AddTexture(const std::string& name, Texture tex)
		{
			m_nameToTexture[engine::utility::StringHash(name)] = tex;
			m_idToName[tex.id] = name;
		}

		static void AddTexture(const std::string& name, ooTexID id)
		{
			Texture tex{};
			tex.id = id;
			tex.name = name;
			m_nameToTexture[engine::utility::StringHash(name)] = tex;
			m_idToName[tex.id] = name;
		}

		static void UpdateTexture(engine::utility::StringHash name, ooTexID id) { m_nameToTexture[name].id = id; }

		static void RemoveTexture(engine::utility::StringHash name) { m_nameToTexture[name].id = 0; }
		static Texture GetTexture(engine::utility::StringHash name) { return m_nameToTexture[name]; }
		static const std::string& GetName(ooTexID id) { return m_idToName[id]; }

		// TODO:
		static std::vector<std::pair<Texture, std::string>> GetAllTextures()
		{ 
			std::vector<std::pair<Texture, std::string>> vec;
			for (auto& iter : m_nameToTexture)
			{
				vec.emplace_back(std::make_pair(iter.second, m_idToName[iter.second.id]));
			}
			return vec;
		};

	private:
		static std::unordered_map<int32_t, Texture> m_nameToTexture;
		static std::unordered_map<ooTexID, std::string> m_idToName;
	};

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


