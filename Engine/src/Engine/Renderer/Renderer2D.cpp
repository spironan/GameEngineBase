#include "pch.h"
#include "Renderer2D.h"

#include <glad/glad.h>
//#define GL_VERSION_4_3
//#include "GL/glcorearb.h"
//#include "GL/gl3w.h"
//#include "sdl2/SDL_opengl.h

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine
{
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 colour;
		glm::vec2 texCoord;
		//float TexIndex;
		//float TilingFactor;
		//
		//// Editor-only
		//int EntityID;
		QuadVertex(glm::vec3 p, glm::vec4 col, glm::vec2 tex) :position{ p }, colour{col}, texCoord{tex}{}
	};


	struct Renderer2DData
	{
		//static const uint32_t MaxQuads = 20000;
		//static const uint32_t MaxVertices = MaxQuads * 4;
		//static const uint32_t MaxIndices = MaxQuads * 6;
		//static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		//Ref<VertexArray> QuadVertexArray;
		//Ref<VertexBuffer> QuadVertexBuffer;
		//Ref<Shader> TextureShader;
		//Ref<Texture2D> WhiteTexture;
		GLuint vaoid{};
		GLuint shaderid{};

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		//std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;

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
	//GLModel mdl{};
		// position
	std::array<QuadVertex, 4> vertices{
		//					position    colour,  uv			
		QuadVertex({-0.5f,-0.5f, 1.0f} , {1.0f, 0.0f, 0.0f, 1.0f},{0.0f, 0.0f}),
		QuadVertex({ 0.5f,-0.5f, 1.0f} , {0.0f, 1.0f, 0.0f, 1.0f},{1.0f, 0.0f}),
		QuadVertex({-0.5f, 0.5f, 1.0f} , {1.0f, 0.0f, 1.0f, 1.0f},{0.0f, 1.0f}),
		QuadVertex({ 0.5f, 0.5f, 1.0f} , {0.0f, 0.0f, 1.0f, 1.0f},{1.0f, 1.0f})
	};

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
	glVertexArrayAttribFormat( s_Data.vaoid, 0, 3, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, position));
	glVertexArrayAttribBinding(s_Data.vaoid, 0, 8);
							  
	glEnableVertexArrayAttrib( s_Data.vaoid, 1);
	glVertexArrayAttribFormat( s_Data.vaoid, 1, 4, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, colour));
	glVertexArrayAttribBinding(s_Data.vaoid, 1, 8);
							  
							  
	glEnableVertexArrayAttrib( s_Data.vaoid, 2);
	glVertexArrayAttribFormat( s_Data.vaoid, 2, 2, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, texCoord));
	glVertexArrayAttribBinding(s_Data.vaoid, 2, 8);
	// indices array
	std::array<GLushort, 4> idx_vtx{
		2,0,3,1
	};

	//create one buffer
	GLuint ebo_hdl{};
	glCreateBuffers(1, &ebo_hdl);
	// index buffer ofject
	glNamedBufferStorage(ebo_hdl,
						 sizeof(GLushort) * idx_vtx.size(),
						 reinterpret_cast<GLvoid*>(idx_vtx.data()),
						 GL_DYNAMIC_STORAGE_BIT);

	//link the vao to the indices array
	glVertexArrayElementBuffer(s_Data.vaoid, ebo_hdl);

	glBindVertexArray(0);

	//mdl.primative_type = GL_TRIANGLE_STRIP;
	//mdl.draw_cnt = vertices.size();
	//mdl.primative_cnt = mdl.draw_cnt;
	//mdl.index_cnt = idx_vtx.size();

	std::string vShader = R"raw(
#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 col;
layout(location = 2) in vec2 tex;

layout(location = 1)out vec2 TexCoord;
layout(location = 2)out vec4 fCol;

uniform mat4 uModel_xform;
uniform mat4 uViewProj_xform;

void main()
{
gl_Position = uViewProj_xform* uModel_xform *  vec4(pos, 1.0);
fCol= col;
TexCoord = tex;
}
)raw";

	std::string fShader = R"raw(
#version 450

layout(location = 1)in vec2 TexCoord;
layout(location = 2)in vec4 colour;

out vec4 FragColour;

uniform sampler2D uTex2d;
uniform vec4 col;

void main()
{
    FragColour = texture(uTex2d, TexCoord);

})raw";

	s_Data.shaderid = glCreateProgram();
	const GLchar* ptr = vShader.c_str();
	GLint  len = vShader.length();
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
}


void engine::Renderer2D::BeginScene(const OrthographicCamera& camera)
{
	s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
}

void engine::Renderer2D::EndScene()
{
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
		 0.0f,		0.0f,		1.0f,		0.0f,
		 position.x,position.y, 0.0f,		1.0f
	};

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
	//shd_ref->second.UnUse();
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
		position.x,				position.y,			1.0f, 0.0f,
		 0.0f,					0.0f,				0.0f, 1.0f
	};

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
		position.x,				position.y,			1.0f, 0.0f,
		 0.0f,					0.0f,				0.0f, 1.0f
	};

	glBindVertexArray(s_Data.vaoid);
	glUseProgram(s_Data.shaderid);

	glBindTextureUnit(0, texture);

	GLuint tex_loc = glGetUniformLocation(s_Data.shaderid, "uTex2d");
	glUniform1i(tex_loc, 0);

	GLuint task_loc = glGetUniformLocation(s_Data.shaderid, "uViewProj_xform");
	glUniformMatrix4fv(task_loc, 1, false, glm::value_ptr(s_Data.CameraBuffer.ViewProjection));

	task_loc = glGetUniformLocation(s_Data.shaderid, "uModel_xform");
	glUniformMatrix4fv(task_loc, 1, false, glm::value_ptr(mdl_xform));

	task_loc = glGetUniformLocation(s_Data.shaderid, "col");
	glUniform4fv(task_loc, 1, glm::value_ptr(tintColor));

	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, NULL);
	glUseProgram(0);
	glBindVertexArray(0);
}

}