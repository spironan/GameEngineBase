#type vertex
#version 450

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 col;

layout(location = 1)out vec4 fCol;

uniform mat4 uViewProj_xform;

void main()
{
gl_Position = uViewProj_xform*  pos;
fCol = col;
}

#type fragment
#version 450

layout(location = 1)in vec4 colour;

out vec4 FragColour;


void main()
{
    FragColour = colour;
}