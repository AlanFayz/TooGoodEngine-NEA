#version 460

#extension GL_ARB_bindless_texture: require 

layout(location = 0) out vec4 OutColor;

in vec3 oNormal;
in vec2 oTextureCoord;

void main()
{
	float a = dot(oNormal, oNormal);
    float b = dot(oTextureCoord, oTextureCoord);

    OutColor = vec4(1.0, 1.0, 1.0, 1.0) * a * b;
}
