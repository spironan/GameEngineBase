#pragma once

#include <memory>

#include "../vendor/glm/glm/glm.hpp"

#include "Utility/Hash.h"

#include "Engine/Asset/AssetTypes.h"
#include "Engine/Renderer/Image.h"

namespace engine
{

	class Framebuffer;

	enum class FramebufferBlendMode
	{
		None = 0,
		OneZero,
		SrcAlphaOneMinusSrcAlpha
	};


	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(ImageFormat format) : Format(format) {}

		ImageFormat Format;
		bool Blend = true;
		FramebufferBlendMode BlendMode = FramebufferBlendMode::SrcAlphaOneMinusSrcAlpha;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification>& attachments)
			: Attachments(attachments)
		{
		}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		float Scale = 1.0f;
		uint32_t Width = 0;
		uint32_t Height = 0;
		glm::vec4 ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		bool ClearOnLoad = true;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1; // multisampling

		// TODO: Temp, needs scale
		bool NoResize = false;

		// Master switch (individual attachments can be disabled in FramebufferTextureSpecification)
		bool Blend = true;
		// None means use BlendMode in FramebufferTextureSpecification
		FramebufferBlendMode BlendMode = FramebufferBlendMode::None;

		// SwapChainTarget = screen buffer (i.e. no framebuffer)
		bool SwapChainTarget = false;

		// Note: these are used to attach multi-layered depth images and color image arrays
		//Ref<Image2D> ExistingImage;
		std::vector<uint32_t> ExistingImageLayers;

		// Specify existing images to attach instead of creating
		// new images. attachment index -> image
		//std::map<uint32_t, Ref<Image2D>> ExistingImages;

		// At the moment this will just create a new render pass
		// with an existing framebuffer
		//Ref<Framebuffer> ExistingFramebuffer;

		std::string DebugName;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height, bool forceRecreate = false) = 0;
		//virtual void AddResizeCallback(const std::function<void(Ref<Framebuffer>)>& func) = 0;

		virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual ooRendererID GetRendererID() const = 0;

		virtual std::shared_ptr<Image2D> GetImage(uint32_t attachmentIndex = 0) const = 0;
		virtual std::shared_ptr<Image2D> GetDepthImage() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);
	};

	class FramebufferPool final
	{
	public:
		static void Add(const std::string& name, const std::shared_ptr<Framebuffer>& framebuffer) {	m_nameToFrameBuffer[utility::StringHash(name)] = framebuffer;
																								m_idToName[framebuffer->GetRendererID()] = name;
																							}

		static std::shared_ptr<Framebuffer> Get(utility::StringHash name) { return m_nameToFrameBuffer[name]; }
		static std::string GetName(std::shared_ptr<Framebuffer> fb) { return m_idToName[fb->GetRendererID()]; }

		static std::unordered_map<int32_t, std::shared_ptr<Framebuffer>>& GetAll() { return m_nameToFrameBuffer; }
		//static const std::unordered_map<int32_t, std::shared_ptr<Framebuffer>>& GetAll() const { return m_nameToFrameBuffer; }

	private:
		static std::unordered_map<int32_t, std::shared_ptr<Framebuffer>> m_nameToFrameBuffer;
		static std::unordered_map<ooRendererID, std::string> m_idToName;
	};

}
