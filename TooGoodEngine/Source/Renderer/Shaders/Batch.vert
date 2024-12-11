#version 460 

layout(location = 0) in mat4x4 Transform;

layout(binding = 0) uniform u_PerFrame 
{
	mat4 ViewProjection;
} Buffer;

void main()
{
	gl_Position = Buffer.ViewProjection * Transform * vec4(1.0, 1.0, 1.0, 0.0);
}