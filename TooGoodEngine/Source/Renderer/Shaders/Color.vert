#version 460

//---- per vertex data ----
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TextureCoord;

struct InstanceData
{
    mat4 Transform;
    uvec4 MaterialIndex;
};
 
readonly layout(binding = 0) buffer u_Instance
{
    InstanceData Data[];
} Instances;


out VertexData
{
    vec3 WorldPosition;
    vec3 Normal;
    vec2 TextureCoord;
    flat uint MaterialIndex;
} Out;

layout(binding = 4) uniform u_GeometryPassBuffer 
{
	int  PointLightSize;
	int  DirectionalLightSize;
	int  HasCubeMap;
	int  NotUsed;
	vec3 CameraPosition;
	float NumberOfMipMaps;
	mat4 ViewProjection;
} Buffer;

void main()
{
    vec4 WorldPosition = Instances.Data[gl_InstanceID].Transform * vec4(Position, 1.0);
    gl_Position = Buffer.ViewProjection * WorldPosition;

    Out.WorldPosition = WorldPosition.xyz;
    Out.Normal = transpose(inverse(mat3(Instances.Data[gl_InstanceID].Transform))) * Normal; 
    Out.Normal = normalize(Out.Normal);
    Out.TextureCoord = TextureCoord; 
    Out.MaterialIndex = Instances.Data[gl_InstanceID].MaterialIndex.x;
}
