#version 460

#extension GL_ARB_bindless_texture: require 

layout(location = 0) out vec4 OutColor;

const int MATERIAL_TYPE_IMAGE  = 1;
const int MATERIAL_TYPE_VECTOR = 2;

struct MaterialComponent 
{
	vec4  Component;
	uvec2 BindlessSampler;
	int   Type;
};

struct Material 
{
	MaterialComponent Ambient;
	MaterialComponent Albedo;
	MaterialComponent Metallic;
	MaterialComponent Emission;
	MaterialComponent Roughness;

	float AlbedoFactor;  
	float MetallicFactor;
	float EmissionFactor;
};

struct MaterialData
{
	vec4  Ambient;
	vec4  Albedo;
	vec4  Metallic;
	vec4  Emission;
	float Roughness;
};

struct PointLight
{
	vec4 Position;
	vec4 Color;
	float Intensity;
	float Radius;
};

struct DirectionalLight
{
	vec4 Direction;
	vec4 Color;
	float Intensity;
};

readonly layout(std430, binding = 1) buffer u_MaterialBuffer 
{
	Material Data[];
} Materials;

readonly layout(std430, binding = 2) buffer u_PointLightBuffer 
{
	PointLight Data[];
} PointLights;

readonly layout(std430, binding = 3) buffer u_DirectionalLightBuffer 
{
	DirectionalLight Data[];
} DirectionalLights;

 //TODO: should package everything into PerFrameData struct
uniform int u_PointLightSize;
uniform int u_DirectionalLightSize;

MaterialData FetchMaterialData(in Material material, in vec2 textureCoordinate)
{
	MaterialData data;

	if(material.Ambient.Type == MATERIAL_TYPE_IMAGE)
		data.Ambient = texture(sampler2D(material.Ambient.BindlessSampler), textureCoordinate);
	else 
		data.Ambient = material.Ambient.Component;

	if(material.Albedo.Type == MATERIAL_TYPE_IMAGE)
		data.Albedo = texture(sampler2D(material.Albedo.BindlessSampler), textureCoordinate) * material.AlbedoFactor;
	else 
		data.Albedo = material.Albedo.Component * material.AlbedoFactor;

	if(material.Metallic.Type == MATERIAL_TYPE_IMAGE)
		data.Metallic = texture(sampler2D(material.Metallic.BindlessSampler), textureCoordinate) * material.MetallicFactor;
	else   
		data.Metallic = material.Metallic.Component * material.MetallicFactor;

	if(material.Emission.Type == MATERIAL_TYPE_IMAGE)
		data.Emission = texture(sampler2D(material.Emission.BindlessSampler), textureCoordinate) * material.EmissionFactor;
	else 
		data.Emission = material.Emission.Component * material.EmissionFactor;

	if(material.Roughness.Type == MATERIAL_TYPE_IMAGE)
		data.Roughness = texture(sampler2D(material.Albedo.BindlessSampler), textureCoordinate).r;
	else 
		data.Roughness = material.Roughness.Component.r;

	return data;
}

in vec3 o_Normal;
in vec2 o_TextureCoord;
in flat uint o_MaterialIndex;

void main()
{
	float a = dot(o_Normal, o_Normal);
    float b = dot(o_TextureCoord, o_TextureCoord);

	MaterialData materialData = FetchMaterialData(Materials.Data[o_MaterialIndex], o_TextureCoord);

	vec4 LightContribution = vec4(1.0);

	//TODO: add proper light calculations

	for(int i = 0; i < u_PointLightSize; i++)
		LightContribution += PointLights.Data[i].Color;

	for(int i = 0; i < u_DirectionalLightSize; i++)
		LightContribution += DirectionalLights.Data[i].Color;

    OutColor = vec4(1.0, 1.0, 1.0, 1.0) * materialData.Albedo * LightContribution + ((a + b) * 0.1);
}
