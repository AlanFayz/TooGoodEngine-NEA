#version 460 core 

layout(location = 0) out vec4 OutColor;

in vec3 o_TextureCoordinate;

uniform samplerCube u_EnviormentMap;

uniform float u_LevelOfDetail;

void main()
{
	OutColor = textureLod(u_EnviormentMap, o_TextureCoordinate, u_LevelOfDetail);
}
