/************************************************************************************//*!
\file          Renderer2D.cpp
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 1, 2021
\brief         File contains implementation of a 2D renderer implemented with OpenGL 4.5


Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "Renderer2D.h"

#include <glad/glad.h>

#include "Engine/Core/Log.h"
#include "Engine/Core/Assert.h"

#include "Engine/Memory/MemoryCommon.h"

//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include <oom/oom.hpp>

#include "Engine/ECS/WorldManager.h"
#include "Engine/Transform/Transform3D.h"

#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Renderer/RenderPass.h"
#include "Engine/Platform/OpenGL/OpenGLShader.h"

namespace engine
{
	struct QuadVertex
	{
		oom::vec4 position{};
		oom::vec4 colour{};
		oom::vec2 texCoord{};
		float TexIndex{};
		float TilingFactor{};
		//
		//// Editor-only
		//int EntityID;
		//QuadVertex(oom::vec3 p, oom::vec4 col, oom::vec2 tex) :position{ p }, colour{col}, texCoord{tex}{}
	};

	struct LineVertex
	{
		oom::vec4 position{};
		oom::vec4 colour{};
	};


	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		//Ref<VertexArray> QuadVertexArray;
		//Ref<VertexBuffer> QuadVertexBuffer;
		//Ref<Shader> TextureShader;

		std::shared_ptr<Framebuffer> frameBuffer;

		GLuint WhiteTexture{};

		GLuint quadBatchVAO{};
		GLuint quadBatchBuffer{};

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<GLuint, MaxTextureSlots> TextureSlots{};
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		std::array<oom::vec4, 4> QuadVertexPositions;

		Renderer2D::BatchRenderStats Stats;

		static const uint32_t MaxLines = 20000;
		static const uint32_t MaxLineVertice = MaxLines * 2;
		GLuint lineBatchVAO{};
		GLuint lineBatchBuffer{};

		static const uint32_t CircleNumSegments= 32;
		uint32_t LineIndexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;
		std::array<oom::vec4, CircleNumSegments> LineVertexPositions;

		struct CameraData
		{
			oom::mat4 View{ 1.0f };
			oom::mat4 Projection{ 1.0f };
			oom::mat4 ViewProjection{ 1.0f };
		};
		CameraData CameraBuffer;
		//Ref<UniformBuffer> CameraUniformBuffer;

		std::shared_ptr<Shader> TextureQuadsShader;
		std::shared_ptr<Shader> linesShader;
	};

	static Renderer2DData s_Data;


void engine::Renderer2D::Init()
{

	FramebufferSpecification fbSpec{};
	fbSpec.DebugName = "Renderer2D_FB";
	fbSpec.Attachments = { ImageFormat::RGBA32F, ImageFormat::Depth };
	fbSpec.Samples = 1;
	fbSpec.ClearOnLoad = false;
	fbSpec.SwapChainTarget = false;
	fbSpec.ClearColor = { 0.18 , 0.04, 0.14, 1.0 };

	s_Data.frameBuffer = Framebuffer::Create(fbSpec);

	FramebufferPool::Add("2D_Framebuffer", s_Data.frameBuffer);

	RenderPassSpecification rpSpec{};
	rpSpec.DebugName = "Renderer2D_RP";
	rpSpec.TargetFramebuffer = s_Data.frameBuffer;

	std::shared_ptr RenderPass = RenderPass::Create(rpSpec);

	//Initilization of quad buffer
	{

	s_Data.QuadVertexPositions[0] = { -0.5f,-0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[1] = { -0.5f, 0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[2] = {  0.5f, 0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[3] = {  0.5f,-0.5f, 0.0f, 1.0f };

	size_t bufferSize = sizeof(QuadVertex) * s_Data.MaxVertices;
	glCreateBuffers(1, &s_Data.quadBatchBuffer);
	glNamedBufferStorage(s_Data.quadBatchBuffer, bufferSize, NULL, GL_DYNAMIC_STORAGE_BIT);
	
	glCreateVertexArrays(1, &s_Data.quadBatchVAO);

	glVertexArrayVertexBuffer(s_Data.quadBatchVAO, 8, s_Data.quadBatchBuffer, 0, sizeof(QuadVertex));
	glEnableVertexArrayAttrib(s_Data.quadBatchVAO, 0);
	glVertexArrayAttribFormat(s_Data.quadBatchVAO, 0, 4, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, position));
	glVertexArrayAttribBinding(s_Data.quadBatchVAO, 0, 8);

	glEnableVertexArrayAttrib(s_Data.quadBatchVAO, 1);
	glVertexArrayAttribFormat(s_Data.quadBatchVAO, 1, 4, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, colour));
	glVertexArrayAttribBinding(s_Data.quadBatchVAO, 1, 8);

	glEnableVertexArrayAttrib(s_Data.quadBatchVAO, 2);
	glVertexArrayAttribFormat(s_Data.quadBatchVAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, texCoord));
	glVertexArrayAttribBinding(s_Data.quadBatchVAO, 2, 8);

	glEnableVertexArrayAttrib(s_Data.quadBatchVAO, 3);
	glVertexArrayAttribFormat(s_Data.quadBatchVAO, 3, 1, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, TexIndex));
	glVertexArrayAttribBinding(s_Data.quadBatchVAO, 3, 8);
	
	glEnableVertexArrayAttrib(s_Data.quadBatchVAO, 4);
	glVertexArrayAttribFormat(s_Data.quadBatchVAO, 4, 1, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, TilingFactor));
	glVertexArrayAttribBinding(s_Data.quadBatchVAO, 4, 8);
	}
	
	s_Data.QuadVertexBufferBase = OONEW QuadVertex[s_Data.MaxVertices];
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

	// predetermine indices because all of them are the same. we dont care
	GLushort* quadIndices = OONEW GLushort[s_Data.MaxIndices];
	uint32_t offset = 0;
	for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}
	//create one buffer
	GLuint ebo_hdl{};
	glCreateBuffers(1, &ebo_hdl);
	// index buffer ofject
	glNamedBufferStorage(ebo_hdl,
						 sizeof(GLushort) * s_Data.MaxIndices,
						 reinterpret_cast<GLvoid*>(quadIndices),
						 GL_DYNAMIC_STORAGE_BIT);
	//link the vao to the indices array
	glVertexArrayElementBuffer(s_Data.quadBatchVAO, ebo_hdl);
	glBindVertexArray(0);

	delete[] quadIndices;

	// 1x1 white texture
	{
	glCreateTextures(GL_TEXTURE_2D, 1, &s_Data.WhiteTexture);
		// allocate GPU storage for texture image data loaded from file
	glTextureStorage2D(s_Data.WhiteTexture, 1, GL_RGBA8, 1, 1);
	// copy image data from client memory to GPU texture buffer memory
	uint32_t whiteCol = 0xFFFFFFFF;
	glTextureSubImage2D(s_Data.WhiteTexture, 0, 0, 0, 1, 1,
						GL_RGBA, GL_UNSIGNED_BYTE, &whiteCol);

	glTextureParameteri(s_Data.WhiteTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(s_Data.WhiteTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(s_Data.WhiteTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(s_Data.WhiteTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	s_Data.TextureSlotIndex = 1;
	s_Data.TextureSlots[0] = s_Data.WhiteTexture;
	for (size_t i = 1; i < s_Data.TextureSlots.size(); i++)
	{
		s_Data.TextureSlots[i] = 0;
	}


	//Line vertices 
	{
		constexpr float theta = 2.0f * oom::pi<float>() / static_cast<float>(s_Data.CircleNumSegments);
		float c = cosf(theta);//precalculate the sine and cosine
		float s = sinf(theta);
		float t{};

		float radius = 0.5f;
		float x = radius;//we start at angle = 0 
		float y{};

		for (int ii = 0; ii < s_Data.CircleNumSegments; ii++)
		{
			s_Data.LineVertexPositions[ii] = { x + c * x , y + c * y, 0.0f, 1.0f };
			//apply the rotation matrix
			t = x;
			x = c * x - s * y;
			y = s * t + c * y;
		}
	}

	s_Data.LineVertexBufferBase = OONEW LineVertex[s_Data.MaxLineVertice];
	s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

	size_t lineBufferSize = sizeof(LineVertex) * s_Data.MaxLineVertice;
	glCreateBuffers(1, &s_Data.lineBatchBuffer);
	glNamedBufferStorage(s_Data.lineBatchBuffer, lineBufferSize, NULL, GL_DYNAMIC_STORAGE_BIT);

	glCreateVertexArrays(1, &s_Data.lineBatchVAO);

	glVertexArrayVertexBuffer(s_Data.lineBatchVAO, 8, s_Data.lineBatchBuffer, 0, sizeof(LineVertex));
	glEnableVertexArrayAttrib(s_Data.lineBatchVAO, 0);
	glVertexArrayAttribFormat(s_Data.lineBatchVAO, 0, 4, GL_FLOAT, GL_FALSE, offsetof(LineVertex,position));
	glVertexArrayAttribBinding(s_Data.lineBatchVAO, 0, 8);

	glEnableVertexArrayAttrib(s_Data.lineBatchVAO, 1);
	glVertexArrayAttribFormat(s_Data.lineBatchVAO, 1, 4, GL_FLOAT, GL_FALSE, offsetof(LineVertex, colour));
	glVertexArrayAttribBinding(s_Data.lineBatchVAO, 1, 8);

	s_Data.TextureQuadsShader = Shader::Create(R"(..\Engine\assets\shaders\quadShader.glsl)");
	s_Data.linesShader = Shader::Create(R"(..\Engine\assets\shaders\linesShader.glsl)");

}

void engine::Renderer2D::Shutdown()
{
	glDeleteVertexArrays(1,&s_Data.quadBatchVAO);
	glDeleteTextures(1, &s_Data.WhiteTexture);
	delete[] s_Data.QuadVertexBufferBase;

	glDeleteVertexArrays(1,&s_Data.lineBatchVAO);
	delete[] s_Data.LineVertexBufferBase;
}

void Renderer2D::BeginScene(const oom::mat4& viewProj, const oom::mat4& view)
{
	s_Data.CameraBuffer.View = view;
	s_Data.CameraBuffer.ViewProjection = viewProj;
	glEnable(GL_DEPTH_TEST);

	//s_Data.frameBuffer->Bind();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	s_Data.TextureSlotIndex = 1;
}


void engine::Renderer2D::EndScene()
{
	FlushAndReset();
	FlushAndResetLines();

	//make mipmaps for framebuffer XD
	// TODO: FIX THIS SHIT
	glGenerateTextureMipmap(s_Data.frameBuffer->GetRendererID());

	s_Data.frameBuffer->Unbind();
}

void Renderer2D::Flush()
{


	size_t dataSize = ((char*)s_Data.QuadVertexBufferPtr - (char*)s_Data.QuadVertexBufferBase);
	glNamedBufferSubData(s_Data.quadBatchBuffer, 0, dataSize, s_Data.QuadVertexBufferBase);
	
	glBindVertexArray(s_Data.quadBatchVAO);

	auto qShdr = std::dynamic_pointer_cast<OpenGLShader>(s_Data.TextureQuadsShader);

	qShdr->Bind();

	int samplers[32];
	for (int i = 0; i < 32; i++)
	{
		samplers[i] = i;
	}

	qShdr->SetUniform("uTex2d", 32, samplers);
	qShdr->SetUniform("uViewProj_xform", s_Data.CameraBuffer.ViewProjection);

	//// Bind textures
	for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
	{
		glBindTextureUnit(i, s_Data.TextureSlots[i]);
	}


	glDrawElements(GL_TRIANGLES, s_Data.QuadIndexCount, GL_UNSIGNED_SHORT, NULL);

	//Renderer::DrawIndexed(s_Data->QuadIndexCount, false);
	s_Data.Stats.DrawCalls++;
	s_Data.Stats.QuadCount += s_Data.QuadIndexCount/6;
}

void Renderer2D::FlushLines()
{
	//Drawing lines dont need depth ?? maybe??
	glDisable(GL_DEPTH_TEST);

	size_t dataSize = ((char*)s_Data.LineVertexBufferPtr - (char*)s_Data.LineVertexBufferBase);
	glNamedBufferSubData(s_Data.lineBatchBuffer, 0, dataSize, s_Data.LineVertexBufferBase);

	glBindVertexArray(s_Data.lineBatchVAO);

	auto lShdr = std::dynamic_pointer_cast<OpenGLShader>(s_Data.linesShader);

	lShdr->Bind();

	lShdr->SetUniform("uViewProj_xform", s_Data.CameraBuffer.ViewProjection);

	glLineWidth(1.0f);
	glDrawArrays(GL_LINES,0, s_Data.LineIndexCount);

	//Renderer::DrawIndexed(s_Data->QuadIndexCount, false);
	//s_Data.Stats.DrawCalls++;
	//s_Data.Stats.QuadCount += s_Data.QuadIndexCount / 6;
}

void engine::Renderer2D::DrawQuad(const oom::vec2& position, const oom::vec2& size, const oom::vec4& color)
{
	DrawQuad({ position.x,position.y, 0.0f }, size, color);
}

void engine::Renderer2D::DrawQuad(const oom::vec3& position, const oom::vec2& size, const oom::vec4& color)
{
	oom::mat4 mdl_xform = {
		 size.x,	0.0f,		0.0f,		0.0f,
		 0.0f,		size.y,		0.0f,		0.0f,
		 0.0f,		0.0f,		0.0f,		0.0f,
		 position.x,position.y, position.z,	1.0f
	};

	constexpr size_t quadVertexCount = 4;
	//const float textureIndex = 0.0f; // White Texture
	// eventually can change textures coords in sprite
	constexpr oom::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	const float tilingFactor = 1.0f;

	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		FlushAndReset();

	float defaultTexIndex = 0.0f;

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		s_Data.QuadVertexBufferPtr->position = mdl_xform * s_Data.QuadVertexPositions[i];
		s_Data.QuadVertexBufferPtr->colour = color;
		s_Data.QuadVertexBufferPtr->texCoord = textureCoords[i];
		s_Data.QuadVertexBufferPtr->TexIndex = defaultTexIndex; // white tex
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;
	}

	s_Data.QuadIndexCount += 6;
}

void engine::Renderer2D::DrawRotatedQuad(const oom::vec2& position, const oom::vec2& size, float rotation, const oom::vec4& color)
{
	DrawRotatedQuad({ position.x,position.y,0.0f }, size, rotation, color);
}

void engine::Renderer2D::DrawRotatedQuad(const oom::vec3& position, const oom::vec2& size, float rotation, const oom::vec4& color)
{
	GLfloat rot = oom::radians(rotation);
	oom::mat4 mdl_xform = {
		cosf(rot) * size.x,		sinf(rot) * size.x,	0.0f,		0.0f,
		-sinf(rot) * size.y,	cosf(rot) * size.y,	0.0f,		0.0f,
		 0.0f,					0.0f,				0.0f,		0.0f,
		position.x,				position.y,			position.z, 1.0f
	};

	constexpr size_t quadVertexCount = 4;
	//const float textureIndex = 0.0f; // White Texture
	// eventually can change textures coords in sprite
	constexpr oom::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	float tilingFactor = 1.0f;

	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		FlushAndReset();

	float defaultTexIndex = 0.0f;
	for (size_t i = 0; i < quadVertexCount; i++)
	{
		s_Data.QuadVertexBufferPtr->position = mdl_xform * s_Data.QuadVertexPositions[i];
		s_Data.QuadVertexBufferPtr->colour = color;
		s_Data.QuadVertexBufferPtr->texCoord = textureCoords[i];
		s_Data.QuadVertexBufferPtr->TexIndex = defaultTexIndex; // white textures
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;
	}

	s_Data.QuadIndexCount += 6;
}

void Renderer2D::DrawRotatedQuad(const oom::vec2& position, const oom::vec2& size, float rotation, const ooRendererID& texture, float tilingFactor, const oom::vec4& tintColor)
{																										
	DrawRotatedQuad({ position.x,position.y,0.0f }, size, rotation,texture, tilingFactor, tintColor);
}

void Renderer2D::DrawRotatedQuad(const oom::vec3& position, const oom::vec2& size, float rotation, const ooRendererID& texture, float tilingFactor, const oom::vec4& tintColor)
{
	GLfloat rot = oom::radians(rotation);
	oom::mat4 mdl_xform = {
		cosf(rot) * size.x,		sinf(rot) * size.x,	0.0f,		0.0f,
		-sinf(rot) * size.y,	cosf(rot) * size.y,	0.0f,		0.0f,
		 0.0f,					0.0f,				0.0f,		0.0f,
		position.x,				position.y,			position.z, 1.0f
	};

	constexpr size_t quadVertexCount = 4;
	//const float textureIndex = 0.0f; // White Texture
	// eventually can change textures coords in sprite
	constexpr oom::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		FlushAndReset();

	float textureIndex = 0.0f;
	//check for valid texture
	if (texture == static_cast<uint32_t>(-1))
	{
	}
	else{
		//check for existing texture
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		//texture doesnt exist
		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex == Renderer2DData::MaxTextureSlots)
				FlushAndReset();

			textureIndex = (float)s_Data.TextureSlotIndex;
			ENGINE_ASSERT(s_Data.TextureSlotIndex < Renderer2DData::MaxTextureSlots)
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}
	}

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		s_Data.QuadVertexBufferPtr->position = mdl_xform * s_Data.QuadVertexPositions[i];
		s_Data.QuadVertexBufferPtr->colour = tintColor;
		s_Data.QuadVertexBufferPtr->texCoord = textureCoords[i];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex; // textures
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;
	}
	s_Data.QuadIndexCount += 6;
}

void Renderer2D::DrawCircle(const oom::vec3& position, float rotation, float radius, const oom::vec4& color)
{
	GLfloat rot = oom::radians(rotation);
	oom::mat4 mdl_xform = {
		 cosf(rot)* radius,		sinf(rot)* radius,	0.0f,		0.0f,
		 -sinf(rot) * radius,	cosf(rot)* radius,		0.0f,		0.0f,
		 0.0f,		0.0f,		0.0f,		0.0f,
		 position.x,position.y, position.z,		1.0f
	};

	oom::vec4 firstLine0 = mdl_xform * oom::vec4{0,0,0,1.0f};
	oom::vec4 firstLine1 = mdl_xform * s_Data.LineVertexPositions[0];
	DrawLine(firstLine0, firstLine1, color);
	for (size_t i = 0; i < s_Data.CircleNumSegments -1; i++)
	{
		oom::vec4 p0 = mdl_xform * s_Data.LineVertexPositions[i];
		oom::vec4 p1 = mdl_xform * s_Data.LineVertexPositions[i+1];
		DrawLine(p0, p1, color);
	}
	oom::vec4 p0 = mdl_xform * s_Data.LineVertexPositions[s_Data.CircleNumSegments-1];
	oom::vec4 p1 = mdl_xform * s_Data.LineVertexPositions[0];
	DrawLine(p0, p1, color);
}

void Renderer2D::DrawLine(const oom::vec3& p0, const oom::vec3& p1, const oom::vec4& color)
{
	if (s_Data.LineIndexCount >= Renderer2DData::MaxLineVertice)
		FlushAndResetLines();

	s_Data.LineVertexBufferPtr->position = oom::vec4{ p0 ,1.0f};
	s_Data.LineVertexBufferPtr->colour = color;
	s_Data.LineVertexBufferPtr++;

	s_Data.LineVertexBufferPtr->position = oom::vec4{ p1, 1.0f };
	s_Data.LineVertexBufferPtr->colour = color;
	s_Data.LineVertexBufferPtr++;

	s_Data.LineIndexCount += 2;

	//s_Data.Stats.LineCount++;
}

void Renderer2D::ResetStats()
{
	s_Data.Stats.DrawCalls = 0;
	s_Data.Stats.QuadCount = 0;
}

Renderer2D::BatchRenderStats Renderer2D::GetStats()
{
	return s_Data.Stats;
}

void Renderer2D::FlushAndReset()
{
	Flush();

	s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		  
	s_Data.TextureSlotIndex = 1;
}

void Renderer2D::FlushAndResetLines()
{
	FlushLines();

	s_Data.LineIndexCount = 0;
	s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;
}



}