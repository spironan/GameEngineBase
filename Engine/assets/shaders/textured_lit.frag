#version 450

//shader input
layout (location = 0) in vec3 inColor;
layout (location = 1) in vec2 texCoord;

layout (set = 2,  binding = 0) uniform sampler2D tex1;

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
//return color
vec3 color  = texture(tex1,texCoord).xyz;
outFragColor = vec4(color, 1.0f);
}