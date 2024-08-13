#version 460 core 

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;

out vec3 o_TextureCoordinate;

void main()
{
	o_TextureCoordinate = a_Position * 2.1;
	vec4 position = u_ViewProjection * vec4(o_TextureCoordinate, 1.0);
	gl_Position = position.xyww;
}