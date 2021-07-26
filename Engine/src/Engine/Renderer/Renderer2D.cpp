#include "pch.h"
#include "Renderer2D.h"

#include <glad/glad.h>
//#define GL_VERSION_4_3
//#include "GL/glcorearb.h"
//#include "GL/gl3w.h"
//#include "sdl2/SDL_opengl.h

#include "Engine/Core/Log.h"

#include "Engine/Memory/MemoryCommon.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine
{
	struct QuadVertex
	{
		glm::vec4 position{};
		glm::vec4 colour{};
		glm::vec2 texCoord{};
		float TexIndex{};
		float TilingFactor{};
		//
		//// Editor-only
		//int EntityID;
		//QuadVertex(glm::vec3 p, glm::vec4 col, glm::vec2 tex) :position{ p }, colour{col}, texCoord{tex}{}
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
		GLuint WhiteTexture{};
		GLuint vaoid{};
		GLuint shaderid{};

		GLuint batchVAO{};
		GLuint batchBuffer{};

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<GLuint, MaxTextureSlots> TextureSlots{};
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		std::array<glm::vec4, 4> QuadVertexPositions;

		Renderer2D::BatchRenderStats Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection{ 1.0f };
		};
		CameraData CameraBuffer;
		//Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData s_Data;


void engine::Renderer2D::Init()
{
#if 1
	std::array<QuadVertex, 4> vertices{
	};
		//					position    colour,  uv			
	vertices[0] = { glm::vec4{ -0.5f,-0.5f, 1.0f,1.0f }, glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f }, glm::vec2{ 0.0f, 0.0f }, 1.0f };
	vertices[1] = { glm::vec4{ 0.5f,-0.5f, 1.0f,1.0f} , glm::vec4{0.0f, 1.0f, 0.0f, 1.0f},glm::vec2{1.0f, 0.0f},1.0f };
	vertices[2] = { glm::vec4{-0.5f, 0.5f, 1.0f,1.0f} , glm::vec4{1.0f, 0.0f, 1.0f, 1.0f},glm::vec2{0.0f, 1.0f},1.0f };
	vertices[3] = { glm::vec4{ 0.5f, 0.5f, 1.0f,1.0f} , glm::vec4{0.0f, 0.0f, 1.0f, 1.0f},glm::vec2{1.0f, 1.0f},1.0f };


	// Get a buffer handle
	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	//creates and initializes a buffer object
	glNamedBufferStorage(vbo_hdl,
						 sizeof(QuadVertex) * vertices.size(),
						 vertices.data(), GL_DYNAMIC_STORAGE_BIT);

	glCreateVertexArrays(1, &  s_Data.vaoid);
							   
	glVertexArrayVertexBuffer( s_Data.vaoid, 8, vbo_hdl, 0, sizeof(QuadVertex));
	glEnableVertexArrayAttrib( s_Data.vaoid, 0);
	glVertexArrayAttribFormat( s_Data.vaoid, 0, 4, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, position));
	glVertexArrayAttribBinding(s_Data.vaoid, 0, 8);
							  
	glEnableVertexArrayAttrib( s_Data.vaoid, 1);
	glVertexArrayAttribFormat( s_Data.vaoid, 1, 4, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, colour));
	glVertexArrayAttribBinding(s_Data.vaoid, 1, 8);
							  
	glEnableVertexArrayAttrib(s_Data.vaoid, 2);
	glVertexArrayAttribFormat(s_Data.vaoid, 2, 2, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, texCoord));
	glVertexArrayAttribBinding(s_Data.vaoid, 2, 8);

	glEnableVertexArrayAttrib(s_Data.vaoid, 4);
	glVertexArrayAttribFormat(s_Data.vaoid, 4, 1, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, TilingFactor));
	glVertexArrayAttribBinding(s_Data.vaoid, 4, 8);
	// indices array
	std::array<GLushort, 4> idx_vtx{
		2,0,3,1
	};
	
	//create one buffer
	GLuint e_hdl{};
	glCreateBuffers(1, &e_hdl);
	// index buffer ofject
	glNamedBufferStorage(e_hdl,
						 sizeof(GLushort) * idx_vtx.size(),
						 reinterpret_cast<GLvoid*>(idx_vtx.data()),
						 GL_DYNAMIC_STORAGE_BIT);
	
	//link the vao to the indices array
	glVertexArrayElementBuffer(s_Data.vaoid, e_hdl);

