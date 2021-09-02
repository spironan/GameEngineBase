#pragma once
#include "Engine/Asset/Asset.h"

namespace engine
{

class OpenGLTexture2D : public Texture2D{

	OpenGLTexture2D(ImageFormat format, uint32_t width, uint32_t height, const void* data, TextureProperties properties);
	OpenGLTexture2D(const std::string& path, TextureProperties properties);
	virtual ~OpenGLTexture2D();

	virtual void Bind(uint32_t slot = 0) const;

	virtual void Resize(uint32_t width, uint32_t height) override {};

	virtual std::shared_ptr<Image2D> GetImage() { return m_image; };
	virtual void* Get_IMTEXTURE_ID() const override { return reinterpret_cast<void*>(&m_image->GetID()); }

	virtual ImageFormat GetFormat() const override { return m_image->GetSpecification().Format; }
	virtual uint32_t GetWidth() const override { return m_width; }
	virtual uint32_t GetHeight() const override { return m_height; }

	virtual bool Loaded() const override { return m_loaded; };

	virtual const std::string& GetPath() const = 0;

	virtual TextureType GetType() const override { return TextureType::Texture2D; }

	static AssetType GetStaticType() { return AssetType::Texture; }
	virtual AssetType GetAssetType() const override { return GetStaticType(); }
private:
	std::shared_ptr<Image2D> m_image;
	TextureProperties m_properties;
	uint32_t m_width, m_height;

	bool m_loaded = false;

	std::string m_filePath;
};
}
