#version 460 core 

layout(location = 0) out vec4 OutColor;

in vec3 o_TextureCoordinate;

uniform samplerCube u_EnvironmentMap;

uniform float u_LevelOfDetail;

void main()
{
	OutColor = textureLod(u_EnvironmentMap, o_TextureCoordinate, u_LevelOfDetail);
}
