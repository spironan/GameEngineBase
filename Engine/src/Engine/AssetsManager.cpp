#include "pch.h"
#include "AssetsManager.h"
#include <filesystem>
namespace engine
{

	void AssetsManager::SubscribeToTexture(void* objectpointer, std::function<void(ooTexID)> updateFunction, ooTexKey texKey)
	{
		s_textureSubscribers.emplace_back(TextureSubscriber{ objectpointer,updateFunction,texKey });
	}
	void AssetsManager::RemoveSubscriber(void* address)
	{
		for (size_t iter = 0; iter< s_textureSubscribers.size(); ++iter)
		{
			if (s_textureSubscribers[iter].userAddress == address)
			{
				std::swap(s_textureSubscribers[iter], s_textureSubscribers.back());
				s_textureSubscribers.pop_back();
				return;
			}
		}
	}
	void AssetsManager::UpdateSubscriber(void* userAddress,const ooTexKey key)
	{
		auto& iter = s_textures.find(key);
		_ASSERT(iter != s_textures.end());

		for (auto& object : s_textureSubscribers)
		{
			if (object.userAddress == userAddress)
			{
				object.function(iter->second.textureID);
			}
		}
	}
	AssetsManager::ooTexKey AssetsManager::AddTexture(const std::string& str)
	{
		ooTexKey key = utility::StringHash::GenerateFNV1aHash(str);

		TextureObject to;
		//some load texture code TODO: fill this up

		//add in width and height
		
		//process other info
		to.name = std::filesystem::path(str).stem().u8string();

		s_textures.emplace(key,to);
		return key;
	}
	void AssetsManager::RemoveTexture(ooTexKey texID)
	{
		s_textures.erase(texID);
		//update objects linked to key
		for (auto& object : s_textureSubscribers)
		{
			if (texID == object.id)
				object.function(-1);//change to some invalid thing or placeholder
		}
	}
	void AssetsManager::UpdateTexture(const std::string& path)
	{
		ooTexKey key = utility::StringHash::GenerateFNV1aHash(path);
		auto& iter = s_textures.find(key);
		if (iter == s_textures.end())//no exist cant update
			return;
		//reload texture TODO: fill this up
		//iter->second.textureID 
		
		//reassign texture id
		for (auto& object : s_textureSubscribers)
		{
			if (key == object.id)
				object.function(iter->second.textureID);//change to some invalid thing or placeholder
		}
	}
}