#endif

	s_Data.QuadVertexPositions[0] = { -0.5f,-0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[1] = { -0.5f, 0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[2] = {  0.5f, 0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[3] = {  0.5f,-0.5f, 0.0f, 1.0f };


	size_t bufferSize = sizeof(QuadVertex) * s_Data.MaxVertices;
	glCreateBuffers(1, &s_Data.batchBuffer);
	glNamedBufferStorage(s_Data.batchBuffer, bufferSize, NULL, GL_DYNAMIC_STORAGE_BIT);
	
	glCreateVertexArrays(1, &s_Data.batchVAO);

	glVertexArrayVertexBuffer(s_Data.batchVAO, 8, s_Data.batchBuffer, 0, sizeof(QuadVertex));
	glEnableVertexArrayAttrib(s_Data.batchVAO, 0);
	glVertexArrayAttribFormat(s_Data.batchVAO, 0, 4, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, position));
	glVertexArrayAttribBinding(s_Data.batchVAO, 0, 8);

	glEnableVertexArrayAttrib(s_Data.batchVAO, 1);
	glVertexArrayAttribFormat(s_Data.batchVAO, 1, 4, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, colour));
	glVertexArrayAttribBinding(s_Data.batchVAO, 1, 8);

	glEnableVertexArrayAttrib(s_Data.batchVAO, 2);
	glVertexArrayAttribFormat(s_Data.batchVAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, texCoord));
	glVertexArrayAttribBinding(s_Data.batchVAO, 2, 8);

	glEnableVertexArrayAttrib(s_Data.batchVAO, 3);
	glVertexArrayAttribFormat(s_Data.batchVAO, 3, 1, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, TexIndex));
	glVertexArrayAttribBinding(s_Data.batchVAO, 3, 8);
	
	glEnableVertexArrayAttrib(s_Data.batchVAO, 4);
	glVertexArrayAttribFormat(s_Data.batchVAO, 4, 1, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, TilingFactor));
	glVertexArrayAttribBinding(s_Data.batchVAO, 4, 8);

	
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
	glVertexArrayElementBuffer(s_Data.batchVAO, ebo_hdl);

	delete[] quadIndices;

	glBindVertexArray(0);


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



	//mdl.primative_type = GL_TRIANGLE_STRIP;
	//mdl.draw_cnt = vertices.size();
	//mdl.primative_cnt = mdl.draw_cnt;
	//mdl.index_cnt = idx_vtx.size();

	std::string vShader = R"raw(
#version 450

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 col;
layout(location = 2) in vec2 tex;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_tiling;

layout(location = 1)out vec4 fCol;
layout(location = 2)out vec2 TexCoord;
layout(location = 3)out float f_TexIndex;

//uniform mat4 uModel_xform;
uniform mat4 uViewProj_xform;

void main()
{
//gl_Position = uViewProj_xform* uModel_xform *  pos;
gl_Position = uViewProj_xform*  pos;
fCol = col;
TexCoord = tex;
f_TexIndex = a_TexIndex;
}
)raw";

	std::string fShader = R"raw(
#version 450

layout(location = 1)in vec4 colour;
layout(location = 2)in vec2 TexCoord;
layout(location = 3)in float v_TexIndex;

out vec4 FragColour;

uniform sampler2D uTex2d[32];

