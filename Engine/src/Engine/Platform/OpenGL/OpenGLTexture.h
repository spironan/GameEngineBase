#pragma once
#include "Engine/Asset/Asset.h"

namespace engine
{

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(ImageFormat format, uint32_t width, uint32_t height, const void* data, TextureProperties properties);
	OpenGLTexture2D(const std::string& path, TextureProperties properties);
	virtual ~OpenGLTexture2D() override;

	virtual void Bind(uint32_t slot = 0) const override;

	virtual void Resize(uint32_t width, uint32_t height) override {};

	virtual std::shared_ptr<Image2D> GetImage() const { return m_image; };
	virtual void* Get_IMTEXTURE_ID() const override { return reinterpret_cast<void*>( static_cast<uint64_t>(m_image->GetID()) ); }

	virtual ImageFormat GetFormat() const override { return m_image->GetSpecification().Format; }
	virtual uint32_t GetWidth() const override { return m_width; }
	virtual uint32_t GetHeight() const override { return m_height; }
	virtual uint32_t GetMipLevelCount() const override { return m_image->GetSpecification().Mips; }
	virtual std::pair<uint32_t, uint32_t> GetMipSize(uint32_t mip) const override { return std::pair<uint32_t, uint32_t>(0, 0); }

	virtual bool Loaded() const override { return m_loaded; };

	virtual const std::string& GetPath() const { return m_filePath; };

	virtual TextureType GetType() const override { return TextureType::Texture2D; }

	virtual ooRendererID& GetID() const override { return m_image->GetID(); }
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
