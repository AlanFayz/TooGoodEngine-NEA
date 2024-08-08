#version 460

//---- per vertex data ----
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TextureCoord;

struct InstanceData
{
    uint MaterialIndex;
    mat4 Transform;
};
 
readonly layout(binding = 0) buffer u_Instance
{
    InstanceData Data[];
} Instances;

out vec3 o_Normal;
out vec2 o_TextureCoord;
out flat uint o_MaterialIndex;

uniform mat4 u_ViewProjection;

void main()
{
    gl_Position = u_ViewProjection * Instances.Data[gl_InstanceID].Transform * vec4(Position, 1.0);

    o_Normal = Normal; 
    o_TextureCoord = TextureCoord; 
    o_MaterialIndex = Instances.Data[gl_InstanceID].MaterialIndex;
}
