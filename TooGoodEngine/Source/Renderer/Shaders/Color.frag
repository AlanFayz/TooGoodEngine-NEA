#version 460

#extension GL_ARB_bindless_texture: require 

layout(location = 0) out vec4 OutColor;

const int MATERIAL_TYPE_IMAGE  = 1;
const int MATERIAL_TYPE_VECTOR = 2;

const float PI = 3.1415926535897932384626433832795;
const float EPSILON = 0.0000001;

const float FLOAT_MAX = 3.402823466e+38;

struct MaterialAttribute 
{
	vec4  Component;
	uvec2 BindlessSampler;
	uvec2 NotUsed;
};

struct Material 
{
	MaterialAttribute Ambient;
	MaterialAttribute Albedo;
	MaterialAttribute Metallic;
	MaterialAttribute Emission;
	MaterialAttribute Roughness;

	vec4 EmissionFactor; //only r is used
};

struct MaterialData
{
	vec4  Ambient;
	vec4  Albedo;
	vec4  Emission;

	float  Metallic;
	float  Roughness;
};

struct PointLight
{
	vec4 PositionAndIntensity; // x y z intensity
	vec4 ColorAndRadius; //r g b radius
};

struct DirectionalLight
{
	vec4 DirectionAndIntensity; //x y z intensity
	vec4 Color;
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

MaterialData FetchMaterialData(in Material material, in vec2 textureCoordinate)
{
	MaterialData data;

	if(material.Ambient.BindlessSampler != uvec2(0, 0)) 
		data.Ambient = texture(sampler2D(material.Ambient.BindlessSampler), textureCoordinate) * material.Ambient.Component;
	else 
		data.Ambient = material.Ambient.Component;

	if(material.Albedo.BindlessSampler != uvec2(0, 0))
		data.Albedo = texture(sampler2D(material.Albedo.BindlessSampler), textureCoordinate) * material.Albedo.Component;
	else 
		data.Albedo = material.Albedo.Component;

	if(material.Metallic.BindlessSampler != uvec2(0, 0))
		data.Metallic = texture(sampler2D(material.Metallic.BindlessSampler), textureCoordinate).r * material.Metallic.Component.r;
	else   
		data.Metallic = material.Metallic.Component.r;

	if(material.Emission.BindlessSampler != uvec2(0, 0))
		data.Emission = texture(sampler2D(material.Emission.BindlessSampler), textureCoordinate) * material.Emission.Component * material.EmissionFactor.r;
	else 
		data.Emission = material.Emission.Component * material.EmissionFactor.r;

	if(material.Roughness.BindlessSampler != uvec2(0, 0))
		data.Roughness = texture(sampler2D(material.Albedo.BindlessSampler), textureCoordinate).r * material.Roughness.Component.r;
	else 
		data.Roughness = material.Roughness.Component.r;

	return data;
}

/*
	Trowbridge-Reitz GGX
	
	Used to statisticaly appromximate the relative surface area of microfacets
	aligned with the halfway vector.
*/

float DistributionGGX(float NdotH, float roughness)
{
	float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH2 = NdotH * NdotH;
	
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return a2 / max(denom, EPSILON); 
}

/*
	Geometry Function which is a combination of GGX and Schlick-Beckmann approximation.
	This decribes the self shadowing properties of a microfacet where one microfacet
	may occlude another (produce a shadow onto another).
*/

float GeometryGGX(float NdotX, float roughness)
{
	float k = (roughness * roughness)/2.0;
	
	float denom = NdotX * (1.0 - k) + k;
	return NdotX / max(denom, EPSILON);
}

float GeometryFun(float NdotV, float NdotL, float roughness)
{
	return GeometryGGX(NdotV, roughness) * GeometryGGX(NdotL, roughness);
}

/*
	the fresnel equation dictates how much reflection there is based on
	the viewing angle.
*/

vec4 FresnelApproximation(float VdotH, vec4 reflectivity)
{
	return reflectivity + (1.0 - reflectivity) * pow(clamp(1.0 - VdotH, 0.0, 1.0), 5.0);
}

float Attenuate(float radius, float dist)
{
	if(radius >= dist)
		return 1.0 / max(dist * dist, EPSILON);

	return 0.0;
}

struct ShadeInfo
{
	MaterialData Data;

