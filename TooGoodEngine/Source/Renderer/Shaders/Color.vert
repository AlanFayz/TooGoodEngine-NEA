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
 
readonly layout(location = 0) buffer u_Instance
{
    InstanceData Data[];
} Instances;

out vec3 oNormal;
out vec2 oTextureCoord;

uniform mat4 ViewProjection;

void main()
{
    gl_Position = ViewProjection * Instances.Data[gl_InstanceID].Transform * vec4(Position, 1.0);

    oNormal = Normal; 
    oTextureCoord = TextureCoord; 
}
