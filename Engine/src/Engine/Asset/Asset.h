/************************************************************************************//*!
\file          Asset.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          September 6, 2021
\brief         This file contains the Asset classes of various types as well as their derived types

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <string>
#include <string_view>

#include "Engine/Renderer/Image.h"

#include "AssetTypes.h"

namespace engine
{
	// Texture
	// Sound
	// Scene
    using AssetHandle = uint32_t;

	class Asset
	{
	public:

		virtual ~Asset(){}

        virtual std::string_view GetName() const
        {
            return m_assetName;
        }

        virtual std::string_view GetFileName() const
        {
            return m_fileName;
        }

        virtual AssetType GetAssetType() const { return AssetType::None; }

        virtual AssetHandle GetHandle() const { return m_handle; }
        virtual void SetHandle(AssetHandle handle) { m_handle = handle; }

	protected:
        AssetHandle m_handle;
        AssetType m_tag{};
		std::string m_assetName;
		std::string m_fileName;
        //UIID

	};

    class Texture : public Asset
    {
    public:
        virtual ~Texture() {}

        virtual void Bind(uint32_t slot = 0) const = 0;

        virtual ImageFormat GetFormat() const = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetMipLevelCount() const = 0;
        virtual std::pair<uint32_t, uint32_t> GetMipSize(uint32_t mip) const = 0;

        virtual bool Loaded() const = 0;

        virtual ooRendererID& GetID() const = 0;
        virtual void* Get_IMTEXTURE_ID() const = 0;

        virtual TextureType GetType() const = 0;

        static AssetType GetStaticType() { return AssetType::Texture; }
        virtual AssetType GetAssetType() const override { return GetStaticType(); }
    };

    /**
     * Data structure for handling Texture assets.
    */
    class Texture2D : public Texture
    {
    public:
        virtual ~Texture2D()override{}

        static std::shared_ptr<Texture2D>Create(ImageFormat format, uint32_t width, uint32_t height, const void* data = nullptr, TextureProperties properties = TextureProperties());
        static std::shared_ptr<Texture2D>Create(const std::string& path, TextureProperties properties = TextureProperties());
        
        virtual std::shared_ptr<Image2D> GetImage() const = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;

        static AssetType GetStaticType() { return AssetType::Texture; }
        virtual AssetType GetAssetType() const override { return GetStaticType(); }
    };


}



/*

{
    "Files": {
        //TextureAssetManager
        "Textures" : [
            { "hud" : ".."}
            { "level1" : "../level1_textures.json"},
            { "level2" : "../level2_textures.json"}
        ]
        //AudioAssetManager
        "Audio" : "../audio.json"
    }
}

//textures.json
{
    Textures: [
        {
            "id": "ogre_tex_d",
            "filepath" : "...",
            "format" : "RGBA8",
            "tag" : "env_map"
        }
    ]
}

"../ogre.png"

??
ogre?
ogre.png?



std::shared_ptr<Texture>() ogre;
ogre.getID();

// Maintains referential integrity. (e.g deleting a single 3DModel should never delete a texture which might be an atlas)

//in gameplay code
auto &ogreTexDiffuse = TextureAssetManager::GetTexture("ogreTex_diffuse");

// Asset
//  tag : sound, texture, evironment, mesh
//  file name
//  item name
//  UUID?
//       Texture
//           image dimensions
//           image format
//           image slicing??

Class Texture : public Asset;

AssetID Asset::GetID();

GetAsset().As<OpenGLTexture>();

template<typename T2>
std::shared_ptr<T2> As() const
{
    return std::shared_ptr<T2>(*this);
}

std::shared_ptr<Texture> ogreTexDiffuse = AssetManager::GetAsset<Texture>("ogreTex_diffuse");

std::map< hash , std::sharedptr<Asset> >


init{
engine::Texture tex = engine::TextureLoader::LoadFromFilePath("../Engine/assets/images/ogre.png");
}
engine::TextureDatabase::AddTexture("ogre", tex);

//level1_textures.json

assets/assets_metadata.json

assets/textures.json
assets/sprites.json

*/