	vec3  WorldPosition;
	vec3  Normal;
	vec3  CameraPosition;
	vec3  LightDirection;
	vec3  LightColor;
	float Attenuation;
};

vec4 Shade(in ShadeInfo info)
{
	vec3 radiance = info.LightColor * info.Attenuation;

	//precalculate everything that is needed to increase performance.
	vec3 viewDir    = normalize(info.CameraPosition - info.WorldPosition);
	vec3 halfwayDir = normalize(info.LightDirection + viewDir);

	float NdotL = max(dot(info.Normal, info.LightDirection), EPSILON);
	float NdotV = max(dot(info.Normal, viewDir),			 EPSILON);
	float NdotH = max(dot(info.Normal, halfwayDir),			 EPSILON);
	float VdotH = max(dot(viewDir,     halfwayDir),		     EPSILON);

	//diffuse
	vec4 lambertianDiffuse = info.Data.Albedo / PI;

	vec4 F0 = vec4(0.04); 
	F0  = mix(F0, info.Data.Albedo, info.Data.Metallic);

	//fresnel 
	vec4 ks = FresnelApproximation(VdotH, F0);
	vec4 kd = (vec4(1.0) - ks) * (1.0 - info.Data.Metallic);

	//specular calculations
	vec3 cookTorranceNum =  DistributionGGX(NdotH, info.Data.Roughness)    *
							GeometryFun(NdotV, NdotL, info.Data.Roughness)  *
							ks.rgb;

    float cookTorranceDenom = (4.0 * NdotL * NdotV) + EPSILON;
	vec4 Specular = vec4(cookTorranceNum / cookTorranceDenom, 1.0);

	vec4 BRDF = kd * lambertianDiffuse + Specular; 

	vec4 color = BRDF * vec4(radiance, 1.0) * NdotL;

	return color;
}


in VertexData
{
	vec3 WorldPosition;
	vec3 Normal;
	vec2 TextureCoord;
	flat uint MaterialIndex;
} In;

uniform samplerCube u_CubeMap;

void main()
{
	MaterialData materialData = FetchMaterialData(Materials.Data[In.MaterialIndex], In.TextureCoord);

	//NOTE: may want to remove ambient as we have emission now
	vec4 Color = materialData.Ambient + materialData.Emission;

	ShadeInfo info;
	info.Data = materialData;
	info.Normal = In.Normal;
	info.CameraPosition = Buffer.CameraPosition;

	
	//Point Light Contribution
	for(int i = 0; i < Buffer.PointLightSize; i++)
	{
		float radius    = PointLights.Data[i].ColorAndRadius.a;
		float intensity = PointLights.Data[i].PositionAndIntensity.a;
		vec3 position   = PointLights.Data[i].PositionAndIntensity.rgb;
		vec3 color      = PointLights.Data[i].ColorAndRadius.rgb;

		float attenuation = Attenuate(radius, distance(position, In.WorldPosition));
		if(attenuation <= 0.0)
			continue;

		info.LightDirection = normalize(position - In.WorldPosition); 
		info.LightColor = color * intensity;
		info.Attenuation = attenuation;
		
		Color += Shade(info);
	}

	
	// Directional Light Contribution 
	for(int i = 0; i < Buffer.DirectionalLightSize; i++)
	{
		info.LightDirection = -DirectionalLights.Data[i].DirectionAndIntensity.xyz;

		info.LightColor = DirectionalLights.Data[i].Color.rgb * max(DirectionalLights.Data[i].DirectionAndIntensity.a, 0.0);
		info.Attenuation = 1.0;

		Color += Shade(info);
	}

	//Enviorment Map Contribution 
	if(Buffer.HasCubeMap == 1)
	{
		info.LightDirection = normalize(reflect(In.WorldPosition - Buffer.CameraPosition, In.Normal));
		info.LightColor = textureLod(u_CubeMap, info.LightDirection, (Buffer.NumberOfMipMaps - 1) * materialData.Roughness).rgb;
		
		info.Attenuation = 1.0;

		Color += Shade(info);
	}
	
    OutColor = Color;
}
