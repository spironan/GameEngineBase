#pragma once

#include <memory> //shared_ptr

#include "Engine/Renderer/Framebuffer.h"

namespace engine
{

	struct RenderPassSpecification
	{
		std::shared_ptr<Framebuffer> TargetFramebuffer;
		std::string DebugName;
	};

	class RenderPass
	{
	public:
		virtual ~RenderPass() = default;

		virtual RenderPassSpecification& GetSpecification() = 0;
		virtual const RenderPassSpecification& GetSpecification() const = 0;

		static std::shared_ptr<RenderPass> Create(const RenderPassSpecification & spec);
	};
}

