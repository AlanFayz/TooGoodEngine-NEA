#version 460 core 

layout(location = 0) out vec4 OutColor;

in vec2 o_Coordinate;

uniform sampler2D u_Image;

void main()
{
	OutColor = texture(u_Image, o_Coordinate);
}