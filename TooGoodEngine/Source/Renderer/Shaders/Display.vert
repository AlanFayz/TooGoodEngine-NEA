#version 460 core 

out vec2 o_Coordinate;

void main()
{
	//this saves us having to use a vertex buffer to render a full screen quad.
	//this will render a massive triangle, the edges get clipped so whats left is a quad.

	// gl_VertexID     X      Y
	// 0               0      0
	// 1               2      3
	// 2               0      2

	//anything above 1 is clipped leaving a quad.

	o_Coordinate = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);

	//transform in range from -1.0 to 1.0
	gl_Position = vec4(o_Coordinate * 2.0 - 1.0, 1.0, 1.0);
}
