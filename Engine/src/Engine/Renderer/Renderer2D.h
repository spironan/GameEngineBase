/************************************************************************************//*!
\file          Renderer2D.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 1, 2021
\brief         File contains API of a 2D renderer

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "Engine/Asset/AssetTypes.h"
#include "Engine/Renderer/DebugCamera.h"
#include "Engine/Renderer/OrthographicCamera.h"


namespace engine
{

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		//static void BeginScene(const Camera& camera, const glm::mat4& transform);
		//static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const OrthographicCamera& camera); // TODO: Remove
		static void EndScene();
		static void Flush();
		static void FlushLines();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		//static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		//static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		//static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		//static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const ooTexID& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const ooTexID& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawCircle(const glm::vec3& p0, float rotation, float radius, const glm::vec4& color);
		//static void FillCircle(const glm::vec2& p0, float radius, const glm::vec4& color, float thickness = 0.05f);
		//static void FillCircle(const glm::vec3& p0, float radius, const glm::vec4& color, float thickness = 0.05f);

		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color = glm::vec4(1.0f));

		//static void DrawAABB(const AABB& aabb, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));

		//static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

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
		static void FlushAndReset();
		static void FlushAndResetLines();
	};

}