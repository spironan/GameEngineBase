#version 450

//shader input
layout (location = 0) in vec3 inColor;
layout (location = 1) in vec2 texCoord;

//output write
layout (location = 0) out vec4 outFragColor;

layout (set = 0, binding =1) uniform SceneData
{
	vec4 fogColor; //w is for exponent
	vec4 fogDistances; // x is for min, y is for max, zw unused;
	vec4 ambientColor;
	vec4 sunlightDireciton; // w for sun power
	vec4 sunlightColor;
}sceneData;

void main ()
{
vec2 garbage = texCoord;
//return color
outFragColor = vec4(inColor + sceneData.ambientColor.xyz,1.0f);
}