void main()
{
   int index =  int(v_TexIndex);
    FragColour = texture(uTex2d[index], TexCoord)* colour;
	//FragColour = vec4(1.0f,0,0,1.0f);
})raw";

	s_Data.shaderid = glCreateProgram();
	const GLchar* ptr = vShader.c_str();
	GLint  len = static_cast<GLint>(vShader.length());
	GLuint vertShad = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShad, 1,&(ptr), &len);
	glCompileShader(vertShad);
	{
		GLint result = 0;
		GLchar eLog[1024] = { 0 };
		glGetShaderiv(vertShad, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(vertShad, sizeof(eLog), NULL, eLog);
		#if defined(DEBUG) | defined(_DEBUG)
				printf("Error compiling the %d shader : '%s' \n", GL_VERTEX_SHADER, eLog);
		#endif
			return;
		}
	}

	ptr = fShader.c_str();
    len = fShader.length();
	GLuint fragShad = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShad, 1, &(ptr), &len);
	glCompileShader(fragShad);
	{
		GLint result = 0;
		GLchar eLog[1024] = { 0 };
		glGetShaderiv(fragShad, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(fragShad, sizeof(eLog), NULL, eLog);
		#if defined(DEBUG) | defined(_DEBUG)
					printf("Error compiling the %d shader : '%s' \n", GL_FRAGMENT_SHADER, eLog);
		#endif
			return;
		}
	}

	glAttachShader(s_Data.shaderid, vertShad);
	glAttachShader(s_Data.shaderid, fragShad);

	glLinkProgram(s_Data.shaderid);
	{
			GLint result = 0;
			GLchar eLog[1024] = { 0 };
		glGetProgramiv(s_Data.shaderid, GL_LINK_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(s_Data.shaderid, sizeof(eLog), NULL, eLog);
			#if defined(DEBUG) | defined(_DEBUG)
					printf("Error linking program: '%s' \n", eLog);
			#endif
			return;
		}
	}

	glValidateProgram(s_Data.shaderid);
	{
		GLint result = 0;
		GLchar eLog[1024] = { 0 };
		glGetProgramiv(s_Data.shaderid, GL_VALIDATE_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(s_Data.shaderid, sizeof(eLog), NULL, eLog);
#if defined(DEBUG) | defined(_DEBUG)
			printf("Error validating program: '%s' \n", eLog);
#endif
			return;
		}
	}

}

void engine::Renderer2D::Shutdown()
{
	glDeleteVertexArrays(1,&s_Data.batchVAO);
	glDeleteTextures(1, &s_Data.WhiteTexture);
	delete[] s_Data.QuadVertexBufferBase;
}


void engine::Renderer2D::BeginScene(const OrthographicCamera& camera)
{
	s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();

	s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	s_Data.TextureSlotIndex = 1;

	//for (uint32_t i = 1; i < s_Data.TextureSlots.size(); i++)
	//	s_Data.TextureSlots[i] = nullptr;

}

void engine::Renderer2D::EndScene()
{
	FlushAndReset();
}

void Renderer2D::Flush()
{

	size_t dataSize = ((char*)s_Data.QuadVertexBufferPtr - (char*)s_Data.QuadVertexBufferBase);
	
		glNamedBufferSubData(s_Data.batchBuffer, 0, dataSize, s_Data.QuadVertexBufferBase);

		glBindVertexArray(s_Data.batchVAO);

		glUseProgram(s_Data.shaderid);

		int samplers[32];
		for (size_t i = 0; i < 32; i++)
		{
			samplers[i] = i;
		}
		GLuint tex_loc = glGetUniformLocation(s_Data.shaderid, "uTex2d");
		glUniform1iv(tex_loc, 32, samplers);

		//// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			glBindTextureUnit(i, s_Data.TextureSlots[i]);
		}

		GLuint task_loc = glGetUniformLocation(s_Data.shaderid, "uViewProj_xform");
		glUniformMatrix4fv(task_loc, 1, false, glm::value_ptr(s_Data.CameraBuffer.ViewProjection));

		glDrawElements(GL_TRIANGLES, s_Data.QuadIndexCount, GL_UNSIGNED_SHORT, NULL);
		
	//glUseProgram(0);
	//Renderer::DrawIndexed(s_Data->QuadIndexCount, false);
	s_Data.Stats.DrawCalls++;
	s_Data.Stats.QuadCount += s_Data.QuadIndexCount/6;
}

void engine::Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	DrawQuad({ position.x,position.y, 0.0f }, size, color);
}

void engine::Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
	glm::mat4 mdl_xform = {
		 size.x,	0.0f,		0.0f,		0.0f,
		 0.0f,		size.y,		0.0f,		0.0f,
		 0.0f,		0.0f,		0.0f,		0.0f,
		 position.x,position.y, 0.0f,		1.0f
	};

	constexpr size_t quadVertexCount = 4;
	//const float textureIndex = 0.0f; // White Texture
	// eventually can change textures coords in sprite
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
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

