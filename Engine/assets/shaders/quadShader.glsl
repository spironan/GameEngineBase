#type vertex
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

#type fragment
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
}