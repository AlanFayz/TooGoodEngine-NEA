#version 460 core 

out vec2 o_Coordinate;

void main()
{
	o_Coordinate = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
	gl_Position = vec4(o_Coordinate * 2.0 - 1.0, 1.0, 1.0);
}