#if old
	//glBindVertexArray(s_Data.vaoid);
	//glUseProgram(s_Data.shaderid);
	//GLuint task_loc = glGetUniformLocation(s_Data.shaderid, "uViewProj_xform");
	//glUniformMatrix4fv(task_loc, 1, false, glm::value_ptr(s_Data.CameraBuffer.ViewProjection));

	//task_loc = glGetUniformLocation(s_Data.shaderid, "uModel_xform");
	//glUniformMatrix4fv(task_loc, 1, false, glm::value_ptr(mdl_xform));

	//task_loc = glGetUniformLocation(s_Data.shaderid, "col");
	//glUniform4fv(task_loc, 1, glm::value_ptr(color));

	//glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, NULL);
	//glUseProgram(0);
	//glBindVertexArray(0);
	////shd_ref->second.UnUse();
#endif
}

void engine::Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
{
	DrawRotatedQuad({ position.x,position.y,0.0f }, size, rotation, color);
}

void engine::Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
{
	GLfloat rot = glm::radians(rotation);
	glm::mat4 mdl_xform = {
		cosf(rot) * size.x,		sinf(rot) * size.x,	0.0f, 0.0f,
		-sinf(rot) * size.y,	cosf(rot) * size.y,	0.0f, 0.0f,
		 0.0f,					0.0f,				0.0f, 0.0f,
		position.x,				position.y,			0.0f, 1.0f
	};

	constexpr size_t quadVertexCount = 4;
	//const float textureIndex = 0.0f; // White Texture
	// eventually can change textures coords in sprite
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
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



#ifdef old
	glBindVertexArray(s_Data.vaoid);
	glUseProgram(s_Data.shaderid);
	GLuint task_loc = glGetUniformLocation(s_Data.shaderid, "uViewProj_xform");
	glUniformMatrix4fv(task_loc, 1, false, glm::value_ptr(s_Data.CameraBuffer.ViewProjection));

	task_loc = glGetUniformLocation(s_Data.shaderid, "uModel_xform");
	glUniformMatrix4fv(task_loc, 1, false, glm::value_ptr(mdl_xform));

	task_loc = glGetUniformLocation(s_Data.shaderid, "col");
	glUniform4fv(task_loc, 1, glm::value_ptr(color));

	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, NULL);
	glUseProgram(0);
	glBindVertexArray(0);
#endif

}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const ooTexID& texture, float tilingFactor, const glm::vec4& tintColor)
{																										
	DrawRotatedQuad({ position.x,position.y,0.0f }, size, rotation,texture, tilingFactor, tintColor);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const ooTexID& texture, float tilingFactor, const glm::vec4& tintColor)
{
	GLfloat rot = glm::radians(rotation);
	glm::mat4 mdl_xform = {
		cosf(rot) * size.x,		sinf(rot) * size.x,	0.0f, 0.0f,
		-sinf(rot) * size.y,	cosf(rot) * size.y,	0.0f, 0.0f,
		 0.0f,					0.0f,				0.0f, 0.0f,
		position.x,				position.y,			0.0f, 1.0f
	};

	constexpr size_t quadVertexCount = 4;
	//const float textureIndex = 0.0f; // White Texture
	// eventually can change textures coords in sprite
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

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

	/* 
	glBindVertexArray(s_Data.vaoid);
	glUseProgram(s_Data.shaderid);


	glBindTextureUnit(1, texture);
	GLuint tex_loc = glGetUniformLocation(s_Data.shaderid, "uTex2d");

	GLuint task_loc = glGetUniformLocation(s_Data.shaderid, "uViewProj_xform");
	glUniformMatrix4fv(task_loc, 1, false, glm::value_ptr(s_Data.CameraBuffer.ViewProjection));

	task_loc = glGetUniformLocation(s_Data.shaderid, "uModel_xform");
	glUniformMatrix4fv(task_loc, 1, false, glm::value_ptr(mdl_xform));

	task_loc = glGetUniformLocation(s_Data.shaderid, "col");
	glUniform4fv(task_loc, 1, glm::value_ptr(tintColor));

	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, NULL);
	glUseProgram(0);
	glBindVertexArray(0);*/
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



}