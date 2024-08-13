#version 460 core 

layout(location = 0) out vec4 OutColor;

in vec3 o_TextureCoordinate;

uniform samplerCube u_EnviormentMap;

void main()
{
	OutColor = texture(u_EnviormentMap, normalize(o_TextureCoordinate));
}
