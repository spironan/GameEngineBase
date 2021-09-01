/************************************************************************************//*!
\file          Renderer2D.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 1, 2021
\brief         File contains API of a 2D renderer.
				Commands to the renderer are performed by first 
				1. BeginScene
				2. Draw commands
				3. Render

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "Engine/Asset/AssetTypes.h"
#include "Engine/Renderer/DebugCamera.h"
#include "Engine/Renderer/Camera.h"


namespace engine
{

	class Renderer2D
	{
	public:

		/*********************************************************************************//*!
		\brief    Initializes the host and device data required for the rendering system
		 
		*//**********************************************************************************/
		static void Init();

		/*********************************************************************************//*!
		\brief    Releases the host and device data required for the rendering system

		*//**********************************************************************************/
		static void Shutdown();

		/*********************************************************************************//*!
		\brief    Begins the rendering batch using a projection and viewpoint
		 
		\param    viewProj viewprojection of the scene
		\param    view view of the scene
		*//**********************************************************************************/
		static void BeginScene(const oom::mat4& viewProj, const oom::mat4& view);
		//static void BeginScene(const EditorCamera& camera);

		/*********************************************************************************//*!
		\brief    Ends the scene and submits the call to the device
		 
		*//**********************************************************************************/
		static void EndScene();

		// Primitives

		static void DrawQuad(const oom::vec2& position, const oom::vec2& size, const oom::vec4& color);
		static void DrawQuad(const oom::vec3& position, const oom::vec2& size, const oom::vec4& color);
		//static void DrawQuad(const oom::vec2& position, const oom::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const oom::vec4& tintColor = oom::vec4(1.0f));
		//static void DrawQuad(const oom::vec3& position, const oom::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const oom::vec4& tintColor = oom::vec4(1.0f));

		//static void DrawQuad(const oom::mat4& transform, const oom::vec4& color, int entityID = -1);
		//static void DrawQuad(const oom::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const oom::vec4& tintColor = oom::vec4(1.0f), int entityID = -1);

		static void DrawRotatedQuad(const oom::vec2& position, const oom::vec2& size, float rotation, const oom::vec4& color);
		static void DrawRotatedQuad(const oom::vec3& position, const oom::vec2& size, float rotation, const oom::vec4& color);
		static void DrawRotatedQuad(const oom::vec2& position, const oom::vec2& size, float rotation, const ooRendererID& texture, float tilingFactor = 1.0f, const oom::vec4& tintColor = oom::vec4(1.0f));
		static void DrawRotatedQuad(const oom::vec3& position, const oom::vec2& size, float rotation, const ooRendererID& texture, float tilingFactor = 1.0f, const oom::vec4& tintColor = oom::vec4(1.0f));

		static void DrawCircle(const oom::vec3& p0, float rotation, float radius, const oom::vec4& color);
		//static void FillCircle(const oom::vec2& p0, float radius, const oom::vec4& color, float thickness = 0.05f);
		//static void FillCircle(const oom::vec3& p0, float radius, const oom::vec4& color, float thickness = 0.05f);

		static void DrawLine(const oom::vec3& p0, const oom::vec3& p1, const oom::vec4& color = oom::vec4(1.0f));

		//static void DrawAABB(const AABB& aabb, const oom::mat4& transform, const oom::vec4& color = oom::vec4(1.0f));

		//static void DrawSprite(const oom::mat4& transform, SpriteRendererComponent& src, int entityID);

		// Stats
		struct BatchRenderStats
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};
		static void ResetStats();
		static BatchRenderStats GetStats();

	private:
		/*********************************************************************************//*!
		\brief    Helper function to flush the data in the quad buffer, drawing to render target
		 
		*//**********************************************************************************/
		static void Flush();
		/*********************************************************************************//*!
		\brief    Helper function to flush the data in the lines buffer, drawing to render target

		*//**********************************************************************************/
		static void FlushLines();

		/*********************************************************************************//*!
		\brief    Flushes the data in the buffer, drawing to render target. This resets the buffers

		*//**********************************************************************************/
		static void FlushAndReset();
		/*********************************************************************************//*!
		\brief    Flushes the data in the lines buffer, drawing to render target. This resets the buffers

		*//**********************************************************************************/
		static void FlushAndResetLines();
	};

}