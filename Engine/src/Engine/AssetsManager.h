#pragma once
#include <map>
#include <functional>
#include "Utility/Hash.h"
namespace engine
{

class AssetsManager
{
public:
	using ooTexID = uint64_t;
	using ooTexKey = utility::StringHash::size_type;

	static void SubscribeToTexture(void* userAddress,std::function<void(ooTexID)> updateFunction, ooTexKey texKey);
	static void RemoveSubscriber(void* userAddress);
	static void UpdateSubscriber(void* userAddress,const ooTexKey key);
	static ooTexKey AddTexture(const std::string& path);
	static void RemoveTexture(ooTexKey texID);
	static void UpdateTexture(const std::string& path);


public:
	struct TextureObject
	{
		std::string name;
		ooTexID textureID;
		float width, height;
	};
	struct TextureSubscriber
	{
		void* userAddress;
		std::function<void(ooTexID)> function;
		ooTexKey id;
	};
private:
	static std::map<ooTexKey, TextureObject> s_textures;
	static std::vector< TextureSubscriber > s_textureSubscribers;
};

}


